#ifndef SEQUENCE_HPP_DF9686DF_C543_4B61_97BD_A25B29963615_
#define SEQUENCE_HPP_DF9686DF_C543_4B61_97BD_A25B29963615_

#include <string>
#include <yaml.h>

namespace yamlcast {

class document;
class node;

class sequence {
 public:
  class const_iterator {
   public:
    const_iterator(
        const document& document,
        yaml_node_item_t* it);

    const_iterator& operator ++();

    bool operator==(const const_iterator& it) const;

    bool operator!=(const const_iterator& it) const;

    node operator*() const;

   private:
    const document& document_;
    yaml_node_item_t* it;
  };

  sequence(const document& document, const yaml_node_t* node);

  const_iterator begin() const;

  const_iterator end() const;

  std::size_t size() const;

  node operator[](const std::size_t i) const;

 private:
  const document& document_;
  const yaml_node_t* node_;
};

}  // namespace yamlcast

#endif  // SEQUENCE_HPP_DF9686DF_C543_4B61_97BD_A25B29963615_
