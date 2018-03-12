#include "..\include\CImg.h"

using namespace cimg_library;

namespace hough
{

  typedef unsigned char uchar;

  CImg<uchar> transform(CImg<uchar>& image, CImgList<uchar>& imageList, std::vector<std::string>& imageNames);
}