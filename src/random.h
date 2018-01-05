#pragma once

#include <random>
#include <type_traits>

template <typename T> struct distribution_traits {
  template <typename U>
  static typename std::enable_if<std::is_integral<U>::value,
                                 std::uniform_int_distribution<U> >::type
  dist();

  template <typename U>
  static typename std::enable_if<std::is_floating_point<U>::value,
                                 std::uniform_real_distribution<U> >::type
  dist();

  using type = decltype(dist<T>());
};

class Random {
  std::mt19937 engine_ = std::mt19937{ std::random_device()() };

  static Random &instance() {
    static Random r;
    return r;
  }

  template <typename T> T _next() {
    using dist_t = typename distribution_traits<T>::type;
    auto dist = dist_t();
    return dist(engine_);
  }

  template <typename T> T _next(const T &max) {
    using dist_t = typename distribution_traits<T>::type;
    auto dist = dist_t({}, max);
    return dist(engine_);
  }

public:
  template <typename T> static T next() { return instance()._next<T>(); }

  template <typename T> static T next(const T &max) {
    return instance()._next<T>(max);
  }
};
