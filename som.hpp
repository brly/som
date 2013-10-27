#ifndef SOM_HPP
#define SOM_HPP

// This source files use C++11 funtion, notations.

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <cmath>
#include <functional>
#include <iostream>

namespace brly {

// unsigned int
typedef unsigned uint;
// constant Infinity
const uint Infinity = 1 << 28;

namespace util {

template <typename T>
inline T min(const T& lhs, const T& rhs) {
  return lhs < rhs ? lhs : rhs ;
}

template <typename T>
inline T max(const T& lhs, const T& rhs) {
  return lhs > rhs ? lhs : rhs ;
}

template <typename T>
inline T abs(const T& value) {
  return 0 < value ? value : -value ;
}

};


template <typename Type>
class Som {

  // Table
  struct Table {
    explicit Table(const uint aCol, const uint aRow)
        : Table([=]() -> uint { return aCol; },
                [=]() -> uint { return aRow; }) {}
    Table(const std::function<uint()> aCol, const std::function<uint()> aRow)
        : Col(aCol), Row(aRow) {}
    const std::function<uint()> Col;
    const std::function<uint()> Row;
  };
  
  // Feature
  struct Feature {
    explicit Feature(const uint aDim, const uint aCol, const uint aRow)
        : table(Table(aCol, aRow)),
          Dim([=]() -> uint {return aDim; }) {}
    const Table table;
    const std::function<uint()> Dim;
  };
  
  // Unit
  struct Unit {
    explicit Unit(const uint aI, const uint aJ)
        : Unit([=]() -> uint { return aI; },
               [=]() -> uint { return aJ; } ) {}
    Unit(const std::function<uint()> aI, const std::function<uint()> aJ)
        : I(aI), J(aJ) {}
    const std::function<uint()> I;
    const std::function<uint()> J;
  };
  
  // DistanceFunction
  struct DistanceFunction {
    typedef std::vector<Type> Elem;
    typedef std::function<Type(const Elem *lhs, const Elem *rhs)> FunctionType;
    static FunctionType euclidean() {
      return [](const Elem *l, const Elem *r) -> Type {
        const uint MaxSize = util::min(l->size(), r->size());
        Type returnValue = 0;
        for (uint i = 0; i < MaxSize; ++i) {
          returnValue += util::abs((l->at(i)) - (r->at(i)));
        }
        return sqrt(returnValue);
      };
    }
    
    static FunctionType dot() {
      return [](const Elem *l, const Elem *r) -> Type {
        const uint MaxSize = util::min(l->size(), r->size());
        Type returnValue = 0;
        for (uint i = 0; i < MaxSize; ++i) {
          returnValue += (*l[i]) * (*r[i]);
        }
        return returnValue;
      };
    }
  };
  
  // OutputLayer
  class OutputLayer {
    typedef std::vector<Type> Elem;
    typedef std::vector<Elem> ElemVec;
    typedef std::vector<ElemVec> ElemMat;
    
    const Feature feature;
    ElemMat data;
    uint t;

    Unit searchBestMatchUnit(const Elem *inputData) {
      uint bmuI = Infinity, bmuJ = Infinity;
      double bmuValue = Infinity;
      std::function<Type(const Elem *l, const Elem *r)> distanceFunctionSubstance =
          DistanceFunction::euclidean();
      for (uint i = 0; i < feature.table.Row(); ++i) {
        for (uint j = 0; j < feature.table.Col(); ++j) {
          double tempValue =
              // 0;
              // DistanceFunction::euclidean()(inputData, &data[i][j]);
              // double tempValue = distanceFunctionSubstance(inputData, &data[i][j]);
               distanceFunctionSubstance(inputData, &data[i][j]);
          
          if (bmuI == Infinity ||
              tempValue < bmuValue) {
            bmuI = i, bmuJ = j, bmuValue = tempValue;
          }
        }
      }
      return Unit(bmuI, bmuJ);
    }

    void update(const Elem *inputData, const Unit& bestMatchUnit) {
      const double Total =
          static_cast<double>(feature.table.Row() * feature.table.Col());
      
      for (int i = 0; i < feature.table.Row(); ++i) {
        for (int j = 0; j < feature.table.Col(); ++j) {
          const int di = static_cast<int>(bestMatchUnit.I()) - i;
          const int dj = static_cast<int>(bestMatchUnit.J()) - j;
          double distance = sqrt(pow(di, 2.0) + pow(dj, 2.0));
          double alpha = 1.0 - 0.0001 * t;
          if (distance > 1e-6) {
            alpha /= distance;
          }
          if (sqrt(sqrt(Total)) - 0.0001 * t > distance) {
            for (uint k = 0; k < feature.Dim(); ++k) {
              data[i][j][k] += alpha * ((*inputData)[k] - data[i][j][k]);
            }
            normalizeElem(&data[i][j]);
          }
        }
      }
    }

    void normalizeElem(Elem *elem) const {
      Type sum = 0;
      for (uint i = 0; i < feature.Dim(); ++i)
        sum += pow(elem->at(i), 2.0);
      sum = sqrt(sum);
      for (uint i = 0; i < feature.Dim(); ++i)
        ((*elem)[i]) /= sum;
    }

    Elem createRandomInputData() {
      Elem randomInputData(feature.Dim());
      for (uint i = 0; i < feature.Dim(); ++i) {
        randomInputData[i] = OutputLayer::randomValue();
      }
      normalizeElem(&randomInputData);
      return randomInputData;
    }

    static Type randomValue() {
      return static_cast<Type>(rand());
    }

    void normalizeAllElem() {
      for (size_t i = 0; i < feature.table.Row(); ++i)
        for (size_t j = 0; j < feature.table.Col(); ++j)
          normalizeElem(&data[i][j]);
    }

    void initializeData() {
      srand((unsigned)time(NULL));      
      for (size_t i = 0; i < feature.table.Row(); ++i)
        for (size_t j = 0; j < feature.table.Col(); ++j)
          for (size_t k = 0; k < feature.Dim(); ++k)
            data[i][j][k] = OutputLayer::randomValue();
      normalizeAllElem();
    }

   public:

    explicit OutputLayer(const Feature f)
        : feature(f),
          data(f.table.Row(), ElemVec(f.table.Col(), Elem(f.Dim()))),
          t(0) {
      initializeData();
    }

    Type at(const uint i, const uint j, const uint k) const {
      return data[i][j][k];
    }

    void apply() {
      const Elem e = createRandomInputData();
      apply(&e);
    }
    
    void apply(const Elem *inputData) {
      const Unit BestMatchUnit = searchBestMatchUnit(inputData);
      update(inputData, BestMatchUnit);
      ++t;
    }

    uint row() const {
      return feature.table.Row();
    }

    uint col() const {
      return feature.table.Col();
    }

    uint dim() const {
      return feature.Dim();
    }
    
  }; // class OutputLayer

  OutputLayer layer;
 
 public:
  
  explicit Som() : Som(3, 128, 128) {}
  Som(const uint aDim, const uint aRow, const uint aCol)
      : layer(Feature(aDim, aRow, aCol)) {}

  void apply() {
    layer.apply();
  }

  void apply(const std::vector<Type> *inputData) {
    layer.apply(inputData);
  }

  Type at(const uint i, const uint j, const uint k) const {
    return layer.at(i, j, k);
  }

  uint row() const {
    return layer.row();
  }

  uint col() const {
    return layer.col();
  }

  uint dim() const {
    return layer.dim();
  }
  
}; // class Som

}; // namespace brly

#endif // SOM_HPP_
