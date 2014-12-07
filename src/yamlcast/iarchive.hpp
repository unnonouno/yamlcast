#ifndef IARCHIVE_HPP_C086E45B_D690_4375_8463_B29BD459F3FD_
#define IARCHIVE_HPP_C086E45B_D690_4375_8463_B29BD459F3FD_

#include <stdint.h>
#include <cstring>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include <yaml.h>

#include <pficommon/data/optional.h>
#include <pficommon/data/serialization.h>
#include <pficommon/lang/cast.h>

#include "exception.hpp"
#include "yaml.hpp"

namespace yamlcast {

class yaml_iarchive_cast {
 public:
  explicit yaml_iarchive_cast(const node& node);

  const class node& node() const { return node_; }

 private:
  const class node& node_;
};

template <typename T>
void from_yaml(const yaml_document_t& yaml, T& v);

void type_check(const yaml_iarchive_cast& yaml, yaml_node_type_t expect);

template <typename T>
void serialize(const yaml_iarchive_cast& yaml, T& n) {
  pfi::data::serialization::access::serialize(yaml, n);
}

template <>
void serialize(const yaml_iarchive_cast& yaml, bool& v);

#define YAMLCAST_GEN_SERIALIZE_DEF(T)                   \
  template <>                                           \
  void serialize(const yaml_iarchive_cast& yaml, T& v);

YAMLCAST_GEN_SERIALIZE_DEF(int8_t)
YAMLCAST_GEN_SERIALIZE_DEF(int16_t)
YAMLCAST_GEN_SERIALIZE_DEF(int32_t)
YAMLCAST_GEN_SERIALIZE_DEF(int64_t)

YAMLCAST_GEN_SERIALIZE_DEF(uint8_t)
YAMLCAST_GEN_SERIALIZE_DEF(uint16_t)
YAMLCAST_GEN_SERIALIZE_DEF(uint32_t)
YAMLCAST_GEN_SERIALIZE_DEF(uint64_t)

YAMLCAST_GEN_SERIALIZE_DEF(float)  // NOLINT(readability/casting)
YAMLCAST_GEN_SERIALIZE_DEF(double)  // NOLINT(readability/casting)
YAMLCAST_GEN_SERIALIZE_DEF(std::string)

template <typename T>
void serialize(const yaml_iarchive_cast& yaml, std::vector<T>& vec) {
  sequence seq = yaml.node().as_sequence();  // may throw exception
  std::vector<T> v;
  for (sequence::const_iterator it = seq.begin(), end = seq.end();
       it != end; ++it) {
    T x;
    serialize(yaml_iarchive_cast(*it), x);
    v.push_back(x);
  }
  return v.swap(vec);
}

template <typename K, typename T>
void serialize(const yaml_iarchive_cast& yaml, std::map<K, T>& map) {
  mapping mapping = yaml.node().as_mapping();  // may throw exception
  std::map<std::string, T> m;
  for (mapping::const_iterator it = mapping.begin(), end = mapping.end();
       it != end; ++it) {
    node key_node = it.key();
    K key;
    serialize(yaml_iarchive_cast(key_node), key);
    node value_node = it.value();
    T value;
    serialize(yaml_iarchive_cast(value_node), value);
    m.insert(std::make_pair(key, value));
  }
  m.swap(map);
}

template <typename T>
void serialize(
    const yaml_iarchive_cast& yaml,
    pfi::data::serialization::named_value<pfi::data::optional<T> >& v) {
  mapping map = yaml.node().as_mapping();  // may throw an exception
  mapping::const_iterator node = map.find(v.name);
  if (node == map.end()) {
    v.v = pfi::data::optional<T>();
  } else {
    T value = T();
    serialize(yaml_iarchive_cast(node.value()), value);
    v.v = value;
  }
}

template <typename T>
void serialize(
    const yaml_iarchive_cast& yaml,
    pfi::data::serialization::named_value<T>& v) {
  mapping map = yaml.node().as_mapping();  // may throw an exception
  mapping::const_iterator node = map.find(v.name);
  if (node == map.end()) {
    throw yaml_not_found(yaml.node(), v.name);
  }
  serialize(yaml_iarchive_cast(node.value()), v.v);
}

template <>
void serialize(const yaml_iarchive_cast& yaml, node::ptr& n);


template <typename T>
void from_yaml(const node& yaml, T& v) {
  yaml_iarchive_cast in(yaml);
  serialize(in, v);
}

template <class T>
T yaml_cast(const document& yaml) {
  T value = T();
  from_yaml(yaml.get_root(), value);
  return value;
}

}  // namespace yamlcast

#endif  // IARCHIVE_HPP_C086E45B_D690_4375_8463_B29BD459F3FD_
