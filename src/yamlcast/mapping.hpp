#ifndef MAPPING_HPP_FC89B1D4_1FDC_4CF0_A534_D0D80EF9212D_
#define MAPPING_HPP_FC89B1D4_1FDC_4CF0_A534_D0D80EF9212D_

#include <string>
#include <yaml.h>

namespace yamlcast {

class document;
class node;

class mapping {
 public:
  class const_iterator {
   public:
    const_iterator(
        const document& document,
        yaml_node_pair_t* it);

    bool operator==(const const_iterator& it) const;

    bool operator!=(const const_iterator& it) const;

    const_iterator& operator ++();

    node key() const;

    node value() const;

   private:
    const document& document_;
    yaml_node_pair_t* it;
  };

  mapping(const document& document, const yaml_node_t* node);

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator find(const std::string& key) const;

  std::size_t count(const std::string& key) const;

 private:
  const document& document_;
  const yaml_node_t* node_;
};

}  // namespace yamlcast

#endif  // MAPPING_HPP_FC89B1D4_1FDC_4CF0_A534_D0D80EF9212D_
