#ifndef EXCEPTION_HPP_FE226B93_4BA9_47DD_A8EB_68D8C1EEAD11_
#define EXCEPTION_HPP_FE226B93_4BA9_47DD_A8EB_68D8C1EEAD11_

#include <stdexcept>
#include <string>
#include <sstream>
#include <typeinfo>

#include <yaml.h>

#include <pficommon/lang/demangle.h>

#include "util.hpp"

namespace yamlcast {

std::string make_msg(
    const char* msg,
    const yaml_node_t& node) {
  std::stringstream ss;
  ss << msg
     << " at line:" << node.start_mark.line
     << " col:" << node.start_mark.column
     << " - line:" << node.end_mark.line
     << " col:" << node.end_mark.column;
  return ss.str();
}

class yaml_cast_exception : public std::exception {
 public:
  explicit yaml_cast_exception(
      const yaml_node_t& node,
      const std::string& msg)
      : msg_(make_msg(msg.c_str(), node)),
        start_mark_(node.start_mark),
        end_mark_(node.end_mark) {
  }

  virtual ~yaml_cast_exception() throw() {}

  const char* what() const throw() {
    return msg_.c_str();
  }

 private:
  const std::string msg_;
  yaml_mark_t start_mark_;
  yaml_mark_t end_mark_;
};

class yaml_bad_cast : public yaml_cast_exception {
 public:
  yaml_bad_cast(
      const yaml_node_t& node,
      yaml_node_type_t expect,
      yaml_node_type_t actual)
      : yaml_cast_exception(
            node,
            std::string("invalid type: ")
            + type_to_string(expect)
            + " is expected, but is "
            + type_to_string(actual)),
        expect_(expect),
        actual_(actual) {
  }

  ~yaml_bad_cast() throw() {}

  yaml_node_type_t get_expect() const {
    return expect_;
  }

  yaml_node_type_t get_actual() const {
    return actual_;
  }

 private:
  yaml_node_type_t expect_;
  yaml_node_type_t actual_;
};

class yaml_invalid_scalar : public yaml_cast_exception {
 public:
  yaml_invalid_scalar(
      const yaml_node_t& node,
      const std::string& value,
      const std::type_info& type)
      : yaml_cast_exception(
            node,
            "Cannot convert \"" + value + "\" to "
            + pfi::lang::demangle(type.name())),
        value_(value),
        type_(type) {
  }

  ~yaml_invalid_scalar() throw() {}

 private:
  std::string value_;
  const std::type_info& type_;
};

class yaml_not_found : public yaml_cast_exception {
 public:
  yaml_not_found(
      const yaml_node_t& node,
      const std::string& key)
      : yaml_cast_exception(
            node,
            "\"" + key + "\" is not found"),
        key_(key) {
  }

  ~yaml_not_found() throw() {}

 private:
  const std::string key_;
};

}  // namespace yamlcast

#endif  // EXCEPTION_HPP_FE226B93_4BA9_47DD_A8EB_68D8C1EEAD11_
