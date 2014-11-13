#ifndef UTIL_HPP_3B01CE0E_0F6F_4219_AD4E_5199E209C097_
#define UTIL_HPP_3B01CE0E_0F6F_4219_AD4E_5199E209C097_

#include <yaml.h>

namespace yamlcast {

inline
const char* type_to_string(yaml_node_type_t t) {
  switch (t) {
    case YAML_NO_NODE:
      return "null";
    case YAML_SCALAR_NODE:
      return "scalar";
    case YAML_SEQUENCE_NODE:
      return "sequence";
    case YAML_MAPPING_NODE:
      return "map";
    default:
      return "no type";
  }
}

}  // namespace yamlcast

#endif  // UTIL_HPP_3B01CE0E_0F6F_4219_AD4E_5199E209C097_
