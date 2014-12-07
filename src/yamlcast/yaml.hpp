#ifndef YAML_HPP_7BFAED50_664E_4774_AFB6_92400FBDC50E_
#define YAML_HPP_7BFAED50_664E_4774_AFB6_92400FBDC50E_

#include <string>
#include <yaml.h>

#include <pficommon/lang/shared_ptr.h>

#include "exception.hpp"
#include "mapping.hpp"
#include "scalar.hpp"
#include "sequence.hpp"

namespace yamlcast {

class node;

class document {
 public:
  typedef pfi::lang::shared_ptr<document> ptr;

  ~document();

  static ptr parse_string(const std::string& str);
  static ptr parse_file(const std::string& path);
  static ptr parse_file(FILE* file);

  node get_root() const;

  node get_node(int key) const;

 private:
  explicit document(const yaml_document_t& document);
  explicit document(const document&);

  yaml_document_t document_;
};

class node {
 public:
  typedef pfi::lang::shared_ptr<node> ptr;

  node(const document& document, const yaml_node_t* node);

  const document& get_document() const;

  yaml_node_type_t type() const;

  scalar as_scalar() const;

  sequence as_sequence() const;

  mapping as_mapping() const;

  const yaml_mark_t& start_mark() const;

  const yaml_mark_t& end_mark() const;

 private:
  void type_check(yaml_node_type_t expect) const;

  const document* document_;
  const yaml_node_t* node_;
};

}  // namespace yamlcast

#endif  // YAML_HPP_7BFAED50_664E_4774_AFB6_92400FBDC50E_
