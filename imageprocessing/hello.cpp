#include <iostream>
#include <vector>


#define cimg_use_jpeg
#include "..\include\CImg.h"

#include "imageprocessing.h"
#include "hough.h"
using namespace cimg_library;



int main() {
  
  CImg<unsigned char> original("../data/sudoku.jpg");
  auto image = original.resize(800, (800.0 / (original.width())) * original.height());
  
  std::cout << "w: " << image.width() << " h: " << image.height() << std::endl;
  std::cout << "spectrum: " << image.spectrum() << " depth: " << image.depth() << std::endl;

  auto imageList = CImgList<unsigned char>();
  
  auto names = std::vector<std::string>();

  auto gs = imageprocessing::toGrayScale(image);
  imageList.insert(gs);
  names.push_back("gray scale");

  auto binary = imageprocessing::binarize(gs, 90);
  imageList.insert(binary);
  names.push_back("binary");

  auto inverted = imageprocessing::invert(binary);
  imageList.insert(inverted);
  names.push_back("inverted");

  auto eroded1 = imageprocessing::erode(inverted, 1);
  imageList.insert(eroded1);
  names.push_back("eroded1");

  auto closed = imageprocessing::close(inverted, 1);
  imageList.insert(closed);
  names.push_back("closed");

  auto inverted_closed = imageprocessing::invert(closed);
  imageList.insert(inverted_closed);
  names.push_back("inverted_closed");

  auto hough = hough::transform(inverted_closed, imageList, names);

  std::vector<CImgDisplay> imgDisplays;
  for (size_t i = 0; i < imageList.size(); i++)
  {
    CImgDisplay imgdisplay;
    auto img = imageList.at(i);
    unsigned char gray[] = {127u, 127u, 127u};
    img.draw_text(100, 100, names[i].c_str(), gray);    
    imgdisplay.display(img);
    imgDisplays.push_back(imgdisplay);
  }

  CImgDisplay orig_disp;
  orig_disp.display(original);
  while (!orig_disp.is_closed()) {
    orig_disp.wait();
  }
  return 0;
}