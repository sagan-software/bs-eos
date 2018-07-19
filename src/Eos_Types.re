/*
 Reference:

 - https://github.com/EOSIO/eos/blob/master/contracts/eosiolib/types.h
 - https://github.com/EOSIO/eos/tree/master/libraries/chain/include/eosio/chain
 */

module type OpaqueType = {
  type t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
  let areEqual: (t, t) => bool;
};

module type OpaqueStringTypeArgs = {type t = string;};

module type OpaqueStringType = {
  include OpaqueType;
  let fromString: string => t;
  let toString: t => string;
  let isEmpty: t => bool;
};

module MakeOpaqueStringType =
       (Opaque: OpaqueStringTypeArgs)
       : OpaqueStringType => {
  include Opaque;
  let fromString = t => t;
  let toString = t => t;
  let decode = Json.Decode.(string |> map(fromString));
  let encode = d => d |> toString |> Json.Encode.string;
  let isEmpty = t => t |. toString |. Js.String.trim |. Js.String.length === 0;
  let areEqual = (==);
};

module type OpaqueIntTypeArgs = {type t = int;};

module type OpaqueIntType = {
  include OpaqueType;
  let fromInt: int => t;
  let toInt: t => int;
};

module MakeOpaqueIntType = (Opaque: OpaqueIntTypeArgs) : OpaqueIntType => {
  include Opaque;
  let fromInt = t => t;
  let toInt = t => t;
  let decode = Json.Decode.(int |> map(fromInt));
  let encode = d => d |> toInt |> Json.Encode.int;
  let areEqual = (==);
};

module MakeChecksum256Type = (Opaque: OpaqueStringTypeArgs) => {
  include MakeOpaqueStringType(Opaque);
};

module MakeChecksum160Type = (Opaque: OpaqueStringTypeArgs) => {
  include MakeOpaqueStringType(Opaque);
};

module MakeChecksum512 = (Opaque: OpaqueStringTypeArgs) => {
  include MakeOpaqueStringType(Opaque);
};

module MakeNameType = (Opaque: OpaqueStringTypeArgs) => {
  include MakeOpaqueStringType(Opaque);
  [@bs.module "eosjs"] [@bs.scope ("modules", "format")]
  external isValid_ : string => bool = "isName";

  [@bs.module "eosjs"] [@bs.scope ("modules", "format")]
  external encode_ : string => string = "encodeName";

  let resultFromString = str =>
    if (str |. Js.String.trim |. Js.String.length === 0) {
      "Empty string is not a name" |. Belt.Result.Error;
    } else if (str
               |> Js.String.replaceByRe([%bs.re "/\\./g"], "")
               |. Js.String.trim
               |. Js.String.length === 0) {
      "Names must include characters other than dots" |. Belt.Result.Error;
    } else {
      switch (encode_(str)) {
      | _encoded => str |. fromString |. Belt.Result.Ok
      | exception (Js.Exn.Error(e)) =>
        (
          switch (Js.Exn.message(e)) {
          | Some(message) => message
          | None => {j|Could not convert "$str" to a name type|j}
          }
        )
        |. Belt.Result.Error
      };
    };

  let optionFromString = str =>
    switch (str |. resultFromString) {
    | Ok(t) => Some(t)
    | Error(_) => None
    };
};

module AccountName =
  MakeNameType({
    type t = string;
  });

module PermissionName =
  MakeNameType({
    type t = string;
  });

module TokenName =
  MakeNameType({
    type t = string;
  });

module TableName =
  MakeNameType({
    type t = string;
  });

module ScopeName =
  MakeNameType({
    type t = string;
  });

module ActionName =
  MakeNameType({
    type t = string;
  });

module TypeName =
  MakeNameType({
    type t = string;
  });

module type OpaqueDateType = {
  include OpaqueType;
  let fromDate: Js.Date.t => t;
  let toDate: t => Js.Date.t;
  let isEmpty: t => bool;
};

module type OpaqueDateTypeArgs = {
  type t;
  let fromDate: Js.Date.t => t;
  let toDate: t => Js.Date.t;
  let decode: Js.Json.t => t;
  let encode: t => Js.Json.t;
};

module MakeOpaqueDateType = (Args: OpaqueDateTypeArgs) : OpaqueDateType => {
  type t = (Args.t, Js.Date.t);
  let fromDate = date => (date |. Args.fromDate, date);
  let toDate = ((_, date): t) => date;
  [@bs.val] external isNaN : 'a => bool = "";
  let decode = j : t => {
    let native = j |. Args.decode;
    let date = native |. Args.toDate;
    let date = date |. isNaN ? Js.Date.fromFloat(0.) : date;
    (native, date);
  };
  let encode = ((i, _): t) => i |. Args.encode;
  let isEmpty = ((_, date): t) => date |. Js.Date.getTime == 0.;
  let areEqual = ((_, a), (_, b)) =>
    Js.Date.getTime(a) == Js.Date.getTime(b);
};

module BlockTimestamp =
  MakeOpaqueDateType({
    type t = string;
    let toDate = t => t ++ "Z" |. Js.Date.fromString;
    let fromDate = t =>
      t |. Js.Date.toISOString |> Js.String.slice(~from=0, ~to_=-1);
    let decode = Json.Decode.string;
    let encode = Json.Encode.string;
  });

module TimePoint =
  MakeOpaqueDateType({
    type t = BigNumber.t;
    let toDate = t =>
      t |. BigNumber.divInt(1000) |. BigNumber.toFloat |. Js.Date.fromFloat;
    let fromDate = t => t |. Js.Date.getTime |. BigNumber.fromFloat;
    let decode = BigNumber.decode;
    let encode = BigNumber.encode;
  });

module PublicKey = {
  include MakeOpaqueStringType({
    type t = string;
  });
  [@bs.module "eosjs"] [@bs.scope ("modules", "ecc")]
  external isValidString : string => bool = "isValidPublic";
  let isValid = t => t |. toString |. isValidString;
};

module PrivateKey = {
  include MakeOpaqueStringType({
    type t = string;
  });
  [@bs.module "eosjs"] [@bs.scope ("modules", "ecc")]
  external isValidString : string => bool = "isValidPrivate";
  let isValid = t => t |. toString |. isValidString;
  [@bs.module "eosjs"] [@bs.scope ("modules", "ecc")]
  external toPublicString : t => string = "privateToPublic";
  let toPublic = t => t |. toPublicString |. PublicKey.fromString;
  [@bs.module "eosjs"] [@bs.scope ("modules", "ecc")]
  external random : unit => Js.Promise.t(t) = "randomKey";
};

module Asset = {
  type t;

  [@bs.module "eosjs"] [@bs.scope ("modules", "format")]
  external fromString : string => t = "parseAsset";

  [@bs.module "eosjs"] [@bs.scope ("modules", "format")]
  external toString : t => string = "printAsset";

  let fromBigNumber = (bigNumber, ~precision, ~symbol) => {
    let amount = bigNumber |. BigNumber.toFixedDp(precision);
    {j|$amount $symbol|j} |. fromString;
  };

  let decode = Json.Decode.(string |> map(fromString));
  let encode = d => d |> toString |> Json.Encode.string;

  [@bs.get] external amount : t => string = "";
  [@bs.get] external precision : t => int = "";
  [@bs.get] external symbol : t => string = "";
};
let asset = Asset.fromString;

module MerkleRoot =
  MakeOpaqueStringType({
    type t = string;
  });

module Signature =
  MakeOpaqueStringType({
    type t = string;
  });

module BlockId =
  MakeChecksum256Type({
    type t = string;
  });

module TransactionId =
  MakeChecksum256Type({
    type t = string;
  });

module AccountPermission = {
  type t = {
    actor: AccountName.t,
    permission: PermissionName.t,
  };

  let decode = x =>
    Json.Decode.{
      actor: x |> field("actor", AccountName.decode),
      permission: x |> field("permission", PermissionName.decode),
    };

  let encode = x =>
    Json.Encode.(
      object_([
        ("actor", x.actor |> AccountName.encode),
        ("permission", x.permission |> PermissionName.encode),
      ])
    );
};
