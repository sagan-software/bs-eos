open Eos_Types;
open Eos_Chain;

type t;

[@bs.deriving abstract]
type config = {
  httpEndpoint: string,
  [@bs.optional]
  verbose: bool,
  [@bs.optional]
  keyProvider: array(string),
  [@bs.optional]
  chainId: string,
  [@bs.optional]
  expireInSeconds: int,
  [@bs.optional]
  sign: bool,
  [@bs.optional]
  broadcast: bool,
};

[@bs.module] external make_ : config => t = "eosjs";

let make =
    (
      ~httpEndpoint,
      ~verbose=?,
      ~keyProvider=?,
      ~chainId=?,
      ~expireInSeconds=?,
      ~sign=?,
      ~broadcast=?,
      (),
    )
    : t =>
  config(
    ~httpEndpoint,
    ~verbose?,
    ~keyProvider?,
    ~chainId?,
    ~expireInSeconds?,
    ~sign?,
    ~broadcast?,
    (),
  )
  |> make_;

let thenDecode = (decode, promise) =>
  promise |> Js.Promise.then_(d => d |> decode |> Js.Promise.resolve);

[@bs.send]
external getInfoJson :
  (t, [@bs.as {json|true|json}] _) => Js.Promise.t(Js.Json.t) =
  "getInfo";

let getInfo = t => t |. getInfoJson |> thenDecode(Info.decode);

[@bs.deriving abstract]
type tableRowsArgs = {
  code: string,
  scope: string,
  table: string,
  [@bs.optional]
  json: bool,
  [@bs.optional] [@bs.as "table_key"]
  tableKey: string,
  [@bs.optional] [@bs.as "lower_bound"]
  lowerBound: string,
  [@bs.optional] [@bs.as "upper_bound"]
  upperBound: string,
  [@bs.optional]
  limit: int,
};

[@bs.send]
external getTableRowsRaw : (t, tableRowsArgs) => Js.Promise.t(Js.Json.t) =
  "getTableRows";

let getTableRows =
    (
      t,
      ~rowDecoder,
      ~code,
      ~scope,
      ~table,
      ~json=true,
      ~tableKey=?,
      ~lowerBound=?,
      ~upperBound=?,
      ~limit=?,
      (),
    )
    : Js.Promise.t(TableRows.t('row)) =>
  tableRowsArgs(
    ~code=code |> AccountName.toString,
    ~scope=scope |> AccountName.toString,
    ~table=table |> TableName.toString,
    ~json,
    ~tableKey=?tableKey |. Belt.Option.map(TableName.toString),
    ~lowerBound=?lowerBound |. Belt.Option.map(BigNumber.toString),
    ~upperBound=?upperBound |. Belt.Option.map(BigNumber.toString),
    ~limit?,
    (),
  )
  |> getTableRowsRaw(t)
  |> thenDecode(TableRows.decode(rowDecoder));

let getTableSingleton =
    (t, ~rowDecoder, ~code, ~scope, ~table, ~json=true, ())
    : Js.Promise.t(option('row)) =>
  getTableRows(t, ~rowDecoder, ~code, ~scope, ~table, ~json, ~limit=1, ())
  |> Js.Promise.then_((tableRows: TableRows.t('row)) =>
       tableRows.rows |. Belt.Array.get(0) |. Js.Promise.resolve
     );

[@bs.send]
external getCodeRaw : (t, string) => Js.Promise.t(Js.Json.t) = "getCode";

let getCode = (t, ~accountName) =>
  t
  |. getCodeRaw(accountName |. AccountName.toString)
  |> thenDecode(Code.decode);
