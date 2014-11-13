#ifndef IARCHIVE_HPP_C086E45B_D690_4375_8463_B29BD459F3FD_
#define IARCHIVE_HPP_C086E45B_D690_4375_8463_B29BD459F3FD_

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
  explicit yaml_iarchive_cast(const yaml_document_t* yaml)
      : yaml_(yaml),
        node_(
            ::yaml_document_get_root_node(
                 const_cast<yaml_document_t*>(yaml))) {
  }
  yaml_iarchive_cast(
      const yaml_document_t* yaml,
      const yaml_node_t* node) : yaml_(yaml), node_(node) {}
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

inline
void type_check(const yaml_iarchive_cast& yaml, yaml_node_type_t expect) {
  if (yaml.node().type != expect)
    throw yaml_bad_cast(yaml.node(), expect, yaml.node().type);
}

template <typename T>
void serialize_primitive(const yaml_iarchive_cast& yaml, T& v) {
  type_check(yaml, YAML_SCALAR_NODE);
  const yaml_node_t& node = yaml.node();
  std::string value(
      reinterpret_cast<const char*>(node.data.scalar.value),
      node.data.scalar.length);

  try {
    v = pfi::lang::lexical_cast<T>(value);
  } catch(std::bad_cast& e) {
    throw yaml_invalid_scalar(node, value, typeid(T));
  }
}

template <typename T>
inline
void serialize(const yaml_iarchive_cast& yaml, T& n) {
  pfi::data::serialization::access::serialize(yaml, n);
}

template <>
inline
void serialize(const yaml_iarchive_cast& yaml, bool& v) {
  type_check(yaml, YAML_SCALAR_NODE);

  const yaml_node_t& node = yaml.node();
  size_t length = node.data.scalar.length;
  const char* value = reinterpret_cast<const char*>(node.data.scalar.value);
  if (length == 4 && std::strncmp(value, "true", 4) == 0) {
    v = true;
    return;
  }
  if (length == 5 && std::strncmp(value, "false", 5) == 0) {
    v = false;
    return;
  }
  throw yaml_invalid_scalar(
      node,
      value,
      typeid(bool));  // NOLINT(readability/function)
}

template <>
inline
void serialize(const yaml_iarchive_cast& yaml, int& v) {
  serialize_primitive(yaml, v);
}

template <>
inline
void serialize(const yaml_iarchive_cast& yaml, double& v) {
  serialize_primitive(yaml, v);
}

template <>
inline
void serialize(const yaml_iarchive_cast& yaml, std::string& v) {
  serialize_primitive(yaml, v);
}

template <typename T>
inline
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
inline
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
    const std::string& key) {
  const yaml_node_t& node = yaml.node();
  for (yaml_node_pair_t* it = node.data.mapping.pairs.start;
       it < node.data.mapping.pairs.top; ++it) {
    const yaml_node_t& key_node = yaml.get_item(it->key);
    // Is it error?
    if (key_node.type != YAML_SCALAR_NODE)
      continue;

    const char* value
        = reinterpret_cast<const char*>(key_node.data.scalar.value);
    size_t length = key_node.data.scalar.length;
    if (length == key.size() && std::strncmp(key.c_str(), value, length) == 0) {
      return &yaml.get_item(it->value);
    }
  }
  return NULL;
}

template <typename T>
inline
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
inline
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
