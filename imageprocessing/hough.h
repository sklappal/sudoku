#include "..\include\CImg.h"

using namespace cimg_library;

namespace imageprocessing
{

  typedef unsigned char uchar;

  CImg<uchar> toGrayScale(const CImg<uchar>& image);

  CImg<uchar> binarize(const CImg<uchar>& image, uchar threshold);

  CImg<uchar> dilate(const CImg<uchar>& image, int radius);
  
  CImg<uchar> erode(const CImg<uchar>& image, int radius);

  CImg<uchar> close(const CImg<uchar>& image, int radius);

  CImg<uchar> open(const CImg<uchar>& image, int radius);

  CImg<uchar> whiten(const CImg<uchar>& image, int radius);

  CImg<uchar> transform(CImg<uchar>& image, CImgList<uchar>& imageList);
}