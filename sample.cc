#include <opencv2/opencv.hpp>
#include "som.hpp"

namespace {

enum {
  kImageRow = 100,
  kImageCol = 100,
  kChannel = 3,
  kPixel = 5
};

// http://ja.wikibooks.org/wiki/More_C%2B%2B_Idioms
template <class T>
class NonCopyable {
  protected:
    NonCopyable () {}
    ~NonCopyable () {}
  private: 
    NonCopyable (const NonCopyable &);
    T & operator = (const T &);
};

class MatHolder : private NonCopyable<MatHolder> {
  cv::Mat mat;
 public: 
  explicit MatHolder()
      : mat(cv::Size(kImageRow * kPixel, kImageCol * kPixel),
            CV_8UC3) {}

  void set(const size_t i, const size_t j, const size_t k, const size_t value) {
    mat.at<cv::Vec3b>(i, j)[k] = value;
  }

  size_t at(const size_t i, const size_t j, const size_t k) const {
    return mat.at<cv::Vec3b>(i, j)[k];
  }

  cv::Mat get() const { return mat; }
};

namespace object {

MatHolder matHolder;
brly::Som<double> som(kChannel, kImageRow, kImageCol);

void next();
void copy();
void copyElemSelected(const size_t, const size_t);
void copyPixelSelected(const size_t, const size_t, const size_t, const size_t);

void next() {
  som.apply();
}

void copy() {
  for (size_t i = 0; i < kImageRow; ++i) {
    for (size_t j = 0; j < kImageCol; ++j) {
      copyElemSelected(i, j);
    }
  }
}

void copyElemSelected(const size_t si, const size_t sj) {
  for (size_t i = 0; i < kPixel; ++i) {
    for (size_t j = 0; j < kPixel; ++j) {
      copyPixelSelected(si, sj, i, j);
    }
  }
}


void copyPixelSelected(const size_t si, const size_t sj,
                       const size_t pi, const size_t pj) {
  for (size_t k = 0; k < kChannel; ++k) {
    matHolder.set(si * kPixel + pi,
                  sj * kPixel + pj,
                  k,
                  som.at(si, sj, k) * 255);
  }
}

}; // namespace object

class Viewer : private NonCopyable<Viewer> {

  // static
  static std::string WindowName() { return std::string("Som_Sample"); }
  static void OnMouseCallback(int ev, int x, int y, int f, void*) {
    object::next();
    object::copy();
    cv::imshow(Viewer::WindowName(), object::matHolder.get());
    cv::waitKey(0);
  }
  
 public:
  
  explicit Viewer() {
    init();
  }

  void init() {
    object::copy();
    cv::namedWindow(Viewer::WindowName(),
                    CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
    cv::imshow(Viewer::WindowName(), object::matHolder.get());
    cv::setMouseCallback(Viewer::WindowName(), Viewer::OnMouseCallback);
    cv::waitKey(0);
  }
};

class Application : private NonCopyable<Application> {
 public:
  explicit Application() {
    Viewer view;
  }
};

}; // anonymous namespace

int main() {
  ::Application();
  return 0;
}
