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

- yaml-cpp

  - http://code.google.com/p/yaml-cpp/
  - YAML libarary for C++
  - yamlcast only works in the old API


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

 YAML::Node node;
 // parse YAML ...
 int n = yamlcast::yaml_cast<int>(node);


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

 User user = yamlcast::yaml_cast<User>(node);


See the sources for more usage!


License
=======

This library is distributed under MIT license.
