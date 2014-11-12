#ifndef YAMLCAST__YAML_IARCHIVE_HPP_
#define YAMLCAST__YAML_IARCHIVE_HPP_

#include <map>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <pficommon/data/serialization.h>
#include <pficommon/lang/cast.h>
#include <pficommon/text/json.h>

#include "exception.hpp"
#include "util.hpp"

namespace yamlcast {

class yaml_iarchive_cast {
 public:
  explicit yaml_iarchive_cast(const YAML::Node& yaml) : yaml_(yaml) {}
  const YAML::Node& get() const { return yaml_; }

 private:
  const YAML::Node& yaml_;
};

template <typename T>
void from_yaml(const YAML::Node& yaml, T& v);

inline
void type_check(const yaml_iarchive_cast& yaml, YAML::NodeType::value expect) {
  if (yaml.get().Type() != expect)
    throw yaml_bad_cast(expect, yaml.get().Type());
}

template <typename T>
void serialize_primitive(const yaml_iarchive_cast& yaml, T& v) {
  type_check(yaml, YAML::NodeType::Scalar);
  try {
    v = yaml.get().as<T>();
  } catch(YAML::BadConversion& e) {
    const std::string& value = yaml.get().Scalar();
    throw yaml_invalid_scalar(value, typeid(T), e.mark);
  }
}

template <typename T>
inline void serialize(const yaml_iarchive_cast& yaml, T& n) {
  pfi::data::serialization::access::serialize(yaml, n);
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, bool& v) {
  serialize_primitive(yaml, v);
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, int& v) {
  serialize_primitive(yaml, v);
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, double& v) {
  serialize_primitive(yaml, v);
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, std::string& v) {
  serialize_primitive(yaml, v);
}

template <typename T>
inline void serialize(const yaml_iarchive_cast& yaml, std::vector<T>& vec) {
  type_check(yaml, YAML::NodeType::Sequence);
  std::vector<T> v(yaml.get().size());
  for (size_t i = 0; i < yaml.get().size(); ++i) {
    serialize(yaml_iarchive_cast(yaml.get()[i]), v[i]);
  }
  return v.swap(vec);
}

template <typename K, typename T>
inline void serialize(const yaml_iarchive_cast& yaml, std::map<K, T>& map) {
  type_check(yaml, YAML::NodeType::Map);
  std::map<std::string, T> m;
  for (YAML::const_iterator it = yaml.get().begin();
       it != yaml.get().end(); ++it) {
    K key;
    from_yaml(it->first, key);
    T value;
    from_yaml(it->second, value);
    m[key] = value;
  }
  m.swap(map);
}

template <typename T>
inline void serialize(
    const yaml_iarchive_cast& yaml,
    pfi::data::serialization::named_value<T>& v) {
  type_check(yaml, YAML::NodeType::Map);
  if (!yaml.get()[v.name])
    throw std::bad_cast();
  serialize(yaml_iarchive_cast(yaml.get()[v.name]), v.v);
}

template <class T>
inline const yaml_iarchive_cast &operator&(const yaml_iarchive_cast &ar, T &v) {
  serialize(ar, v);
  return ar;
}


template <typename T>
void from_yaml(const YAML::Node& yaml, T& v) {
  yaml_iarchive_cast in(yaml);
  serialize(in, v);
}

template <typename T>
T yaml_cast(const YAML::Node& yaml) {
  T v;
  from_yaml(yaml, v);
  return v;
}

}  // namespace yamlcast

#endif  // YAMLCAST__YAML_IARCHIVE_HPP_
