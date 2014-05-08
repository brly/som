#ifndef SOM_HPP
#define SOM_HPP

/**
   自己組織化写像を計算するクラス

   クラス内では変化し続けるマップ(写像)の状態を扱う必要がある
   外部からの要請は以下の大きな二つになる
   - マップの更新
   - 指定位置のマップの情報の取得
 */

#include "element.hpp"
#include "som_map.hpp"

namespace brly {

template<class T>
class Som {
  // マップを管理しているオブジェクト
  SomMap<T> mSomMap;
 public:
  // aDim := マップ中の各要素の次元数
  // aRow, aCol := マップの縦横サイズ
  explicit Som(const int aDim, const int aRow, const int aCol) {}

  // マップの更新を行う
  // SomMapに委譲
  void Update() {
    mSomMap.Update();
  }
  void Update(const Element<T>& aElement) {
    mSomMap.Update(aElement);
  }

  // 引数位置のマップの要素を取得
  Element<T> Get(const int aVerticalIndex, const int aHorizontalIndex) {
    return mSomMap.Get(aVerticalIndex, aHorizontalIndex);
  }
}; // class Som
}; // namespace brly

#endif










