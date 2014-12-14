使い方
======

yamlcastはC++から使われます。
利用したいソースファイルから `yamlcast.hpp` をインクルードして下さい。

::

   #include <yamlcast.hpp>


簡単な使い方
------------

はじめに、 :cpp:class:`yamlcast::document` を用意する必要があります。
このオブジェクトは、単一のYAMLドキュメントを示します。
実体は、libyamlの薄いラッパーです。
文字列をパースするときは、 :cpp:class:`yamlcast::document::parse_string` を使って下さい。

::

   std::string str = "...";
   yamlcast::document::ptr doc = yamlcast::document::parse_string(str);

ファイルをパースするときは、 :cpp:class:`yamlcast::document::parse_file` を使って下さい。

::

   yamlcast::document::ptr doc = yamlcast::document::parse_file("/path/to/file");


準備が出来ました。
:cpp:func:`yamlcast::yaml_cast` を呼び出しましょう。
読み込んだYAML文書を、任意の型に変換します。

::

   std::map<std::string, int> m
     = yamlcast::yaml_cast<std::map<std::string, int> >(*doc);


YAMLの各ノードは、値を示すスカラー（scalar）と、配列を示すシーケンス（sequence）、ハッシュを示すマッピング（mapping）があります。
個別のデータ型はスカラーから、 :cpp:class:`std::vector` にはシーケンスから、 :cpp:class:`std::map` にはマッピングから変換できます。
   

ユーザー定義型
--------------

YAMLのマッピングから、ユーザー定義の型に変換することもできます。
この場合、pficommonのシリアライザを使います。
pficommonのシリアライザは、 `serialize` という名前のtemplateメンバ関数を作ることで利用できます。
引数に渡された参照に対して、 `operator &` で、データをシリアライズできます。
このとき、 `MEMBER` マクロでメンバ関数をくくって下さい。

::

   #include <pficommon/data/serialization.h>

   struct User {
     int age;
     std::string name;

     template <typename Ar>
     void serialize(Ar& ar) {
       ar & MEMBER(age) & MEMBER(name);
     }
   };

   // ...

   User user = yamlcast::yaml_cast<User>(*doc);


エラー処理
----------

yamlcastのエラーは大きく分けて2つあります。
1つは、YAML文書の読み込みの失敗で、これは正しくないYAML文書が与えられた時に発生します。
もう1つは、型変換時の不整合で、与えたYAML文書が期待した型になっていない時に発生します。

YAML文書のチェックはパース時に行われます。
以下の文字列は `[` が閉じていませんから、正しいYAML文書として読み込めません。

::

   yamlcast::document::parse_string("[1,2");

このとき、 :cpp:class:`yamlcast::yaml_parse_exception` が発生します。


型情報のチェックはキャストするときに行われます。
型の不整合は、3種類あります。

YAML文書上での型と、キャスト先の型が一致しない時は、 :cpp:class:`yamlcast::yaml_bad_cast` が発生します。

::

   yamlcast::document::ptr d = yamlcast::document::parse_string("{\"name\": \"taro\"}");
   std::vector<std::string> xs = yamalcast::yaml_cast<std::vector<std::string> >(*d);

上の例は、 :cpp:class:`vector` にキャストしたかったため、シーケンスを期待していましたが、マッピングが与えられたので、エラーとなります。

YAML文書上では、文字列と数値は、いずれもスカラーとして区別されません。
スカラーの値を対象の型に変換するのに失敗すると、 :cpp:class:`yamlcast::yaml_invalid_scalar` が発生します。

::

   yamlcast::document::ptr d = yamlcast::document::parse_string("[1, \"hello\"]");
   std::vector<int> xs = yamalcast::yaml_cast<std::vector<int> >(*d);

上の例は、 "hello" という文字列を `int` に変換しようとしたため、エラーが発生します。


ユーザー定義型は、YAML文書上ではマッピングを利用します。
想定したフィールドが存在しない時は、 :cpp:class:`yamlcast::yaml_not_found` が発生します。

::

   yamlcast::document::ptr d = yamlcast::document::parse_string("{\"age\": 20}");
   User user = yamalcast::yaml_cast<User>(*d);

上の例は、 `User` 中の `name` フィールドが、マッピング中に存在しないためエラーが発生します。
