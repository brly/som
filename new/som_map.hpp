#ifndef SOM_MAP_HPP
#define SOM_MAP_HPP

#include <vector>
#include "element.hpp"
#include "util.hpp"

namespace brly {

template <class T>
class SomMap {
  typedef std::vector<Element<T> > SomSubMap;
  std::vector<SomSubMap> mMap;

  struct Location {
    int mX;
    int mY;
    explicit Location(const int aX, const int aY) : mX(aX), mY(aY) {}
    int getX() const {
      return mX;
    }
    int getY() const {
      return mY;
    }
  };
  
 public:
  explicit SomMap(const int aDim, const int aRow, const int aCol)
      : mMap(aRow, SomSubMap(aCol)) {}

  void Update() {
    Update(Element<T>::CreateRandomElement());
  }

  void Update(const Element<T>& aElement) {
    const Location bestMatchUnit = findBestMatchUnit(aElement);
    
  }

  Element<T> Get(const int aVerticalIndex, const int aHorizontalIndex) {
    return Element<T>(1);
  }

 private:

  // T型とインデックス
  typedef std::pair<T, int> PairTInt;
  
  // マップの中から引数の要素に最も近い要素の場所を返す
  Location findBestMatchUnit(Element<T> element) {
    const int N = mMap.size();
    Location nearest(-1, -1);
    PairTInt pairTInt(1<<28, -1);
    
    for (int i = 0; i < N; i++) {
      PairTInt tmp = findBestMatchUnit(element, i);
      if (tmp.first < pairTInt.first) {
        pairTInt = tmp;
        nearest = Location(pairTInt.second, i);
      }
    }
    
    return nearest;
  }

  // 引数にインデックスがある場合は mMap 中の行のインデックスを示し、
  // 行の中で最も引数の要素に近い場所の列インデックスと距離を返す
  PairTInt findBestMatchUnit(Element<T> aElement, int aIndex) {
    const SomSubMap subMap = mMap[aIndex];
    const int N = subMap.size();
    
    T minDiff = static_cast<T>(1 << 28);
    int col = -1;
    
    for (int i = 0; i < N; i++) {
      T diff = aElement.distance(subMap[i]);
      // より引数要素に近い要素が見つかった場合
      if (col == -1 || minDiff > diff) {
        col = i;
        minDiff = diff;
      }
    }
    return PairTInt(minDiff, col);
  }
  
}; // class SomMap

}; // namespace brly

#endif 











