#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "line.h"
#include "hough.h"
#include "gridfinder.h"
#include "../timer.h"

using namespace cimg_library;

namespace hough
{
  namespace
  {
    auto rhoCount = 1024;
    auto thetaCount = 1024;
    auto lineFindCount = 1000u;
    
    auto thetaIncrement = 1.0 / (thetaCount-1) * (M_PI);
    auto minTheta = -M_PI * 0.50;
    //auto maxTheta = M_PI * 0.55;

    int round(double value)
    {
      return static_cast<int>(value + (value >= 0.0 ? 0.5 : -0.5));
    }

    double indexToTheta(int index)
    {
      return minTheta + index * thetaIncrement;
    }

    int rhoToIndex(double rho, double maxRho)
    {
      return round(((rho + maxRho) / maxRho) * (rhoCount / 2));
    }

    double indexToRho(int index, double maxRho)
    {
      return ((2 * index * maxRho) / rhoCount) - maxRho;
    }

    std::vector<double> normalize(const std::vector<int>& vec)
    {
      auto max = *std::max_element(vec.begin(), vec.end());
      auto ret = std::vector<double>(vec.size());
      for (size_t i = 0; i < vec.size(); i++)
      {
        ret[i] = ((double)vec[i]) / max;
      }

      return ret;
    }

    struct Comp{
      Comp(const std::vector<double>& v ) : _v(v) {}
      bool operator ()(int a, int b) { return _v[a] > _v[b]; }
      const std::vector<double>& _v;
    };

    std::vector<int> findMaxIndices(std::vector<double> values, int count)
    {
      auto vx = std::vector<int>(values.size());
      
      for(size_t i = 0; i < vx.size(); i++)
      {
          vx[i] = i;
      } 
      std::partial_sort(vx.begin(), vx.begin()+count, vx.end(), Comp(values));
      return vx;
    }
  }


  CImg<uchar> transform(CImg<uchar>& gs, CImgList<uchar>& imageList, std::vector<std::string>& imageNames)
  {
    auto though = common::timer("Hough transform");
    auto maxRho = std::max(gs.width(), gs.height()) * std::sqrt(2.0) + 1; 

    auto cosCache = std::vector<double>(thetaCount);
    auto sinCache = std::vector<double>(thetaCount);

    for(int i = 0; i < thetaCount; i++)
    {
      auto theta = indexToTheta(i);
      cosCache[i] = std::cos(theta);
      sinCache[i] = std::sin(theta);
    }

    auto vec = std::vector<int>(rhoCount * thetaCount);
    for (int x = 0; x < gs.width(); x++)
    {
      for (int y = 0; y < gs.height(); y++)
      {
        auto imageVal = *gs.data(x, y, 0, 0);
        
        if (imageVal < 100)
        {
          for (int i = 0; i < thetaCount; i++)
          {
            auto rho = x * cosCache[i] + y * sinCache[i];
            auto rhoIndex = rhoToIndex(rho, maxRho);
            vec[rhoIndex * thetaCount + i] += 1;         
          }
        }
      }      
    }

    auto normalized = normalize(vec);

    auto ret = CImg<uchar>(thetaCount, rhoCount, 1, 1, 0);
    for (int theta = 0; theta < thetaCount; theta++)
    {
      for (int rho = 0; rho < rhoCount; rho++)
      {      
        *ret.data(theta, rho, 0, 0) = round(normalized[rho * thetaCount + theta] * 255);
      }
    }
    though.stop();

    auto tlinefind = common::timer("Line finding & drawing");
    auto maxIndices = findMaxIndices(normalized, lineFindCount);
    auto parametrized_lines = std::vector<parametrized_line>();
    for (size_t i = 0; i < lineFindCount; i++)
    {
      auto index = maxIndices[i];
      auto thetaIndex = index % thetaCount;
      auto theta = indexToTheta(thetaIndex);
      auto rho = indexToRho(index / thetaCount, maxRho);

      auto p_line = parametrized_line(rho, theta);
      parametrized_lines.push_back(p_line);
    }   

    parametrized_lines = hough::find_grid(parametrized_lines, maxRho);

    auto lines = std::vector<line>();
    for (size_t i = 0; i < parametrized_lines.size(); i++)
    {
      auto p_line = parametrized_lines.at(i);
      auto theta = p_line.theta();
      auto rho = p_line.rho();

      auto cost = std::cos(theta);
      auto sint = std::sin(theta);      

      // line equation: rho = x cos (theta) + y sin (theta)
      int x_0, x_1, y_0, y_1;
      if (std::abs(cost) > 1e-1)
      {
        // normal case, find x-intersection
        x_0 = round(rho / cost);
        y_0 = 0;

        y_1 = gs.height();
        x_1 = round((rho - y_1 * sint) / cost);        
      }
      else
      {
        // along the x axis, find y-intersection
        x_0 = 0;
        y_0 = round(rho / sint);

        x_1 = gs.width();
        y_1 = round((rho - x_1 * cost) / sint);
      }
      lines.push_back(line(x_0, y_0, x_1, y_1));
    }   

    auto with_lines = CImg<uchar>(gs);
    uchar color = 127u;    
    for(size_t i = 0; i < lines.size(); i++)
    {
      auto line = lines[i];
      with_lines.draw_line(line.x0(), line.y0(), line.x1(), line.y1(), &color);      
    }

    imageList.insert(with_lines);
    imageNames.push_back("with lines");
    imageList.insert(ret);
    imageNames.push_back("hough transformed");
    return ret;
  }
}
