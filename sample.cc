#include "som.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef std::vector<double> elem;
typedef std::vector<elem> vec;
typedef std::vector<vec> mat; 

namespace {

// Output layer size.
const int kRow = 100;
const int kCol = 100;
const int kDim = 3;

// Pixel per color.
const int kPixel = 5;

Mat screen(Size(kRow * kPixel, kCol * kPixel), CV_8UC3);

// SOM object
som::Som<double, kDim, kRow, kCol> *som_obj;

}; // anonymous namespace

// ------------------------------------------------------------
// Prototype definition

// Copy data from som_obj to screen.
void copy_to_opencv_mat();

// Run window, it calls waitKey().
void run_window();

// Event-handler, relate with window.
void on_mouse(int event, int x, int y, int flag, void*);

// ------------------------------------------------------------
// Implementation
void copy_to_opencv_mat() {
  const mat& m = som_obj->data();
  for (unsigned i = 0; i < kRow; ++i) {
    for (unsigned j = 0; j < kCol; ++j) {
      for (unsigned ii = 0; ii < kPixel; ++ii) {
        for (unsigned jj = 0; jj < kPixel; ++jj) {
          const unsigned kI = i * kPixel + ii;
          const unsigned kJ = j * kPixel + jj;
          for (unsigned k = 0; k < kDim; ++k) {
            screen.at<Vec3b>(kI, kJ)[k] = m[i][j][k] * 255;
          }
        }
      }
    }
  }
}

void run_window() {
  copy_to_opencv_mat();
  namedWindow("opencv_window", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  setMouseCallback("opencv_window", on_mouse, 0);
  imshow("opencv_window", screen);
  waitKey(0);
}

void on_mouse(int event, int x, int y, int flag, void*) {
  som_obj->next();
  copy_to_opencv_mat();
  imshow("opencv_window", screen);
}

// ------------------------------------------------------------
// entry point
int main() {
  // initialize
  som_obj = new som::Som<double, kDim, kRow, kCol>();
  // run window
  run_window();
  return 0;
}
