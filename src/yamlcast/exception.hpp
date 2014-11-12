#ifndef YAMLCAST__YAML_EXCEPTION_HPP_
#define YAMLCAST__YAML_EXCEPTION_HPP_

#include <stdexcept>
#include <string>
#include <typeinfo>

#include <pficommon/lang/demangle.h>

#include <yaml-cpp/yaml.h>

#include "util.hpp"

namespace yamlcast {

std::string make_msg(const char* msg,
                     const YAML::Mark& mark) {
  std::stringstream ss;
  ss << msg
     << " at line " << mark.line + 1
     << ", column " << mark.column + 1;
  return ss.str();
}

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

  const YAML::NodeType::value& get_expect() const {
    return expect_;
  }

  const YAML::NodeType::value& get_actual() const {
    return actual_;
  }

 private:
  std::string msg_;
  YAML::NodeType::value expect_;
  YAML::NodeType::value actual_;
};

class yaml_invalid_scalar : public std::bad_cast {
 public:
  yaml_invalid_scalar(const std::string& value,
                      const std::type_info& type,
                      const YAML::Mark& mark)
      : msg_(make_msg("invalid scalar", mark)
             + ": "
             + "Cannot convert \"" + value + "\" to "
             + pfi::lang::demangle(type.name())),
        value_(value),
        type_(type),
        mark_(mark) {}

  ~yaml_invalid_scalar() throw() {}

  const char* what() const throw() {
    return msg_.c_str();
  }

  const std::string msg_;
  const std::string& value_;
  const std::type_info& type_;
  YAML::Mark mark_;
};

}  // namespace yamlcast

#endif  // YAMLCAST__YAML_EXCEPTION_HPP_
