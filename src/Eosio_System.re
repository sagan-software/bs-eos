open Eos_Types;

module GlobalState = {
  type t = {
    maxBlockNetUsage: BigNumber.t,
    targetBlockNetUsagePct: BigNumber.t,
    maxTransactionNetUsage: BigNumber.t,
    basePerTransactionNetUsage: BigNumber.t,
    netUsageLeeway: BigNumber.t,
    contextFreeDiscountNetUsageNum: BigNumber.t,
    contextFreeDiscountNetUsageDen: BigNumber.t,
    maxBlockCpuUsage: BigNumber.t,
    targetBlockCpuUsagePct: BigNumber.t,
    maxTransactionCpuUsage: BigNumber.t,
    minTransactionCpuUsage: BigNumber.t,
    maxTransactionLifetime: BigNumber.t,
    deferredTrxExpirationWindow: BigNumber.t,
    maxTransactionDelay: BigNumber.t,
    maxInlineActionSize: BigNumber.t,
    maxInlineActionDepth: BigNumber.t,
    maxAuthorityDepth: BigNumber.t,
    maxRamSize: BigNumber.t,
    totalRamBytesReserved: BigNumber.t,
    totalRamStake: BigNumber.t,
    lastProducerScheduleUpdate: BlockTimestamp.t,
    lastPerVoteBucketFill: TimePoint.t,
    perVoteBucket: BigNumber.t,
    perBlockBucket: BigNumber.t,
    totalUnpaidBlocks: BigNumber.t,
    totalActivatedStake: BigNumber.t,
    threshActivatedStakeTime: TimePoint.t,
    lastProducerScheduleSize: BigNumber.t,
    totalProducerVoteWeight: BigNumber.t,
    lastNameClose: BlockTimestamp.t,
  };

  let decode = x =>
    Json.Decode.{
      maxBlockNetUsage: x |> field("max_block_net_usage", BigNumber.decode),
      targetBlockNetUsagePct:
        x |> field("target_block_net_usage_pct", BigNumber.decode),
      maxTransactionNetUsage:
        x |> field("max_transaction_net_usage", BigNumber.decode),
      basePerTransactionNetUsage:
        x |> field("base_per_transaction_net_usage", BigNumber.decode),
      netUsageLeeway: x |> field("net_usage_leeway", BigNumber.decode),
      contextFreeDiscountNetUsageNum:
        x |> field("context_free_discount_net_usage_num", BigNumber.decode),
      contextFreeDiscountNetUsageDen:
        x |> field("context_free_discount_net_usage_den", BigNumber.decode),
      maxBlockCpuUsage: x |> field("max_block_cpu_usage", BigNumber.decode),
      targetBlockCpuUsagePct:
        x |> field("target_block_cpu_usage_pct", BigNumber.decode),
      maxTransactionCpuUsage:
        x |> field("max_transaction_cpu_usage", BigNumber.decode),
      minTransactionCpuUsage:
        x |> field("min_transaction_cpu_usage", BigNumber.decode),
      maxTransactionLifetime:
        x |> field("max_transaction_lifetime", BigNumber.decode),
      deferredTrxExpirationWindow:
        x |> field("deferred_trx_expiration_window", BigNumber.decode),
      maxTransactionDelay:
        x |> field("max_transaction_delay", BigNumber.decode),
      maxInlineActionSize:
        x |> field("max_inline_action_size", BigNumber.decode),
      maxInlineActionDepth:
        x |> field("max_inline_action_depth", BigNumber.decode),
      maxAuthorityDepth: x |> field("max_authority_depth", BigNumber.decode),
      maxRamSize: x |> field("max_ram_size", BigNumber.decode),
      totalRamBytesReserved:
        x |> field("total_ram_bytes_reserved", BigNumber.decode),
      totalRamStake: x |> field("total_ram_stake", BigNumber.decode),
      lastProducerScheduleUpdate:
        x |> field("last_producer_schedule_update", BlockTimestamp.decode),
      lastPerVoteBucketFill:
        x |> field("last_pervote_bucket_fill", TimePoint.decode),
      perVoteBucket: x |> field("pervote_bucket", BigNumber.decode),
      perBlockBucket: x |> field("perblock_bucket", BigNumber.decode),
      totalUnpaidBlocks: x |> field("total_unpaid_blocks", BigNumber.decode),
      totalActivatedStake:
        x |> field("total_activated_stake", BigNumber.decode),
      threshActivatedStakeTime:
        x |> field("thresh_activated_stake_time", TimePoint.decode),
      lastProducerScheduleSize:
        x |> field("last_producer_schedule_size", BigNumber.decode),
      totalProducerVoteWeight:
        x |> field("total_producer_vote_weight", BigNumber.decode),
      lastNameClose: x |> field("last_name_close", BlockTimestamp.decode),
    };
};

module ProducerInfo = {
  type t = {
    owner: AccountName.t,
    totalVotes: BigNumber.t,
    producerKey: string,
    isActive: bool,
    url: string,
    unpaidBlocks: BigNumber.t,
    lastClaimTime: TimePoint.t,
    location: int,
  };
  let decode = j =>
    Json.Decode.{
      owner: j |> field("owner", AccountName.decode),
      totalVotes: j |> field("total_votes", BigNumber.decode),
      producerKey: j |> field("producer_key", string),
      isActive: j |> field("is_active", int |> map(x => x === 1)),
      url: j |> field("url", string),
      unpaidBlocks: j |> field("unpaid_blocks", BigNumber.decode),
      lastClaimTime: j |> field("last_claim_time", TimePoint.decode),
      location: j |> field("location", int),
    };
  let encode = d =>
    Json.Encode.(
      object_([
        ("owner", d.owner |> AccountName.encode),
        ("total_votes", d.totalVotes |> BigNumber.encode),
        ("producer_key", d.producerKey |> string),
        ("is_active", int(d.isActive ? 1 : 0)),
        ("url", d.url |> string),
        ("unpaid_blocks", d.unpaidBlocks |> BigNumber.encode),
        ("last_claim_time", d.lastClaimTime |> TimePoint.encode),
        ("location", d.location |> int),
      ])
    );
};

module VoterInfo = {
  type t = {
    owner: AccountName.t,
    proxy: AccountName.t,
    producers: array(AccountName.t),
    staked: BigNumber.t,
    lastVoteWeight: BigNumber.t,
    proxiedVoteWeight: BigNumber.t,
    isProxy: bool,
  };

  let decode = x =>
    Json.Decode.{
      owner: x |> field("owner", AccountName.decode),
      proxy: x |> field("proxy", AccountName.decode),
      producers: x |> field("producers", array(AccountName.decode)),
      staked: x |> field("staked", BigNumber.decode),
      lastVoteWeight: x |> field("last_vote_weight", BigNumber.decode),
      proxiedVoteWeight: x |> field("proxied_vote_weight", BigNumber.decode),
      isProxy: x |> field("is_proxy", int |> map(x => x === 1)),
    };
};

module UserResources = {
  type t = {
    owner: AccountName.t,
    netWeight: Asset.t,
    cpuWeight: Asset.t,
    ramBytes: BigNumber.t,
  };

  let decode = x =>
    Json.Decode.{
      owner: x |> field("owner", AccountName.decode),
      netWeight: x |> field("net_weight", Asset.decode),
      cpuWeight: x |> field("cpu_weight", Asset.decode),
      ramBytes: x |> field("ram_bytes", BigNumber.decode),
    };
};

module DelegatedBandwidth = {
  type t = {
    from: AccountName.t,
    to_: AccountName.t,
    netWeight: Asset.t,
    cpuWeight: Asset.t,
  };
  let decode = x =>
    Json.Decode.{
      from: x |> field("from", AccountName.decode),
      to_: x |> field("to", AccountName.decode),
      netWeight: x |> field("net_weight", Asset.decode),
      cpuWeight: x |> field("cpu_weight", Asset.decode),
    };
};

module Connector = {
  type t = {
    balance: Asset.t,
    weight: BigNumber.t,
  };
};

module ExchangeState = {
  type t = {
    supply: Asset.t,
    base: Connector.t,
    quote: Connector.t,
  };
};

module RefundRequest = {
  type t = {
    owner: AccountName.t,
    requestTime: Js.Date.t,
    netAmount: Asset.t,
    cpuAmount: Asset.t,
  };
};

module NameBidInfo = {
  type t = {
    newName: AccountName.t,
    highBidder: AccountName.t,
    highBid: int,
    lastBidTime: Js.Date.t,
  };
};

let estimateProducerVotePay =
    (producerInfo: ProducerInfo.t, globalState: GlobalState.t) => {
  let perVoteBucket = globalState.perVoteBucket;
  let totalVotes = producerInfo.totalVotes;
  let totalProducerVoteWeight = globalState.totalProducerVoteWeight;
  let producerPerVotePay =
    perVoteBucket
    |. BigNumber.times(totalVotes)
    |. BigNumber.div(totalProducerVoteWeight);
  let minPerVoteDailyPay = 1000000 |. BigNumber.fromInt;
  producerPerVotePay |. BigNumber.lt(minPerVoteDailyPay) ?
    0 |. BigNumber.fromInt : producerPerVotePay |. BigNumber.divInt(10000);
};

let estimateProducerBlockPay =
    (producerInfo: ProducerInfo.t, globalState: GlobalState.t) => {
  let perBlockBucket = globalState.perBlockBucket;
  let unpaidBlocks = producerInfo.unpaidBlocks;
  let totalUnpaidBlocks = globalState.totalUnpaidBlocks;
  perBlockBucket
  |. BigNumber.times(unpaidBlocks)
  |. BigNumber.div(totalUnpaidBlocks)
  |. BigNumber.divInt(10000);
};

let estimateProducerPay =
    (producerInfo: ProducerInfo.t, globalState: GlobalState.t) => {
  let votePay = estimateProducerVotePay(producerInfo, globalState);
  let blockPay = estimateProducerBlockPay(producerInfo, globalState);
  votePay |. BigNumber.plus(blockPay);
};
