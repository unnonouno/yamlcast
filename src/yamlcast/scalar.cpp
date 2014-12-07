#include "scalar.hpp"

#include <cstring>
#include <string>

#include "yaml.hpp"

namespace yamlcast {

scalar::scalar(const yaml_node_t* node)
    : node_(node) {
}

bool scalar::operator ==(const char* str) const {
  return std::strlen(str) == length()
      && std::strncmp(value(), str, length()) == 0;
}

bool scalar::operator ==(const std::string& str) const {
  return str.size() == length()
    && std::strncmp(value(), str.c_str(), length()) == 0;
}

size_t scalar::length() const {
  return node_->data.scalar.length;
}

const char* scalar::value() const {
  return reinterpret_cast<const char*>(node_->data.scalar.value);
}

std::string scalar::str() const {
  return std::string(value(), length());
}

}  // namespace yamlcast
