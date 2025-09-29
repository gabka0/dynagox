#include "singleton.h"
#include <catch2/catch_test_macros.hpp>

class SingletonTestClass {
public:
  unsigned long value;
};

TEST_CASE("Singleton: should return an instance on acquire") {
  Singleton<SingletonTestClass> singleton;
  auto instance = singleton.acquire();

  REQUIRE(instance != nullptr);
  singleton.release(instance);
}

TEST_CASE("Singleton: should return the same instance on multiple acquire") {
  Singleton<SingletonTestClass> singleton;
  auto instance1 = singleton.acquire();
  auto instance2 = singleton.acquire();

  REQUIRE(instance1 == instance2);
  singleton.release(instance1);
  singleton.release(instance2);
}

TEST_CASE("Singleton: should create a new instance after full release") {
  Singleton<SingletonTestClass> singleton;
  auto instance1 = singleton.acquire();
  instance1->value = 1;
  singleton.release(instance1);

  auto instance2 = singleton.acquire();
  REQUIRE(instance2 != nullptr);
  REQUIRE(instance2->value == 0);
  singleton.release(instance2);
}

TEST_CASE("Singleton: should retain the instance on partial release") {
  Singleton<SingletonTestClass> singleton;
  auto instance1 = singleton.acquire();
  auto instance2 = singleton.acquire();

  singleton.release(instance1);
  auto instance3 = singleton.acquire();

  REQUIRE(instance3 == instance2);
  singleton.release(instance2);
  singleton.release(instance3);
}
