#include "iarchive.hpp"

#include <string>

namespace yamlcast {

yaml_iarchive_cast::yaml_iarchive_cast(
    const class node& node)
    : node_(node) {
}

template <typename T>
void serialize_primitive(const yaml_iarchive_cast& yaml, T& v) {
  scalar s = yaml.node().as_scalar();
  std::string value = s.str();

  try {
    v = pfi::lang::lexical_cast<T>(value);
  } catch(std::bad_cast& e) {
    throw yaml_invalid_scalar(yaml.node(), value, typeid(T));
  }
}

#define YAMLCAST_GEN_SERIALIZE(T)                        \
  template <>                                            \
  void serialize(const yaml_iarchive_cast& yaml, T& v) { \
    serialize_primitive(yaml, v);                        \
  }

YAMLCAST_GEN_SERIALIZE(int8_t)
YAMLCAST_GEN_SERIALIZE(int16_t)
YAMLCAST_GEN_SERIALIZE(int32_t)
YAMLCAST_GEN_SERIALIZE(int64_t)

YAMLCAST_GEN_SERIALIZE(uint8_t)
YAMLCAST_GEN_SERIALIZE(uint16_t)
YAMLCAST_GEN_SERIALIZE(uint32_t)
YAMLCAST_GEN_SERIALIZE(uint64_t)

YAMLCAST_GEN_SERIALIZE(float)  // NOLINT(readability/casting)
YAMLCAST_GEN_SERIALIZE(double)  // NOLINT(readability/casting)
YAMLCAST_GEN_SERIALIZE(std::string)

template <>
void serialize(const yaml_iarchive_cast& yaml, bool& v) {
  scalar s = yaml.node().as_scalar();
  if (s == "true") {
    v = true;
    return;
  }
  if (s == "false") {
    v = false;
    return;
  }
  throw yaml_invalid_scalar(
      yaml.node(),
      s.str(),
      typeid(bool));  // NOLINT(readability/function)
}

template <>
void serialize(const yaml_iarchive_cast& yaml, node::ptr& n) {
  n.reset(new node(yaml.node()));
}

}  // namespace yamlcast
