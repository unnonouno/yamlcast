#ifndef EXCEPTION_HPP_FE226B93_4BA9_47DD_A8EB_68D8C1EEAD11_
#define EXCEPTION_HPP_FE226B93_4BA9_47DD_A8EB_68D8C1EEAD11_

#include <stdexcept>
#include <string>
#include <sstream>
#include <typeinfo>

#include <yaml.h>

#include <pficommon/lang/demangle.h>

#include "yaml.hpp"

namespace yamlcast {

class yaml_exception : public std::exception {
 public:
  explicit yaml_exception(const std::string& msg);

  virtual ~yaml_exception() throw();

  const char* what() const throw() {
    return msg_.c_str();
  }

 private:
  const std::string msg_;
};

class yaml_parse_exception : public yaml_exception {
 public:
  yaml_parse_exception(
      const char*& msg,
      const yaml_mark_t& mark);

  const yaml_mark_t& get_problem_mark() const {
    return problem_mark_;
  }

 private:
  const yaml_mark_t problem_mark_;
};

class yaml_cast_exception : public yaml_exception {
 public:
  explicit yaml_cast_exception(
      const class node& node,
      const std::string& msg);

  const yaml_mark_t& get_start_mark() const {
    return start_mark_;
  }

  const yaml_mark_t& get_end_mark() const {
    return end_mark_;
  }

 private:
  const yaml_mark_t start_mark_;
  const yaml_mark_t end_mark_;
};

class yaml_bad_cast : public yaml_cast_exception {
 public:
  yaml_bad_cast(
      const node& node,
      yaml_node_type_t expect,
      yaml_node_type_t actual);

  ~yaml_bad_cast() throw();

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
      const node& node,
      const std::string& value,
      const std::type_info& type);

  ~yaml_invalid_scalar() throw();

  const std::type_info& get_type() const {
    return type_;
  }

 private:
  std::string value_;
  const std::type_info& type_;
};

class yaml_not_found : public yaml_cast_exception {
 public:
  yaml_not_found(
      const node& node,
      const std::string& key);

  ~yaml_not_found() throw();

  const std::string& get_key() const {
    return key_;
  }

 private:
  const std::string key_;
};

}  // namespace yamlcast

#endif  // EXCEPTION_HPP_FE226B93_4BA9_47DD_A8EB_68D8C1EEAD11_
