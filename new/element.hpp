#ifndef ELEMENT_HPP
#define ELEMENT_HPP


#include <vector>
#include <cstdlib>
#include <ctime>

namespace brly {

template <class T>
class Element {
  std::vector<T> mData;
 public:
  explicit Element(const int aDim) : mData(aDim) {}

  T operator[](int aIndex) {
    return mData[aIndex];
  }
  
  size_t getSize() const {
    return mData.size();
  }

  // 引数要素との距離を計算する
  T distance(const Element<T> other) const {
    T sum = static_cast<T>(0);
    const int N = min(mData.size(), other.getSize());
    for (int i = 0; i < N; i++) {
      sum += abs(mData[i] - other[i]);
    }
    return sqrt(sum); // implicit cast
  }

  // 引数次元の要素を作成する
  static Element<T> CreateRandomElement(const int aDim) {
    // ランダムな値を乱数のシードにセット
    srand((unsigned)time(NULL));
    // 各要素にランダムな値を代入する
    Element<T> randomElement(aDim);
    for (int i = 0; i < aDim; i++) {
      randomElement[i] = static_cast<T>(rand());
    }
    return randomElement;
  }
  
}; // class Element

}; // namespace brly

#endif
