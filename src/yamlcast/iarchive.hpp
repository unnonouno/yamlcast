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

namespace yamlcast {

class yaml_iarchive_cast {
 public:
  explicit yaml_iarchive_cast(const yaml_document_t* yaml);

  yaml_iarchive_cast(const yaml_document_t* yaml, const yaml_node_t* node);

  const yaml_document_t& get() const { return *yaml_; }
  const yaml_node_t& node() const { return *node_; }
  const yaml_node_t& get_item(int key) const {
    return *(yaml_->nodes.start + key - 1);
  }

 private:
  const yaml_document_t* yaml_;
  const yaml_node_t* node_;
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

YAMLCAST_GEN_SERIALIZE_DEF(float)
YAMLCAST_GEN_SERIALIZE_DEF(double)
YAMLCAST_GEN_SERIALIZE_DEF(std::string)

template <typename T>
void serialize(const yaml_iarchive_cast& yaml, std::vector<T>& vec) {
  type_check(yaml, YAML_SEQUENCE_NODE);
  const yaml_node_t& node = yaml.node();
  std::vector<T> v;
  for (yaml_node_item_t* it = node.data.sequence.items.start;
       it < node.data.sequence.items.top; ++it) {
    T x;
    const yaml_node_t& child = yaml.get_item(*it);
    serialize(yaml_iarchive_cast(&yaml.get(), &child), x);
    v.push_back(x);
  }
  return v.swap(vec);
}

template <typename K, typename T>
void serialize(const yaml_iarchive_cast& yaml, std::map<K, T>& map) {
  type_check(yaml, YAML_MAPPING_NODE);
  const yaml_node_t& node = yaml.node();
  std::map<std::string, T> m;
  for (yaml_node_pair_t* it = node.data.mapping.pairs.start;
       it < node.data.mapping.pairs.top; ++it) {
    const yaml_node_t& key_node = yaml.get_item(it->key);
    K key;
    serialize(yaml_iarchive_cast(&yaml.get(), &key_node), key);
    const yaml_node_t& value_node = yaml.get_item(it->value);
    T value;
    serialize(yaml_iarchive_cast(&yaml.get(), &value_node), value);
    m[key] = value;
  }
  m.swap(map);
}

const yaml_node_t* find(
    const yaml_iarchive_cast& yaml,
    const std::string& key);

template <typename T>
void serialize(
    const yaml_iarchive_cast& yaml,
    pfi::data::serialization::named_value<pfi::data::optional<T> >& v) {
  type_check(yaml, YAML_MAPPING_NODE);
  const yaml_node_t* node = find(yaml, v.name);
  if (node) {
    T value = T();
    serialize(yaml_iarchive_cast(&yaml.get(), node), value);
    v.v = value;
  } else {
    v.v = pfi::data::optional<T>();
  }
}

template <typename T>
void serialize(
    const yaml_iarchive_cast& yaml,
    pfi::data::serialization::named_value<T>& v) {
  type_check(yaml, YAML_MAPPING_NODE);
  const yaml_node_t* node = find(yaml, v.name);
  if (!node) {
    throw yaml_not_found(yaml.node(), v.name);
  }
  serialize(yaml_iarchive_cast(&yaml.get(), node), v.v);
}


template <typename T>
void from_yaml(const yaml_document_t& yaml, T& v) {
  yaml_iarchive_cast in(&yaml);
  serialize(in, v);
}

template <class T>
T yaml_cast(const yaml_document_t& yaml) {
  T value = T();
  from_yaml(yaml, value);
  return value;
}

}  // namespace yamlcast

#endif  // IARCHIVE_HPP_C086E45B_D690_4375_8463_B29BD459F3FD_
