#include "mapping.hpp"

#include <string>

#include "yaml.hpp"

namespace yamlcast {

mapping::const_iterator::const_iterator(
    const document& document,
    yaml_node_pair_t* it)
    : document_(document), it(it) {
}

bool mapping::const_iterator::operator==(const const_iterator& it) const {
  return &document_ == &it.document_ && it.it == this->it;
}

bool mapping::const_iterator::operator!=(const const_iterator& it) const {
  return !(*this == it);
}

mapping::const_iterator& mapping::const_iterator::operator ++() {
  ++it;
  return *this;
}

node mapping::const_iterator::key() const {
  return document_.get_node(it->key);
}

node mapping::const_iterator::value() const {
  return document_.get_node(it->value);
}


mapping::mapping(const document& document, const yaml_node_t* node)
    : document_(document),
      node_(node) {
}

mapping::const_iterator mapping::begin() const {
  return const_iterator(document_, node_->data.mapping.pairs.start);
}

mapping::const_iterator mapping::end() const {
  return const_iterator(document_, node_->data.mapping.pairs.top);
}

mapping::const_iterator mapping::find(const std::string& key) const {
  for (const_iterator it = begin(); it != end(); ++it) {
    node key_node = it.key();
    // Is it error?
    if (key_node.type() != YAML_SCALAR_NODE)
      continue;

    if (key_node.as_scalar() == key) {
      return it;
    }
  }
  return end();
}

std::size_t mapping::count(const std::string& key) const {
  if (find(key) == end()) {
    return 0;
  } else {
    return 1;
  }
}

}  // namespace yamlcast
