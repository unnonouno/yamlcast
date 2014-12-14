インストール方法
================

動作環境
--------

インストールには、以下のパッケージが必要です。

- `libyaml <http://pyyaml.org/wiki/LibYAML>`_
- `pficommon <http://pfi.github.io/pficommon/>`_

また、ソースからビルドするためには、以下のツールが必要になります。

- Python
- g++
- pkg-config


ビルド方法
----------

ビルドシステムとしてwafを利用しています。
同梱されているので、Pythonさえあればビルドすることができます。

.. code-block:: sh

   $ ./waf configure
   $ ./waf
   $ sudo ./waf install

インストール先をかえるときは、configure時に指定して下さい。

.. code-block:: sh

   $ ./waf configure --prefix=$HOME


ライセンス
----------

yamlcastはMITライセンスで配布されています。
