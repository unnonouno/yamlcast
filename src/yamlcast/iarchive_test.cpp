#include <map>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "exception.hpp"
#include "iarchive.hpp"

using std::map;
using std::string;
using std::vector;

namespace yamlcast {

template <typename T>
void deserialize(const string& str, T& v) {
  document::ptr yaml = document::parse_string(str);
  v = yaml_cast<T>(*yaml);
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
  int nam;
  string name;
  vector<string> tags;

  template <typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(age) & MEMBER(nam) & MEMBER(name) & MEMBER(tags);
  }
};

TEST(iarchive, object) {
  Struct s;
  deserialize("{age: 10, nam: 1, name: \"taro\", tags: [\"saitama\"]}", s);
  EXPECT_EQ(10, s.age);
  EXPECT_EQ("taro", s.name);
  ASSERT_EQ(1u, s.tags.size());
  EXPECT_EQ("saitama", s.tags[0]);
}

TEST(iarchive, not_found) {
  Struct s;
  try {
    deserialize("{age: 10, nam: 1, tags: [\"saitama\"]}", s);
    FAIL();
  } catch(const yaml_not_found& e) {
    EXPECT_EQ("name", e.get_key());
  }
}

struct OptionalStruct {
  pfi::data::optional<int> n;
  pfi::data::optional<int> m;

  template <typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(n) & MEMBER(m);
  }
};

TEST(iarchive, optional) {
  OptionalStruct s;
  s.m = 100;
  deserialize("{n: 10}", s);
  EXPECT_TRUE(s.n);
  EXPECT_EQ(10, s.n);
  EXPECT_FALSE(s.m);
}

struct Empty {
  int n;
  template <typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(n);
  }
};

TEST(iarchive, ignore_non_scalar_key) {
  Empty e;
  deserialize("{[aa]: 1, n: 10}", e);
  EXPECT_EQ(10, e.n);
}

struct Any {
  yamlcast::node::ptr arg;

  template <typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(arg);
  }
};

TEST(iarchive, node) {
  Any a;
  document::ptr yaml = document::parse_string("{arg: {x: 10, y: 20}}");
  a = yaml_cast<Any>(*yaml);
  EXPECT_EQ(YAML_MAPPING_NODE, a.arg->type());

  std::map<std::string, int> m;
  from_yaml(*a.arg, m);
  EXPECT_EQ(10, m["x"]);
  EXPECT_EQ(20, m["y"]);
}

}  // namespace yamlcast
