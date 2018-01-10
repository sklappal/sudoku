#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "..\include\CImg.h"
using namespace cimg_library;

namespace hough
{

  CImg<unsigned char> toGrayScale(const CImg<unsigned char>& image)
  {
    auto ret = CImg<unsigned char>(image.width(), image.height(), 1, 1, 0);
    for (int x = 0; x < image.width(); x++)
    {
      for (int y = 0; y < image.height(); y++)
      {
        auto R = *image.data(x, y, 0, 0);
        auto G = *image.data(x, y, 0, 1);
        auto B = *image.data(x, y, 0, 2);

        auto target = ret.data(x, y, 0, 0);
        *target = 0.299 * R + 0.587 * G + 0.114 * B;
      }
    }
    return ret;
  }

  namespace
  {
    auto rhoCount = 1024;
    auto thetaCount = 1024;
    auto thetaIncrement = 1.0 / thetaCount * (1.1 * M_PI);
    auto minTheta = -M_PI * 0.55;

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

  CImg<unsigned char> transform(CImg<unsigned char>& gs)
  {
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


    auto ret = CImg<unsigned char>(thetaCount, rhoCount, 1, 1, 0);
    for (int theta = 0; theta < thetaCount; theta++)
    {
      for (int rho = 0; rho < rhoCount; rho++)
      {      
        *ret.data(theta, rho, 0, 0) = round(normalized[rho * thetaCount + theta] * 255);
      }
    }

    auto maxCount = 5u;
    auto maxIndices = findMaxIndices(normalized, maxCount);

    for (size_t i = 0; i < maxCount; i++)
    {
      auto index = maxIndices[i];
      auto thetaIndex = index % thetaCount;
      auto theta = indexToTheta(thetaIndex);
      auto rho = indexToRho(index / thetaCount, maxRho);
      auto cost = cosCache[thetaIndex];
      auto sint = sinCache[thetaIndex];      

      // line equation: rho = x cos (theta) + y sin (theta)
      if (std::abs(theta) > 1e-2)
      {
        // normal case, find x-intersection
        auto x_0 = rho / cost;
        auto y_0 = 0.0;
        
        auto y_1 = gs.height();
        auto x_1 = (rho - y_1 * sint) / cost;
        
        unsigned char color = 255u;
        gs.draw_line(x_0, y_0, x_1, y_1, &color);
      }
      else
       {
        // along the x axis, find y-intersection
        auto x_0 = 0.0;
        auto y_0 = rho / sint;

        auto x_1 = gs.width();
        auto y_1 = (rho - x_1 * cost) / sint;
        unsigned char color = 255u;
        gs.draw_line(x_0, y_0, x_1, y_1, &color);
      }
     

      std::cout << "Theta: " << indexToTheta(index % thetaCount);
      std::cout << " Rho: " << indexToRho(index / thetaCount, maxRho);
      std::cout << " Val: " << normalized[index] << std::endl;
    }

    

    return ret;
  }
}