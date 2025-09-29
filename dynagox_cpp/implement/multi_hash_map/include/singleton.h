#ifndef DYNAGOX_SINGLETON_H
#define DYNAGOX_SINGLETON_H

#include <cstddef>

template <typename T> class Singleton {
private:
  static T *instance;
  static size_t count;

public:
  T *acquire() {
    ++count;
    if (instance == nullptr) {
      instance = new T();
    }
    return instance;
  }

  void release(T *obj) {
    if (instance == obj && --count == 0) {
      delete instance;
      instance = nullptr;
    }
  }
};

template <typename T> T *Singleton<T>::instance = nullptr;

template <typename T> size_t Singleton<T>::count = 0;

#endif
