#include "sequence.hpp"

#include "yaml.hpp"

namespace yamlcast {

sequence::const_iterator::const_iterator(
    const document& document,
    yaml_node_item_t* it)
    : document_(document), it(it) {}

sequence::const_iterator& sequence::const_iterator::operator ++() {
  ++it;
  return *this;
}

bool sequence::const_iterator::operator==(const const_iterator& it) const {
  return &document_ == &it.document_ && it.it == this->it;
}

bool sequence::const_iterator::operator!=(const const_iterator& it) const {
  return !(*this == it);
}

node sequence::const_iterator::operator*() const {
  return document_.get_node(*it);
}


sequence::sequence(const document& document, const yaml_node_t* node)
    : document_(document),
      node_(node) {
}

sequence::const_iterator sequence::begin() const {
  return const_iterator(document_, node_->data.sequence.items.start);
}

sequence::const_iterator sequence::end() const {
  return const_iterator(document_, node_->data.sequence.items.top);
}

std::size_t sequence::size() const {
  return node_->data.sequence.items.top - node_->data.sequence.items.start;
}

node sequence::operator[](const std::size_t i) const {
  return document_.get_node(node_->data.sequence.items.start[i]);
}

}  // namespace yamlcast
