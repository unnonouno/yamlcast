#include <gtest/gtest.h>
#include "iarchive.hpp"

#include <string>
#include <vector>

using namespace std;

namespace yamlcast {

template <typename T>
void deserialize(const string& str, T& v) {
  YAML::Node yaml;
  stringstream ss(str);
  YAML::Parser p(ss);
  p.GetNextDocument(yaml);
  from_yaml(yaml, v);
}

template <typename T>
void test_eq(const string& str, const T& expect) {
  T v;
  deserialize(str, v);
  EXPECT_EQ(expect, v);
}

TEST(iarchive, bool) {
  test_eq<bool>("true", true);
  test_eq<bool>("false", false);
}

TEST(iarchive, invalid_bool) {
  bool v;
  EXPECT_THROW(deserialize("[]", v), yaml_bad_cast);
  EXPECT_THROW(deserialize("10", v), yaml_invalid_scalar);
}

TEST(iarchive, int) {
  test_eq<int>("10", 10);
}

TEST(iarchive, invalid_int) {
  int v;
  EXPECT_THROW(deserialize("[]", v), yaml_bad_cast);
  EXPECT_THROW(deserialize("true", v), yaml_invalid_scalar);
}

TEST(iarchive, double) {
  test_eq<double>("1.5", 1.5);
}

TEST(iarchive, invalid_double) {
  double v;
  EXPECT_THROW(deserialize("{}", v), yaml_bad_cast);
  EXPECT_THROW(deserialize("true", v), yaml_invalid_scalar);
}

TEST(iarchive, string) {
  test_eq<string>("\"hello\"", "hello");
}

TEST(iarchive, invalid_string) {
  double v;
  EXPECT_THROW(deserialize("{}", v), yaml_bad_cast);
}

TEST(iarchive, vector) {
  vector<int> v;
  v.push_back(0);
  v.push_back(1);
  
  test_eq<vector<int> >("[0, 1]", v);
}

TEST(iarchive, map) {
  map<string, int> m;
  m["hello"] = 1;
  m["goodbye"] = 2;

  test_eq<map<string, int> >("{hello: 1, goodbye: 2}", m);
}

struct Struct {
  int age;
  string name;
  vector<string> tags;

  template <typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(age) & MEMBER(name) & MEMBER(tags);
  }
};

TEST(iarchive, object) {
  Struct s;
  deserialize("{age: 10, name: \"taro\", tags: [\"saitama\"]}", s);
  EXPECT_EQ(10, s.age);
  EXPECT_EQ("taro", s.name);
  ASSERT_EQ(1u, s.tags.size());
  EXPECT_EQ("saitama", s.tags[0]);
}


}
