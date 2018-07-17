open Eos_Types;

type type_ = {
  newTypeName: TypeName.t,
  type_: TypeName.t,
};

type structField = {
  name: TypeName.t,
  type_: TypeName.t,
};

type struct_ = {
  name: TypeName.t,
  base: option(TypeName.t),
  fields: array(structField),
};

type action = {
  name: ActionName.t,
  type_: TypeName.t,
  ricardianContract: string,
};

type table = {
  name: TableName.t,
  type_: TypeName.t,
  indexType: TypeName.t,
  keyNames: array(TypeName.t),
  keyTypes: array(TypeName.t),
};

type t = {
  comment: string,
  version: option(string),
  structs: array(struct_),
  actions: array(action),
  tables: array(table),
  /* clauses */
  /* ricardian_clauses */
};

module Decode = {
  open Json.Decode;

  let type_ = j => {
    newTypeName: j |> field("new_type_name", TypeName.decode),
    type_: j |> field("type", TypeName.decode),
  };

  let structField = j => {
    name: j |> field("name", TypeName.decode),
    type_: j |> field("type", TypeName.decode),
  };

  let struct_ = j => {
    name: j |> field("name", TypeName.decode),
    base: j |> optional(field("base", TypeName.decode)),
    fields: j |> field("fields", array(structField)),
  };

  let action = j => {
    name: j |> field("name", ActionName.decode),
    type_: j |> field("type", TypeName.decode),
    ricardianContract:
      j
      |> optional(field("ricardian_contract", string))
      |. Belt.Option.getWithDefault(""),
  };

  let table = j => {
    name: j |> field("name", TableName.decode),
    type_: j |> field("type", TypeName.decode),
    indexType: j |> field("index_type", TypeName.decode),
    keyNames: j |> field("key_names", array(TypeName.decode)),
    keyTypes: j |> field("key_types", array(TypeName.decode)),
  };

  let t = j => {
    comment:
      j
      |> optional(field("comment", string))
      |. Belt.Option.getWithDefault(""),
    version: j |> optional(field("version", string)),
    structs: j |> field("structs", array(struct_)),
    actions: j |> field("actions", array(action)),
    tables: j |> field("tables", array(table)),
  };
};

let decode = Decode.t;

module Encode = {
  open Json.Encode;

  let type_ = (d: type_) =>
    object_([
      ("new_type_name", d.newTypeName |> TypeName.encode),
      ("type", d.type_ |> TypeName.encode),
    ]);

  let structField = (d: structField) =>
    object_([
      ("name", d.name |> TypeName.encode),
      ("type", d.type_ |> TypeName.encode),
    ]);

  let struct_ = (d: struct_) =>
    object_([
      ("name", d.name |> TypeName.encode),
      (
        "base",
        d.base
        |. Belt.Option.map(TypeName.encode)
        |. Belt.Option.getWithDefault(null),
      ),
      ("fields", d.fields |> array(structField)),
    ]);

  let action = (d: action) =>
    object_([
      ("name", d.name |> ActionName.encode),
      ("type", d.type_ |> TypeName.encode),
      ("ricardian_contract", d.ricardianContract |> string),
    ]);

  let table = (d: table) =>
    object_([
      ("name", d.name |> TableName.encode),
      ("type", d.type_ |> TypeName.encode),
      ("index_type", d.indexType |> TypeName.encode),
      ("key_names", d.keyNames |> array(TypeName.encode)),
      ("key_types", d.keyTypes |> array(TypeName.encode)),
    ]);

  let t = (d: t) =>
    object_([
      ("comment", d.comment |> string),
      ("version", d.version |. Belt.Option.getWithDefault("") |> string),
      ("structs", d.structs |> array(struct_)),
      ("actions", d.actions |> array(action)),
      ("tables", d.tables |> array(table)),
    ]);
};

let encode = Encode.t;
