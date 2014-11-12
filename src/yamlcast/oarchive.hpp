#ifndef YAMLCAST__YAML_OARCHIVE_HPP_
#define YAMLCAST__YAML_OARCHIVE_HPP_

#include <map>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

//#include <pficommon/data/serialization.h>

namespace yamlcast {

class yaml_oarchive_cast {
 public:
  explicit yaml_oarchive_cast(YAML::Emitter& yaml) : yaml_(yaml) {}
  YAML::Emitter& get() const { return yaml_; }

 private:
  YAML::Emitter& yaml_;
};

template <typename T>
inline void serialize(const yaml_oarchive_cast& yaml, T& n) {
  yaml.get() << YAML::BeginMap;
  pfi::data::serialization::access::serialize(yaml, n);
  yaml.get() << YAML::EndMap;
}

template <>
inline void serialize(const yaml_oarchive_cast& yaml, int& n) {
  yaml.get() << n;
}

template <>
inline void serialize(const yaml_oarchive_cast& yaml, double& v) {
  yaml.get() << v;
}

template <>
inline void serialize(const yaml_oarchive_cast& yaml, std::string& v) {
  yaml.get() << v;
}

template <typename T>
inline void serialize(const yaml_oarchive_cast& yaml, std::vector<T>& vec) {
  yaml.get() << YAML::BeginSeq;
  for (size_t i = 0; i < vec.size(); ++i) {
    serialize(yaml, vec[i]);
  }
  yaml.get() << YAML::EndSeq;
}

template <typename T>
inline
void serialize(const yaml_oarchive_cast& yaml, std::map<std::string, T>& map) {
  yaml.get() << YAML::BeginMap;
  for (typename std::map<std::string, T>::const_iterator it = map.begin();
       it != map.end(); ++it) {
    yaml.get() << YAML::Key << it->first;
    yaml.get() << YAML::Value;
    serialize(yaml, it->second);
  }
  yaml.get() << YAML::EndMap;
}

template <typename T>
inline
void serialize(
    const yaml_oarchive_cast& yaml,
    pfi::data::serialization::named_value<T>& v) {
  yaml.get() << YAML::Key << v.name;
  yaml.get() << YAML::Value << v.v;
}

template <class T>
inline const yaml_oarchive_cast &operator&(const yaml_oarchive_cast &ar, T &v) {
  serialize(ar, v);
  return ar;
}


template <typename T>
void to_yaml(YAML::Emitter& yaml, const T& v) {
  yaml_oarchive_cast out(yaml);
  serialize(out, const_cast<T&>(v));
}

template <typename T>
std::string to_yaml(const T& v) {
  YAML::Emitter yaml;
  to_yaml(yaml, v);
  return yaml.c_str();
}

}  // namespace yamlcast

#endif  // YAMLCAST__OARCHIVE_HPP_
