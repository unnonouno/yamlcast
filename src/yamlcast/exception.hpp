#ifndef YAMLCAST__YAML_EXCEPTION_HPP_
#define YAMLCAST__YAML_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

#include <yaml-cpp/yaml.h>

#include "util.hpp"

namespace yamlcast {

class yaml_bad_cast : public std::bad_cast {
 public:
  yaml_bad_cast(YAML::NodeType::value expect,
                YAML::NodeType::value actual)
      : msg_(std::string("invalid type: ")
             + type_to_string(expect) + " is expected, but is "
             + type_to_string(actual)), expect_(expect), actual_(actual) {}

  ~yaml_bad_cast() throw() {}

  const char* what() const throw() {
    return msg_.c_str();
  }

 private:
  std::string msg_;
  YAML::NodeType::value expect_;
  YAML::NodeType::value actual_;
};

}

#endif // YAMLCAST__YAML_EXCEPTION_HPP_
