#include <stdio.h>
#include <string>

#include <gtest/gtest.h>

#include "yaml.hpp"

namespace yamlcast {

TEST(document, scalar) {
  document::ptr d = document::parse_string("saitama");
  node n = d->get_root();
  EXPECT_EQ(YAML_SCALAR_NODE, n.type());
  scalar s = n.as_scalar();
  EXPECT_EQ(7u, s.length());
  EXPECT_TRUE(s == "saitama");  // NOLINT(readability/check)
  EXPECT_TRUE(s == std::string("saitama"));  // NOLINT(readability/check)
  EXPECT_EQ("saitama", s.str());
}

TEST(document, sequence) {
  document::ptr d = document::parse_string("[saitama, gumma]");
  node n = d->get_root();
  EXPECT_EQ(YAML_SEQUENCE_NODE, n.type());
  sequence s = n.as_sequence();
  EXPECT_EQ(2u, s.size());
  EXPECT_EQ("saitama", s[0].as_scalar().str());
  EXPECT_EQ("gumma", s[1].as_scalar().str());
}

TEST(document, mapping) {
  document::ptr d = document::parse_string("{saitama: 1}");
  node n = d->get_root();
  EXPECT_EQ(YAML_MAPPING_NODE, n.type());
  mapping m = n.as_mapping();
  EXPECT_EQ(1u, m.count("saitama"));
  EXPECT_EQ(0u, m.count("gumma"));
  EXPECT_EQ("saitama", m.find("saitama").key().as_scalar().str());
  EXPECT_EQ("1", m.find("saitama").value().as_scalar().str());
}

TEST(document, parse_error) {
  EXPECT_THROW(
      document::parse_string("{"),
      yaml_parse_exception);
}

TEST(document, parse_file) {
  {
    FILE* file = fopen("tmp", "wb");
    fprintf(file, "saitama");
    fclose(file);
  }

  document::ptr d = document::parse_file("tmp");
  EXPECT_EQ("saitama", d->get_root().as_scalar().str());
}

TEST(document, parse_unknown_file) {
  EXPECT_THROW(
      document::parse_file("unknwon_file"),
      yaml_exception);
}

TEST(document, fail_to_parse_file) {
  {
    FILE* file = fopen("tmp", "wb");
    fprintf(file, "{");
    fclose(file);
  }

  EXPECT_THROW(
      document::parse_file("tmp"),
      yaml_parse_exception);
}

}  // namespace yamlcast
