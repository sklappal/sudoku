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

  CImg<unsigned char> transform(const CImg<unsigned char>& gs)
  {
    //auto rhoRange = static_cast<int>(std::ceil(2 * std::max(gs.width(), gs.height()) * std::sqrt(2.0)));
    auto rhoCount = 512;
    auto maxRho = std::max(gs.width(), gs.height()) * std::sqrt(2.0) + 1;    
    auto thetaCount = 512;
    auto thetaIncrement = 1.0 / thetaCount * (1.1 * M_PI);
    auto minTheta = -M_PI * 0.55;

    auto vec = std::vector<int>(rhoCount * thetaCount);

    auto cosCache = std::vector<double>(thetaCount);
    auto sinCache = std::vector<double>(thetaCount);

    auto theta = minTheta;
    for(int i = 0; i < thetaCount; i++)
    {
      cosCache[i] = std::cos(theta);
      sinCache[i] = std::sin(theta);
      theta += thetaIncrement;
    }

    for (int x = 0; x < gs.width(); x++)
    {
      for (int y = 0; y < gs.height(); y++)
      {
        auto imageVal = *gs.data(x, y, 0, 0);
        
        if (imageVal < 100)
        {
          theta = minTheta;
          for (int i = 0; i < thetaCount; i++)
          {
            auto rho = x * cosCache[i] + y * sinCache[i];
            auto rhoIndex = static_cast<int>(((rho + maxRho) / maxRho) * (rhoCount / 2)  + 0.5);
            vec[rhoIndex * thetaCount + i] += 1;            
            theta += thetaIncrement;
          }
        }
      }      
    }

    auto max = *std::max_element(vec.begin(), vec.end());
    auto ret = CImg<unsigned char>(thetaCount, rhoCount, 1, 1, 0);
    for (int theta = 0; theta < thetaCount; theta++)
    {
      for (int rho = 0; rho < rhoCount; rho++)
      {      
        auto vecVal = (((double)vec[rho * thetaCount + theta]) / max) * 255;        
        *ret.data(theta, rho, 0, 0) = static_cast<unsigned char>(vecVal + 0.5);        
      }
    }

    return ret;
  }
}