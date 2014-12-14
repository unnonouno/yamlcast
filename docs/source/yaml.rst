YAML文書
--------


.. cpp:namespace:: yamlcast

yamlcastはlibyamlの薄いラッパーを提供します。
:cpp:class:`document` は読み込まれたYAMLドキュメントを、 :cpp:class:`node` はYAMLドキュメント中の各ノードを示すクラスです。

.. cpp:class:: document

   読み込んだYAMLドキュメントを示します。
   中身は、libyamlのdocument_t型へのポインタです。

   .. cpp:type:: ptr

      :cpp:class:`yamlcast::document` へのshared_ptrの別名です。


   .. cpp:function:: static ptr parse_string(const std::string& str)

      文字列をパースして、読み込まれたドキュメントを返します。

      :param str: パース対象の文字列を渡します。
      :return: パースした結果を :cpp:class:`yamlcast::document` へのポインタで返します。
      :throw yamlcast\:\:yaml_parse_exception:  パースに失敗した場合に発生します。

   
   .. cpp:function:: static document::ptr parse_file(const std::string& path)

   .. cpp:function:: static document::ptr parse_file(FILE* file)

      フィアルを読み込んでパース市、読み込まれたドキュメントを返します。

      :param str: パース対象ファイルへのパスを渡します。
      :return: パースした結果を :cpp:class:`yamlcast::document` へのポインタで返します。
      :throw yamlcast\:\:yaml_parse_exception:  パースに失敗した場合に発生します。


   .. cpp:function:: node get_root() const

      ルートノードを取得します。


.. cpp:class:: node

   :cpp:class:`yamlcast::document` 中のノードを示します。
   ノードには、スカラー、シーケンス、マッピングの3種類のタイプが有ります。

   .. cpp:function:: const document& get_document() const

      このノードが所属するYAML文書への参照を返します。

   .. cpp:function:: yaml_node_type_t type() const

      このノードの種類を示します。

   .. cpp:function:: scalar as_scalar() const

      このノードをスカラーとして扱います。
      ノードがスカラーでない時は例外が発生します。
      ノードのタイプは、 :cpp:func:`yamlcast::node::type` で確認できます。

      :throw yamlcast\:\:yaml_bad_cast: ノードがスカラーでない時に発生します。

   .. cpp:function:: sequence as_sequence() const

      このノードをシーケンスとして扱います。
      ノードがシーケンスでない時は例外が発生します。
      ノードのタイプは、 :cpp:func:`yamlcast::node::type` で確認できます。

      :throw yamlcast\:\:yaml_bad_cast: ノードがスカラーでない時に発生します。

   .. cpp:function:: mapping as_mapping() const

      このノードをマッピングとして扱います。
      ノードがマッピングでない時は例外が発生します。
      ノードのタイプは、 :cpp:func:`yamlcast::node::type` で確認できます。

      :throw yamlcast\:\:yaml_bad_cast: ノードがスカラーでない時に発生します。

   .. cpp:function:: const yaml_mark_t& start_mark() const

      このノードの入力データ中での開始位置を取得します。

   .. cpp:function:: const yaml_mark_t& end_mark() const

      このノードの入力データ中での終了位置を取得します。


.. cpp:class:: scalar

   スカラーのノードを示します。
   スカラーは、値を内部では文字列として持っています。
   数値やブール値へは、値を読んで変換をする必要があります。

   .. cpp:function:: bool operator ==(const char* str) const

   .. cpp:function:: bool operator ==(const std::string& str) const

      スカラーの値が、与えられた文字列と一致するか調べます。

   .. cpp:function:: size_t length() const

      スカラーの値の文字列の長さを取得します。

   .. cpp:function:: const char* value() const

      スカラーの値の文字列へのポインタを取得します。

   .. cpp:function:: std::string str() const

      スカラーの値の文字列を、 :cpp:class:`std::string` として取得します。


.. cpp:class:: sequence

   シーケンスのノードを示します。
   シーケンスは内部に複数のノードを保持しています。

   .. cpp:function:: const_iterator begin() const

      シーケンスの最初の要素へのイテレーターを取得します。

   .. cpp:function:: const_iterator end() const

      シーケンスの最後の要素の次へのイテレーターを取得します。

   .. cpp:function:: std::size_t size() const

      シーケンス中のノードの数を取得します。

   .. cpp:function:: node operator[](const std::size_t i) const

      `i` 番目のノードを取得します。


.. cpp:class:: mapping

   マッピングのノードを示します。
   マッピングは内部にキーと値のノードを保持します。

   .. cpp:function:: const_iterator begin() const

      マッピングの最初の要素へのイテレーターを取得します。

   .. cpp:function:: const_iterator end() const

      マッピングの最後の要素の次へのイテレーターを取得します。

   .. cpp:function:: const_iterator find(const std::string& key) const

      指定された文字列をキーとして持つ要素を探して、そのイテレーターを返します。
      見つからなかった時は、 :cpp:func:`end` の値を返します。

   .. cpp:function:: std::size_t count(const std::string& key) const

      指定された文字列をキーとして持つ要素の数を返します。


