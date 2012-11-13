#ifndef SOM_HPP_
#define SOM_HPP_

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cmath>

namespace som {

template <class Type, unsigned Dim, unsigned Row, unsigned Column>
class Som {
  typedef std::vector<Type> Elem;
  typedef std::vector<Elem> Vec;
  typedef std::vector<Vec> Mat;

  unsigned t, type_;
  Mat data_;
  
  void normalize_elem(Elem& elem) {
    double sum = 0;
    for (unsigned i = 0; i < Dim; ++i) sum += elem[i] * elem[i];
    sum = sqrt(sum);
    for (unsigned i = 0; i < Dim; ++i) elem[i] /= sum;
  }

  void initialize_randomly_elem(Elem& elem) {
    for (unsigned i = 0; i < Dim; ++i) elem[i] = rand();
    normalize_elem(elem);
  }

  double calc_euclidean_distance(const Elem& lhs, const Elem& rhs) {
    double ret = 0;
    for (unsigned i = 0; i < Dim; ++i) ret += fabs(lhs[i] - rhs[i]);
    return sqrt(ret);
  }

  double calc_dot(const Elem& lhs, const Elem& rhs) {
    double ret = 0;
    for (unsigned i = 0; i < Dim; ++i) ret += lhs[i] * rhs[i];
    return ret;
  }

  void apply_data_and_update(const Elem& input) {
    // search BMU
    int bmuI = -1, bmuJ = -1;
    double bmuValue = -1;
    for (int i = 0; i < Row; ++i) {
      for (int j = 0; j < Column; ++j) {
        switch (type_) {
          case 0: {
            double value = calc_euclidean_distance(input, data_[i][j]);
            if (bmuI == -1 || value < bmuValue)
              bmuI = i, bmuJ = j, bmuValue = value;
            break;
          }
          case 1: {
            double value = calc_dot(input, data_[i][j]);
            if (bmuI == -1 || value > bmuValue)
              bmuI = i, bmuJ = j, bmuValue = value;
            break;
          }
          default: {
            perror("error. invalid type of similarity function.");
            assert(false);
          }
        }
      }
    }

    // apply data
    static const double kTotal = Row * Column;
    for (int i = 0; i < Row; ++i) {
      for (int j = 0; j < Column; ++j) {
        double dist = sqrt(pow(bmuI - i, 2.0) + pow(bmuJ - j, 2.0));
        double alpha = (1.0 - 0.0001 * t);
        if (dist > 1e-6)
          alpha /= dist;
        if (sqrt(sqrt(kTotal)) - 0.0001 * t > dist) {
          for (unsigned k = 0; k < Dim; ++k) {
            data_[i][j][k] += alpha * (input[k] - data_[i][j][k]);
          }
          normalize_elem(data_[i][j]);
        }
      }
    }
  }

  void init() {
    srand((unsigned)time(NULL));
    for (unsigned i = 0; i < Row; ++i) {
      for (unsigned j = 0; j < Column; ++j) {
        initialize_randomly_elem(data_[i][j]);
      }
    }
  }
  
 public:
  Som(unsigned type = 0) : t(0), data_(Row, Vec(Column, Elem(Dim))), type_(type) {
    init();
  }

  void next(const Elem& input) {
    apply_data_and_update(input);
    ++t;
  }
  
  void next() {
    Elem e(3);
    initialize_randomly_elem(e);
    apply_data_and_update(e);
    ++t;
  }
  
  const Mat& data() const { return data_; }
};

}; // namespace of som

#endif // SOM_HPP_
