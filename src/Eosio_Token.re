module Transfer = {
  type t;
};

module Create = {
  type t;
};

module Issue = {
  type t;
};

module Account = {
  type t = {balance: Eos.Asset.t};
};

module CurrencyStats = {
  type t = {
    supply: Eos.Asset.t,
    maxSupply: Eos.Asset.t,
    issuer: Eos.AccountName.t,
  };
};
