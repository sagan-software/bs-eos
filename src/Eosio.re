module System = Eosio_System;
module Token = Eosio_Token;

let getGlobalState =
  Eos.getTableSingleton(
    ~rowDecoder=System.GlobalState.decode,
    ~code=Eos.accountName("eosio"),
    ~scope=Eos.accountName("eosio"),
    ~table=Eos.tableName("global"),
  );

let getProducers =
  Eos.getTableRows(
    ~code=Eos.accountName("eosio"),
    ~scope=Eos.accountName("eosio"),
    ~table=Eos.tableName("producers"),
    ~rowDecoder=System.ProducerInfo.decode,
  );

let getVoters =
  Eos.getTableRows(
    ~code=Eos.accountName("eosio"),
    ~scope=Eos.accountName("eosio"),
    ~table=Eos.tableName("voters"),
    ~rowDecoder=System.VoterInfo.decode,
  );
