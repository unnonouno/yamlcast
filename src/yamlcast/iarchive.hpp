#ifndef YAMLCAST__YAML_IARCHIVE_HPP_
#define YAMLCAST__YAML_IARCHIVE_HPP_

#include <yaml-cpp/yaml.h>

#include <pficommon/lang/cast.h>
#include <pficommon/text/json.h>
#include <pficommon/data/serialization.h>

#include "util.hpp"
#include "exception.hpp"

namespace yamlcast {

class yaml_iarchive_cast {
 public:
  yaml_iarchive_cast(const YAML::Node& yaml) : yaml_(yaml) {}
  const YAML::Node& get() const { return yaml_; }

 private:
  const YAML::Node& yaml_;
};

inline
void type_check(const yaml_iarchive_cast& yaml, YAML::NodeType::value expect) {
  if (yaml.get().Type() != expect)
    throw yaml_bad_cast(expect, yaml.get().Type());
}

template <typename T>
inline void serialize(const yaml_iarchive_cast& yaml, T& n) {
  pfi::data::serialization::access::serialize(yaml, n);
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, bool& v) {
  type_check(yaml, YAML::NodeType::Scalar);
  yaml.get() >> v;
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, int& n) {
  type_check(yaml, YAML::NodeType::Scalar);
  yaml.get() >> n;
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, double& v) {
  type_check(yaml, YAML::NodeType::Scalar);
  yaml.get() >> v;
}

template <>
inline void serialize(const yaml_iarchive_cast& yaml, std::string& v) {
  type_check(yaml, YAML::NodeType::Scalar);
  yaml.get() >> v;
}

template <typename T>
inline void serialize(const yaml_iarchive_cast& yaml, std::vector<T>& vec) {
  type_check(yaml, YAML::NodeType::Sequence);
  std::vector<T> v(yaml.get().size());
  for (size_t i = 0; i < yaml.get().size(); ++i) {
    serialize(yaml.get()[i], v[i]);
  }
  return v.swap(vec);
}

template <typename K, typename T>
inline void serialize(const yaml_iarchive_cast& yaml, std::map<K, T>& map) {
  type_check(yaml, YAML::NodeType::Map);
  std::map<std::string, T> m;
  for (YAML::Iterator it = yaml.get().begin();
       it != yaml.get().end(); ++it) {
    K key;
    from_yaml(it.first(), key);
    T value;
    from_yaml(it.second(), value);
    m[key] = value;
  }
  m.swap(map);
}

template <typename T>
inline void serialize(const yaml_iarchive_cast& yaml, pfi::data::serialization::named_value<T>& v) {
  type_check(yaml, YAML::NodeType::Map);
  if (!yaml.get().FindValue(v.name))
    throw std::bad_cast();
  serialize(yaml.get()[v.name], v.v);
}

template <class T>
inline const yaml_iarchive_cast &operator&(const yaml_iarchive_cast &ar, T &v)
{
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

}

#endif // YAMLCAST__YAML_IARCHIVE_HPP_
