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
  type t = {
    accountName: AccountName.t,
    privileged: bool,
    lastCodeUpdate: Js.Date.t,
    created: Js.Date.t,
    coreLiquidBalance: Asset.t,
    ramQuota: BigNumber.t,
    netWeight: BigNumber.t,
    cpuWeight: BigNumber.t,
    netLimit: BigNumber.t,
    cpuLimit: BigNumber.t,
    ramUsage: BigNumber.t,
  };
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
