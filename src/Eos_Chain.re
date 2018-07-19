open Eos_Types;

module Info = {
  type t = {
    serverVersion: string,
    headBlockNum: int,
    lastIrreversibleBlockNum: int,
    lastIrreversibleBlockId: BlockId.t,
    headBlockId: BlockId.t,
    headBlockTime: Js.Date.t,
    headBlockProducer: AccountName.t,
    virtualBlockCpuLimit: BigNumber.t,
    virtualBlockNetLimit: BigNumber.t,
    blockCpuLimit: BigNumber.t,
    blockNetLimit: BigNumber.t,
  };

  let decode = d =>
    Json.Decode.{
      serverVersion: d |> field("server_version", string),
      headBlockNum: d |> field("head_block_num", int),
      lastIrreversibleBlockNum:
        d |> field("last_irreversible_block_num", int),
      lastIrreversibleBlockId:
        d |> field("last_irreversible_block_id", BlockId.decode),
      headBlockId: d |> field("head_block_id", BlockId.decode),
      headBlockTime: d |> field("head_block_time", date),
      headBlockProducer:
        d |> field("head_block_producer", AccountName.decode),
      virtualBlockCpuLimit:
        d |> field("virtual_block_cpu_limit", BigNumber.decode),
      virtualBlockNetLimit:
        d |> field("virtual_block_net_limit", BigNumber.decode),
      blockCpuLimit: d |> field("block_cpu_limit", BigNumber.decode),
      blockNetLimit: d |> field("block_net_limit", BigNumber.decode),
    };
};

module Account = {
  type limit = {
    used: BigNumber.t,
    available: BigNumber.t,
    max: BigNumber.t,
  };

  type permissionKey = {
    key: PublicKey.t,
    weight: int,
  };

  type permissionAccount = {
    permission: AccountPermission.t,
    weight: int,
  };

  type permissionRequiredAuth = {
    threshold: int,
    keys: array(permissionKey),
    accounts: array(permissionAccount),
    /* waits */
  };

  type permission = {
    permName: PermissionName.t,
    parent: PermissionName.t,
    requiredAuth: permissionRequiredAuth,
  };

  type t = {
    accountName: AccountName.t,
    headBlockNum: int,
    headBlockTime: BlockTimestamp.t,
    privileged: bool,
    lastCodeUpdate: BlockTimestamp.t,
    created: BlockTimestamp.t,
    coreLiquidBalance: Asset.t,
    ramQuota: BigNumber.t,
    netWeight: BigNumber.t,
    cpuWeight: BigNumber.t,
    netLimit: limit,
    cpuLimit: limit,
    ramUsage: BigNumber.t,
    permissions: array(permission),
    totalResources: option(Eosio_System.UserResources.t),
    selfDelegatedBandwidth: option(Eosio_System.DelegatedBandwidth.t),
    /* refundRequest:  */
    voterInfo: option(Eosio_System.VoterInfo.t),
  };

  module Decode = {
    open Json.Decode;

    let limit = x => {
      used: x |> field("used", BigNumber.decode),
      available: x |> field("available", BigNumber.decode),
      max: x |> field("max", BigNumber.decode),
    };

    let permissionKey = x => {
      key: x |> field("key", PublicKey.decode),
      weight: x |> field("weight", int),
    };

    let permissionAccount = x => {
      permission: x |> field("permission", AccountPermission.decode),
      weight: x |> field("weight", int),
    };

    let permissionRequiredAuth = x => {
      threshold: x |> field("threshold", int),
      keys: x |> field("keys", array(permissionKey)),
      accounts: x |> field("accounts", array(permissionAccount)),
    };

    let permission = x => {
      permName: x |> field("perm_name", PermissionName.decode),
      parent: x |> field("parent", PermissionName.decode),
      requiredAuth: x |> field("required_auth", permissionRequiredAuth),
    };

    let nullableOption = decoder =>
      decoder |> Json.Decode.nullable |> Json.Decode.map(Js.Null.toOption);

    let t = x => {
      accountName: x |> field("account_name", AccountName.decode),
      headBlockNum: x |> field("head_block_num", int),
      headBlockTime: x |> field("head_block_time", BlockTimestamp.decode),
      privileged: x |> field("privileged", bool),
      lastCodeUpdate: x |> field("last_code_update", BlockTimestamp.decode),
      created: x |> field("created", BlockTimestamp.decode),
      coreLiquidBalance: x |> field("core_liquid_balance", Asset.decode),
      ramQuota: x |> field("ram_quota", BigNumber.decode),
      netWeight: x |> field("net_weight", BigNumber.decode),
      cpuWeight: x |> field("cpu_weight", BigNumber.decode),
      netLimit: x |> field("net_limit", limit),
      cpuLimit: x |> field("cpu_limit", limit),
      ramUsage: x |> field("ram_usage", BigNumber.decode),
      permissions: x |> field("permissions", array(permission)),
      totalResources:
        x
        |> field(
             "total_resources",
             nullableOption(Eosio_System.UserResources.decode),
           ),
      selfDelegatedBandwidth:
        x
        |> field(
             "self_delegated_bandwidth",
             nullableOption(Eosio_System.DelegatedBandwidth.decode),
           ),
      voterInfo:
        x
        |> field("voter_info", nullableOption(Eosio_System.VoterInfo.decode)),
    };
  };

  let decode = Decode.t;
};

module Transaction = {
  type authorization = {
    actor: AccountName.t,
    permission: PermissionName.t,
  };

  type action = {
    account: AccountName.t,
    name: ActionName.t,
    authorization: array(authorization),
    data: Js.Json.t,
    hexData: string,
  };

  type transaction = {
    expiration: Js.Date.t,
    refBlockNum: int,
    refBlockPrefix: int,
    maxNetUsageWords: int,
    maxCpuUsageMs: int,
    delaySec: int,
    actions: array(action),
  };

  type trx = {
    id: TransactionId.t,
    signatures: array(Signature.t),
    compression: string,
    packedContextFreeData: string,
    packedTrx: string,
    transaction,
  };

  type t = {
    status: string,
    cpuUsageUs: int,
    netUsageWords: int,
    trx,
  };
};

module Block = {
  type t = {
    timestamp: Js.Date.t,
    producer: AccountName.t,
    confirmed: int,
    previous: BlockId.t,
    transactionMroot: MerkleRoot.t,
    actionMroot: MerkleRoot.t,
    scheduleVersion: int,
    producerSignature: Signature.t,
    transactions: array(Transaction.t),
    id: BlockId.t,
    num: int,
    refBlockPrefix: string,
  };
};

module TableRows = {
  type t('row) = {
    rows: array('row),
    more: bool,
  };

  let decode = (decodeRow, d) =>
    Json.Decode.{
      rows: d |> field("rows", array(decodeRow)),
      more: d |> field("more", bool),
    };

  let map = (fn, tableRows) => {
    ...tableRows,
    rows: tableRows.rows |. Belt.Array.map(fn),
  };

  let singleton = tableRows => tableRows.rows |. Belt.Array.get(0);
};

module Code = {
  type t = {
    accountName: AccountName.t,
    codeHash: string,
    wast: string,
    wasm: string,
    abi: option(Eos_Abi.t),
  };

  let decode = d =>
    Json.Decode.{
      accountName: d |> field("account_name", AccountName.decode),
      codeHash: d |> field("code_hash", string),
      wast: d |> field("wast", string),
      wasm: d |> field("wasm", string),
      abi: d |> optional(field("abi", Eos_Abi.decode)),
    };

  let encode = d =>
    Json.Encode.(
      object_([
        ("account_name", d.accountName |> AccountName.encode),
        ("code_hash", d.codeHash |> string),
        ("wast", d.wast |> string),
        ("wasm", d.wasm |> string),
        (
          "abi",
          d.abi
          |. Belt.Option.map(Eos_Abi.encode)
          |. Belt.Option.getWithDefault(null),
        ),
      ])
    );
};
