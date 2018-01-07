#include <iostream>


#define cimg_use_jpeg
#include "..\include\CImg.h"

#include "hough.cpp"
using namespace cimg_library;



int main() {
  
  CImg<unsigned char> original("../data/sudoku.jpg");
  auto image = original.resize(800, (800.0 / (original.width())) * original.height());
  
  std::cout << "w: " << image.width() << " h: " << image.height() << std::endl;

  std::cout << "spectrum: " << image.spectrum() << " depth: " << image.depth() << std::endl;

  auto gs = hough::toGrayScale(image);
  gs.save("grayscale.jpg");
  CImgDisplay gs_disp(gs.width(), gs.height());
  gs_disp.display(gs);

  auto hough = hough::transform(gs);

  CImgDisplay hough_disp;
  hough_disp.display(hough.resize(800, 800));
  while (!gs_disp.is_closed()) {
    gs_disp.wait();
  }
  return 0;
}

