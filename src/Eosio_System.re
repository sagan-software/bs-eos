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
    lastProducerScheduleUpdate: Js.Date.t,
    lastPerVoteBucketFill: Js.Date.t,
    perVoteBucket: int,
    perBlockBucket: int,
    totalUnpaidBlocks: int,
    totalActivatedStake: BigNumber.t,
    threshActivatedStakeTime: Js.Date.t,
    lastProducerScheduleSize: int,
    totalProducerVoteWeight: BigNumber.t,
    lastNameClose: Js.Date.t,
  };
};

module ProducerInfo = {
  type t = {
    owner: Eos.AccountName.t,
    totalVotes: BigNumber.t,
    producerKey: string,
    isActive: bool,
    url: string,
    unpaidBlocks: int,
    lastClaimTime: Js.Date.t,
    location: int,
  };
  let decode = j =>
    Json.Decode.{
      owner: j |> field("owner", Eos.AccountName.decode),
      totalVotes: j |> field("total_votes", BigNumber.decodeAny),
      producerKey: j |> field("producer_key", string),
      isActive: j |> field("is_active", int |> map(x => x === 1)),
      url: j |> field("url", string),
      unpaidBlocks: j |> field("unpaid_blocks", int),
      lastClaimTime:
        j
        |> field(
             "last_claim_time",
             either(Json.Decode.float, string |> map(float_of_string))
             |> map(us => us /. 1000. |> Js.Date.fromFloat),
           ),
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
        (
          "last_claim_time",
          (d.lastClaimTime |> Js.Date.getTime) *. 1000. |> int_of_float |> int,
        ),
        ("location", d.location |> int),
      ])
    );
};

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
