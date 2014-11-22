#include "iarchive.hpp"

namespace yamlcast {

yaml_iarchive_cast::yaml_iarchive_cast(const yaml_document_t* yaml)
    : yaml_(yaml),
      node_(
          ::yaml_document_get_root_node(
               const_cast<yaml_document_t*>(yaml))) {
}

yaml_iarchive_cast::yaml_iarchive_cast(
    const yaml_document_t* yaml,
    const yaml_node_t* node)
    : yaml_(yaml),
      node_(node) {
}

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

#define YAMLCAST_GEN_INT_SERIALIZE(T)                    \
  template <>                                            \
  void serialize(const yaml_iarchive_cast& yaml, T& v) { \
    serialize_primitive(yaml, v);                        \
  }

YAMLCAST_GEN_INT_SERIALIZE(int8_t)
YAMLCAST_GEN_INT_SERIALIZE(int16_t)
YAMLCAST_GEN_INT_SERIALIZE(int32_t)
YAMLCAST_GEN_INT_SERIALIZE(int64_t)

YAMLCAST_GEN_INT_SERIALIZE(uint8_t)
YAMLCAST_GEN_INT_SERIALIZE(uint16_t)
YAMLCAST_GEN_INT_SERIALIZE(uint32_t)
YAMLCAST_GEN_INT_SERIALIZE(uint64_t)

template <>
void serialize(const yaml_iarchive_cast& yaml, float& v) {
  serialize_primitive(yaml, v);
}

template <>
void serialize(const yaml_iarchive_cast& yaml, double& v) {
  serialize_primitive(yaml, v);
}

template <>
void serialize(const yaml_iarchive_cast& yaml, std::string& v) {
  serialize_primitive(yaml, v);
}

template <>
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


}  // namespace yamlcast
