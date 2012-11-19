#ifndef YAMLCAST__UTIL_HPP_
#define YAMLCAST__UTIL_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

namespace yamlcast {

inline
const char* type_to_string(YAML::NodeType::value t) {
  switch(t) {
    case YAML::NodeType::Null:
      return "null";
    case YAML::NodeType::Scalar:
      return "scalar";
    case YAML::NodeType::Sequence:
      return "sequence";
    case YAML::NodeType::Map:
      return "map";
    default:
      return "no type";
  }
}

}

#endif // YAMLCAST__UTIL_HPP_
