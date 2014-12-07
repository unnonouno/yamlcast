例外
----

YAML文書のパースや、YAML文書からの変換が失敗した場合、戻り値ではなくて例外で失敗を通知します。
ここでは、yamlcastが生成する例外を解説します。

.. cpp:namespace:: yamlcast

.. cpp:class:: yaml_exception : public std::exception

   yamlcastの返す全ての例外のベースクラスとなります。


.. cpp:class:: yaml_parse_exception : public yaml_exception

   YAMLの構文解析に失敗した時に発生する例外です。

   .. cpp:function:: const yaml_mark_t& get_problem_mark() const

      エラーが起こった場所を取得します。


.. cpp:class:: yaml_cast_exception : public yaml_exception

   読み込んだYAMLデータから型変換に失敗するときに発生する例外です。

   .. cpp:function:: const yaml_mark_t& get_start_mark() const

      エラーの原因となったYAMLのノードの開始位置を取得します。

   .. cpp:function:: const yaml_mark_t& get_end_mark() const

      エラーの原因となったYAMLのノードの終了位置を取得します。


.. cpp:class:: yaml_bad_cast : public yaml_cast_exception

   指定した型への変換に失敗した時に発生する例外です。

   .. cpp:function:: yaml_node_type_t get_expect() const

      期待したYAMLのタイプを取得します。

   .. cpp:function:: yaml_node_type_t get_actual() const

      実際のYAMLのタイプを取得します。


.. cpp:class:: yaml_invalid_scalar : public yaml_cast_exception

   YAML文書中のスカラーを、指定した型に変換するのに失敗した時に発生する例外です。

   .. cpp:function:: const std::type_info& get_type() const

      変換に失敗したデータ型を取得します。


.. cpp:class:: yaml_not_found : public yaml_cast_exception

   ユーザー定義型へ変換するときに、期待したフィールドが見つからなかった沖に発生する例外です。

   .. cpp:function:: const std::string& get_key() const

      見つからなかったフィールド名を取得します。
