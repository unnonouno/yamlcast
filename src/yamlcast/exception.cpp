#include "exception.hpp"

#include <string>

#include "util.hpp"

namespace yamlcast {

namespace  {
std::string make_msg(
    const char* msg,
    const yaml_mark_t& mark) {
  std::stringstream ss;
  ss << msg << " at line:" << mark.line << " col:" << mark.column;
  return ss.str();
}

std::string make_msg(
    const char* msg,
    const class node& node) {
  std::stringstream ss;
  ss << msg
     << " at line:" << node.start_mark().line
     << " col:" << node.start_mark().column
     << " - line:" << node.end_mark().line
     << " col:" << node.end_mark().column;
  return ss.str();
}
}  // namespace

yaml_exception::yaml_exception(const std::string& msg)
    : msg_(msg) {
}

yaml_exception::~yaml_exception() throw() {
}


yaml_parse_exception::yaml_parse_exception(
    const char*& msg,
    const yaml_mark_t& mark)
    : yaml_exception(make_msg(msg, mark)),
      problem_mark_(mark) {
}


yaml_cast_exception::yaml_cast_exception(
    const class node& node,
    const std::string& msg)
    : yaml_exception(make_msg(msg.c_str(), node)),
      start_mark_(node.start_mark()),
      end_mark_(node.end_mark()) {
}


yaml_bad_cast::yaml_bad_cast(
    const node& node,
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

yaml_bad_cast::~yaml_bad_cast() throw() {
}


yaml_invalid_scalar::yaml_invalid_scalar(
    const node& node,
    const std::string& value,
    const std::type_info& type)
    : yaml_cast_exception(
          node,
          "Cannot convert \"" + value + "\" to "
          + pfi::lang::demangle(type.name())),
      value_(value),
      type_(type) {
}

yaml_invalid_scalar::~yaml_invalid_scalar() throw() {
}


yaml_not_found::yaml_not_found(
    const node& node,
    const std::string& key)
    : yaml_cast_exception(
          node,
          "\"" + key + "\" is not found"),
      key_(key) {
}

yaml_not_found::~yaml_not_found() throw() {
}

}  // namespace yamlcast

