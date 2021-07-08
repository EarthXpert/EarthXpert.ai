/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/stl/bitvector.h"

//#include "algo.hpp"
#include "regex.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace earthxpert;

void CreateFiles(const std::string& path, int numFiles);

TEST(stl, BitVector) {
  BitVector bv;

  bv.add(true).add(true).add(false); // [1, 1, 0]
  bv.add(false); // [1, 1, 0, 0]
  bv.add(true); // [1, 1, 0, 0, 1]

  ASSERT_TRUE(bv.any());
  ASSERT_FALSE(bv.all());
  ASSERT_FALSE(bv.none());
}

// Too Many Open Files
//TEST(stl, OpenFiles) {
//  const std::string path = "D:\\Workspace\\Results\\temp\\test\\";
//  CreateFiles(path, 5000);
//}

void CreateFiles(const std::string& path, int numFiles) {

}

class Foo {
public:
  Foo() = default;
  Foo(const Foo& rhs) {
    mn = rhs.mn;
    ms = rhs.ms;
    mv = rhs.mv;
    mm = rhs.mm;
  }
  //Foo(Foo&& rhs) noexcept = default;
  Foo(Foo&& rhs) noexcept {
    mn = rhs.mn;
    ms = std::move(rhs.ms);
    mv = std::move(rhs.mv);
    mm = std::move(rhs.mm);

    rhs.Init();
  }

  ~Foo() {
    mn = 0;
  }

  void SetInt(const int n) { mn = n; }
  void SetStr(const std::string& str) { ms = str; }
  void SetVec(const std::vector<float>& v) { mv = v; }
  void SetMap(const std::map<int, std::string>& m) { mm = m; }

  std::string GetStr() const { return ms; }

private:
  int mn{};
  std::string ms;
  std::vector<float> mv;
  std::map<int, std::string> mm;

  // CopyFrom()
  // MoveFrom()
  void Init() {
    mn = 0;
  }
};

TEST(stl, move_ctr) {
  int n = 10;
  std::string str("abc");
  std::vector<float> vec{1, 2, 3};
  std::map<int, std::string> mis{ {1, "a"}, {2, "ab"}, {3, "abc"} };

  Foo foo;
  foo.SetInt(n);
  foo.SetStr(str);
  foo.SetVec(vec);
  foo.SetMap(mis);

  Foo foo2(std::move(foo));

  std::string str2 = std::move(str);
  std::vector<float> vec2(std::move(vec));
  std::map<int, std::string> mis2 = std::move(mis);

  std::cout << foo2.GetStr() << std::endl;
}
