#ifndef UTIL_HPP
#define UTIL_HPP

namespace brly {

template <typename T>                                                                                                                     inline T min(const T& lhs, const T& rhs) {
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

#endif
