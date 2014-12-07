==========
 yamlcast
==========

.. image:: https://travis-ci.org/unnonouno/yamlcast.svg?branch=cppcheck
    :target: https://travis-ci.org/unnonouno/yamlcast

.. image:: https://coveralls.io/repos/unnonouno/yamlcast/badge.png?branch=master
  :target: https://coveralls.io/r/unnonouno/yamlcast?branch=master


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
It provide a thin wrapper for libyaml.

::

   yamlcast::docuemnt::ptr doc = yamlcast::document::parse_file(path);

Or,

::

   yamlcast::docuemnt::ptr doc = yamlcast::document::parse_string(str);


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

 User user = yamlcast::yaml_cast<User>(*doc);


See the tests for more usage!


License
=======

This library is distributed under MIT license.
