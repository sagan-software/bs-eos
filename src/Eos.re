include Eos_Types;
module Api = Eos_Api;

type t = {
  config: Eos_Js.config,
  eosjs: Eos_Js.t,
};

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
    : t => {
  let config =
    Eos_Js.config(
      ~httpEndpoint,
      ~verbose?,
      ~keyProvider?,
      ~chainId?,
      ~expireInSeconds?,
      ~sign?,
      ~broadcast?,
      (),
    );
  let eosjs = Eos_Js.make(config);
  {config, eosjs};
};

let getInfo = t => t.eosjs |> Eos_Js.getInfo;

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
    : Js.Promise.t(Api.TableRows.t('row)) =>
  Eos_Js.tableRowsArgs(
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
  |> Eos_Js.getTableRowsRaw(t.eosjs)
  |> Api.thenDecode(Api.TableRows.decode(rowDecoder));

let getCode = (t, ~accountName) =>
  t.eosjs
  |. Eos_Js.getCodeRaw(accountName |. AccountName.toString)
  |> Api.thenDecode(Api.Code.decode);
