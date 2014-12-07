#ifndef SCALAR_HPP_D5F034E4_2660_4F67_836C_ED10AF686717_
#define SCALAR_HPP_D5F034E4_2660_4F67_836C_ED10AF686717_

#include <string>
#include <yaml.h>

namespace yamlcast {

class document;

class scalar {
 public:
  explicit scalar(const yaml_node_t* node);

  bool operator ==(const char* str) const;

  bool operator ==(const std::string& str) const;

  size_t length() const;

  const char* value() const;

  std::string str() const;

 private:
  const yaml_node_t* node_;
};

}  // namespace yamlcast

#endif  // SCALAR_HPP_D5F034E4_2660_4F67_836C_ED10AF686717_
