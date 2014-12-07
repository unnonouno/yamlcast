#include "yaml.hpp"

#include <errno.h>
#include <string.h>
#include <string>

namespace yamlcast {
namespace {

class safe_parser {
 public:
  safe_parser() {
    if (!::yaml_parser_initialize(&parser)) {
      throw yaml_exception("fail to initialize a parser");
    }
  }

  ~safe_parser() {
    ::yaml_parser_delete(&parser);
  }

  void set_input_string(const std::string& str) {
    ::yaml_parser_set_input_string(
         &parser,
         reinterpret_cast<const unsigned char*>(str.c_str()),
         str.size());
  }

  void set_input_file(FILE* file) {
    ::yaml_parser_set_input_file(&parser, file);
  }

  void set_encoding(yaml_encoding_t encoding) {
    ::yaml_parser_set_encoding(&parser, encoding);
  }

  void load(yaml_document_t& doc) {
    if (!::yaml_parser_load(&parser, &doc)) {
      throw yaml_parse_exception(parser.problem, parser.problem_mark);
    }
  }

 private:
  yaml_parser_t parser;
};

}  // namespace

void node::type_check(yaml_node_type_t expect) const {
  if (type() != expect) {
    throw yaml_bad_cast(*this, expect, type());
  }
}


document::document(const yaml_document_t& doc)
    : document_(doc) {
}

document::~document() {
  ::yaml_document_delete(&document_);
}

document::ptr document::parse_string(const std::string& str) {
  yaml_document_t doc;
  {
    safe_parser parser;
    parser.set_input_string(str);
    parser.load(doc);
  }
  return document::ptr(new document(doc));
}

document::ptr document::parse_file(const std::string& path) {
  FILE* file = ::fopen(path.c_str(), "rb");
  if (!file) {
    throw yaml_exception(::strerror(errno));
  }
  try {
    document::ptr p = parse_file(file);
    ::fclose(file);
    return p;
  } catch(...) {
    ::fclose(file);
    throw;
  }
}

document::ptr document::parse_file(FILE* file) {
  yaml_document_t doc;
  {
    safe_parser parser;
    parser.set_input_file(file);
    parser.load(doc);
  }
  return document::ptr(new document(doc));
}

node document::get_root() const {
  yaml_node_t* n = ::yaml_document_get_root_node(
      const_cast<yaml_document_t*>(&document_));
  if (!n) {}

  return node(*this, n);
}

node document::get_node(int key) const {
  yaml_node_t* n = document_.nodes.start + key - 1;
  return node(*this, n);
}


node::node(const document& document, const yaml_node_t* node)
    : document_(&document),
      node_(node) {
}

const document& node::get_document() const {
  return *document_;
}

yaml_node_type_t node::type() const {
  return node_->type;
}

scalar node::as_scalar() const {
  type_check(YAML_SCALAR_NODE);
  return scalar(node_);
}

sequence node::as_sequence() const {
  type_check(YAML_SEQUENCE_NODE);
  return sequence(*document_, node_);
}

mapping node::as_mapping() const {
  type_check(YAML_MAPPING_NODE);
  return mapping(*document_, node_);
}

const yaml_mark_t& node::start_mark() const {
  return node_->start_mark;
}

const yaml_mark_t& node::end_mark() const {
  return node_->end_mark;
}

}  // namespace yamlcast
