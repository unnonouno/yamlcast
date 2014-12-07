キャスト
========

YAML文書を読み込んだら、 :cpp:func:`yamlcast::yaml_cast` 関数を呼び出して変換を行います。

.. cpp:namespace:: yamlcast

.. cpp:function:: T yaml_cast<T>(const document& yaml)

   YAML文書を与えられて型Tとしてパースします。
   パースの規則は、型Tに依存します。

   ユーザー定義型など、下記の型以外への変換は、pficommonのシリアライザにしたがって、 `serialize` メンバ関数を呼び出します。
   `serializa` 関数内では、 `MEMBER` マクロか、 `NAMED_MEMBER` マクロを使って、変換したいフィールドを指定して下さい。
   与えられたYAMLがマッピングの時に、指定されたフィールドを対象のメンバ変数に変換します。

   :throw yaml_bad_cast: 対象がマッピングではない場合に発生します。
   :throw yaml_not_found: 指定されたフィールドが見つからなかった時に発生します。


.. cpp:function:: bool yaml_cast<bool>(const document& yaml)

   YAMLをブール値に変換します。

   :return: 文字列"true"の場合はtrueに、"false"の場合はfalseを返します。
   :throw yaml_bad_cast: 対象がスカラーではない場合に発生します。
   :throw yaml_invalid_scalar: スカラーの値だが、"true"でも"false"でもない場合に発生します。

.. cpp:function:: bool yaml_cast<int8_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<int16_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<int32_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<int64_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<uint8_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<uint16_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<uint32_t>(const document& yaml)
.. cpp:function:: bool yaml_cast<uint64_t>(const document& yaml)

   YAMLを整数に変換します。

   :return: 文字列を整数に変換した結果を返します。
   :throw yaml_bad_cast: 対象がスカラーではない場合に発生します。
   :throw yaml_invalid_scalar: スカラーの値が整数でないときや、範囲外の値のときに発生します。


.. cpp:function:: bool yaml_cast<float>(const document& yaml)
.. cpp:function:: bool yaml_cast<double>(const document& yaml)

   YAMLを浮動小数点数に変換します。

   :return: 文字列を浮動小数点数に変換した値を返します。
   :throw yaml_bad_cast: 対象がスカラーではない場合に発生します。
   :throw yaml_invalid_scalar: スカラーの値が浮動小数点数でないときに発生します。


.. cpp:function:: std::vector<T> yaml_cast<std::vector<T> >(const document& yaml)

   YAMLを配列に変換します。
   対象のノードがシーケンスの場合に、再帰的にシーケンス中のノードを `T` 型に変換します。

   :return: 変換された配列を返します。
   :throw yaml_bad_cast: 対象がシーケンスではない場合や、再帰的な要素の変換に失敗した時に発生します。


.. cpp:function:: std::map<K, V> yaml_cast<std::map<K, V> >(const document& yaml)

   YAMLをマップに変換します。
   対象のノードがマッピングの場合に、再帰的にマッピング中のキーと値のノードを `K`, `V` 型に変換します。

   :return: 変換されたマップを返します。
   :throw yaml_bad_cast: 対象がマッピングではない場合や、再帰的な要素の変換に失敗した時に発生します。

