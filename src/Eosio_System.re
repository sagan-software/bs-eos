module GlobalState = {
  type t = {
    maxBlockNetUsage: int,
    targetBlockNetUsagePct: int,
    maxTransactionNetUsage: int,
    basePerTransactionNetUsage: int,
    netUsageLeeway: int,
    contextFreeDiscountNetUsageNum: int,
    contextFreeDiscountNetUsageDen: int,
    maxBlockCpuUsage: int,
    targetBlockCpuUsagePct: int,
    maxTransactionCpuUsage: int,
    minTransactionCpuUsage: int,
    maxTransactionLifetime: int,
    deferredTrxExpirationWindow: int,
    maxTransactionDelay: int,
    maxInlineActionSize: int,
    maxInlineActionDepth: int,
    maxAuthorityDepth: int,
    maxRamSize: BigNumber.t,
    totalRamBytesReserved: BigNumber.t,
    totalRamStake: BigNumber.t,
    lastProducerScheduleUpdate: Eos.BlockTimestamp.t,
    lastPerVoteBucketFill: Eos.TimePoint.t,
    perVoteBucket: int,
    perBlockBucket: int,
    totalUnpaidBlocks: int,
    totalActivatedStake: BigNumber.t,
    threshActivatedStakeTime: Eos.TimePoint.t,
    lastProducerScheduleSize: int,
    totalProducerVoteWeight: BigNumber.t,
    lastNameClose: Eos.BlockTimestamp.t,
  };

  let decode = x =>
    Json.Decode.{
      maxBlockNetUsage: x |> field("max_block_net_usage", int),
      targetBlockNetUsagePct: x |> field("target_block_net_usage_pct", int),
      maxTransactionNetUsage: x |> field("max_transaction_net_usage", int),
      basePerTransactionNetUsage:
        x |> field("base_per_transaction_net_usage", int),
      netUsageLeeway: x |> field("net_usage_leeway", int),
      contextFreeDiscountNetUsageNum:
        x |> field("context_free_discount_net_usage_num", int),
      contextFreeDiscountNetUsageDen:
        x |> field("context_free_discount_net_usage_den", int),
      maxBlockCpuUsage: x |> field("max_block_cpu_usage", int),
      targetBlockCpuUsagePct: x |> field("target_block_cpu_usage_pct", int),
      maxTransactionCpuUsage: x |> field("max_transaction_cpu_usage", int),
      minTransactionCpuUsage: x |> field("min_transaction_cpu_usage", int),
      maxTransactionLifetime: x |> field("max_transaction_lifetime", int),
      deferredTrxExpirationWindow:
        x |> field("deferred_trx_expiration_window", int),
      maxTransactionDelay: x |> field("max_transaction_delay", int),
      maxInlineActionSize: x |> field("max_inline_action_size", int),
      maxInlineActionDepth: x |> field("max_inline_action_depth", int),
      maxAuthorityDepth: x |> field("max_authority_depth", int),
      maxRamSize: x |> field("max_ram_size", BigNumber.decode),
      totalRamBytesReserved:
        x |> field("total_ram_bytes_reserved", BigNumber.decode),
      totalRamStake: x |> field("total_ram_stake", BigNumber.decode),
      lastProducerScheduleUpdate:
        x |> field("last_producer_schedule_update", Eos.BlockTimestamp.decode),
      lastPerVoteBucketFill:
        x |> field("last_pervote_bucket_fill", Eos.TimePoint.decode),
      perVoteBucket: x |> field("pervote_bucket", int),
      perBlockBucket: x |> field("perblock_bucket", int),
      totalUnpaidBlocks: x |> field("total_unpaid_blocks", int),
      totalActivatedStake:
        x |> field("total_activated_stake", BigNumber.decode),
      threshActivatedStakeTime:
        x |> field("thresh_activated_stake_time", Eos.TimePoint.decode),
      lastProducerScheduleSize:
        x |> field("last_producer_schedule_size", int),
      totalProducerVoteWeight:
        x |> field("total_producer_vote_weight", BigNumber.decode),
      lastNameClose: x |> field("last_name_close", Eos.BlockTimestamp.decode),
    };
};

let getGlobalState =
  Eos.getTableSingleton(
    ~rowDecoder=GlobalState.decode,
    ~code=Eos.accountName("eosio"),
    ~scope=Eos.accountName("eosio"),
    ~table=Eos.tableName("global"),
  );

module ProducerInfo = {
  type t = {
    owner: Eos.AccountName.t,
    totalVotes: BigNumber.t,
    producerKey: string,
    isActive: bool,
    url: string,
    unpaidBlocks: int,
    lastClaimTime: Eos.TimePoint.t,
    location: int,
  };
  let decode = j =>
    Json.Decode.{
      owner: j |> field("owner", Eos.AccountName.decode),
      totalVotes: j |> field("total_votes", BigNumber.decode),
      producerKey: j |> field("producer_key", string),
      isActive: j |> field("is_active", int |> map(x => x === 1)),
      url: j |> field("url", string),
      unpaidBlocks: j |> field("unpaid_blocks", int),
      lastClaimTime: j |> field("last_claim_time", Eos.TimePoint.decode),
      location: j |> field("location", int),
    };
  let encode = d =>
    Json.Encode.(
      object_([
        ("owner", d.owner |> Eos.AccountName.encode),
        ("total_votes", d.totalVotes |> BigNumber.encode),
        ("producer_key", d.producerKey |> string),
        ("is_active", int(d.isActive ? 1 : 0)),
        ("url", d.url |> string),
        ("unpaid_blocks", d.unpaidBlocks |> int),
        ("last_claim_time", d.lastClaimTime |> Eos.TimePoint.encode),
        ("location", d.location |> int),
      ])
    );
};

let getProducers =
  Eos.getTableRows(
    ~code=Eos.accountName("eosio"),
    ~scope=Eos.accountName("eosio"),
    ~table=Eos.tableName("producers"),
    ~rowDecoder=ProducerInfo.decode,
  );

module VoterInfo = {
  type t = {
    owner: Eos.AccountName.t,
    proxy: Eos.AccountName.t,
    producers: array(Eos.AccountName.t),
    staked: int,
    lastVoteWeight: BigNumber.t,
    proxiedVoteWeight: BigNumber.t,
    isProxy: bool,
  };
};

module UserResources = {
  type t = {
    owner: Eos.AccountName.t,
    netWeight: Eos.Asset.t,
    cpuWeight: Eos.Asset.t,
    ramBytes: int,
  };
};

module DelegatedBandwidth = {
  type t = {
    from: Eos.AccountName.t,
    to_: Eos.AccountName.t,
    netWeight: Eos.Asset.t,
    cpuWeight: Eos.Asset.t,
  };
};

module Connector = {
  type t = {
    balance: Eos.Asset.t,
    weight: BigNumber.t,
  };
};

module ExchangeState = {
  type t = {
    supply: Eos.Asset.t,
    base: Connector.t,
    quote: Connector.t,
  };
};

module RefundRequest = {
  type t = {
    owner: Eos.AccountName.t,
    requestTime: Js.Date.t,
    netAmount: Eos.Asset.t,
    cpuAmount: Eos.Asset.t,
  };
};

module NameBidInfo = {
  type t = {
    newName: Eos.AccountName.t,
    highBidder: Eos.AccountName.t,
    highBid: int,
    lastBidTime: Js.Date.t,
  };
};
