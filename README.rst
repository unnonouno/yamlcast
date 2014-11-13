==========
 yamlcast
==========

yamlcast is a simple YAML serialization library for pficommon.
You can serialize and deserialize arbitrary data with pficommon's serialization method.


Requirement
===========

- pficommon

  - https://github.com/pfi/pficommon
  - Common library for C++ including serialization library

- libyaml

  - http://pyyaml.org/wiki/LibYAML
  - YAML libarary for C


Install
=======

::

 $ ./waf configure
 $ ./waf
 $ ./waf install


Usage
=====

This library works like pficommon's serialization library.

::

   // Use libyaml to load yaml file
   yaml_parser_t parser;
   ::yaml_parser_initialize(&parser);
   ::yaml_parser_set_input_file(&parser, file);
   yaml_document_t yaml;
   ::yaml_parser_load(&parser, &yaml);
   ::yaml_parser_delete(&parser);


And, use `serialize` method and `operator &` for your own strcuts like pficommon.

::

 struct User {
   int age;
   std::string name;

   template <typename Ar>
   void serialize(Ar& ar) {
     ar & MEMBER(age) & MEMBER(name);
   }
 };

 User user = yamlcast::yaml_cast<User>(yaml);


See the tests for more usage!


License
=======

This library is distributed under MIT license.
