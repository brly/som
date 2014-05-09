#ifndef SOM_MAP_HPP
#define SOM_MAP_HPP

#include <vector>
#include <cmath>
#include "element.hpp"
#include "util.hpp"

namespace brly {

template <class T>
class SomMap {
  typedef std::vector<Element<T> > SomSubMap;
  std::vector<SomSubMap> mMap;

  // 経過時間を管理する
  struct Time {
    unsigned mValue;
    explicit Time () : mValue(0) {}
    int get() const {
      return mValue;
    }
    int increment() {
      ++mValue;
    }
    
  } mTime;

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

  // 引数にインデックスがある場合は mMap 中の行のインデックスを示し
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

  void updateMapImpl(const Location aBestMatchUnit) {
    const int Row = mMap.size(), Col = mMap[0].size();
    const int N = Row * Col;

    for (int i = 0; i < Row; i++) {
      
    }
  }

  void updateMapImpl(const Location aBestMatchUnit,
                     const int aIndex, const int aCol, const int N) {
    for (int i = 0; i < aCol; i++) {
      // BestMatchUnitと距離を距離を計算する
      const double distance =
          calcDistance(aBestMatchUnit.getX(), aBestMatchUnit.getY(),
                       i, aIndex);
      // 対象要素の寄与率を計算する
      // const double alpha = 1.0 - 0.0001 * mTime.get();
    }
  }

  // 引数座標ペアの距離を計算する
  double calcDistance(const int aX0, const int aY0,
                      const int aX1, const int aY1) {
    return sqrt(pow(aX0 - aX1, 2.0) + pow(aY0 - aY1, 2.0));
  }

  // 引数距離に応じた寄与率を計算する
  double calcAlpha(const double aDistance) {
    
  }
  
}; // class SomMap

}; // namespace brly

#endif 


