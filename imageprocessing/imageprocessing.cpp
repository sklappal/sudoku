#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "line.h"
#include "imageprocessing.h"
#include "../timer.h"

using namespace cimg_library;

namespace imageprocessing
{
  namespace
  {

    CImg<uchar> getStructuringElement(int radius)
    {
      auto diam = radius * 2 + 1;
      CImg<uchar> ret = CImg<uchar>(diam, diam, 1, 1, 0);

      // diam = 2 * r + 1
      // midpoint = r

      cimg_forXY(ret, x, y) {
        auto sqrDist = (x-radius)*(x-radius) + (y-radius)*(y-radius);
        if (sqrDist <= radius*radius)
        {
          ret(x, y) = 255;
        }
        else
        {
          ret(x, y) = 0;
        }
      }
      return ret;
    }
  }

  CImg<uchar> toGrayScale(const CImg<uchar>& image)
  {
    auto t = common::timer("To gray scale");
    auto ret = CImg<uchar>(image.width(), image.height(), 1, 1, 0);
    
    cimg_forXY(image, x, y) {
      auto R = image(x, y, 0, 0);
      auto G = image(x, y, 0, 1);
      auto B = image(x, y, 0, 2);

      ret(x, y) = 0.299 * R + 0.587 * G + 0.114 * B;
    }
  
    return ret.normalize(0, 255u);
  }

  CImg<uchar> invert(const CImg<uchar>& image)
  {
    auto t = common::timer("Invert");
    auto ret = CImg<uchar>(image);
   
    cimg_forXY(image, x, y) {
      ret(x, y) = 255 - image(x, y);
    }

    return ret;
  }

  // Didn't figure out how to use CImg::threshold
  CImg<uchar> binarize(const CImg<uchar>& image, uchar threshold)
  {
    auto t = common::timer("Binarize");
    auto ret = CImg<uchar>(image);
    cimg_forXY(image, x, y) {
      if (image(x, y) <= threshold)
      {
        ret(x, y) = 0;
      }
      else
      {
        ret(x, y) = 255;
      }
    }
    return ret;
  }

  CImg<uchar> dilate(const CImg<uchar>& image, int radius)
  {
    auto t = common::timer("Dilate");
    auto ret = CImg<uchar>(image);
    auto element = getStructuringElement(radius);
    ret.dilate(element);
    return ret;
  }

  CImg<uchar> erode(const CImg<uchar>& image, int radius)
  {
    auto t = common::timer("Erode");
    auto ret = CImg<uchar>(image);
    auto element = getStructuringElement(radius);
    ret.erode(element);
    return ret;
  }

  CImg<uchar> open(const CImg<uchar>& image, int radius)
  {
    auto t = common::timer("Open");
    return dilate(erode(image, radius), radius);
  }

  CImg<uchar> close(const CImg<uchar>& image, int radius)
  {
    auto t = common::timer("Close");
    return erode(dilate(image, radius), radius);
  }
}

