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

[@bs.module] external make : config => t = "eosjs";

[@bs.send]
external getInfoJson :
  (t, [@bs.as {json|true|json}] _) => Js.Promise.t(Js.Json.t) =
  "getInfo";

let getInfo = t =>
  t |. getInfoJson |> Eos_Api.thenDecode(Eos_Api.Info.decode);

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

[@bs.send]
external getCodeRaw : (t, string) => Js.Promise.t(Js.Json.t) = "getCode";

module Account = {
  type t = {
    accountName: Eos_Types.AccountName.t,
    privileged: bool,
    lastCodeUpdate: Js.Date.t,
    created: Js.Date.t,
    coreLiquidBalance: Eos_Types.Asset.t,
    ramQuota: BigNumber.t,
    netWeight: BigNumber.t,
    cpuWeight: BigNumber.t,
    netLimit: BigNumber.t,
    cpuLimit: BigNumber.t,
    ramUsage: BigNumber.t,
  };
};
