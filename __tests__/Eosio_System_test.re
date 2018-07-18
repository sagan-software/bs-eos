open Jest;
open Expect;
open! Expect.Operators;

let timeout = 10000;
let httpEndpoint = "http://api.eosnewyork.io";
let chainId = "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906";
let eos = Eos.make(~httpEndpoint, ~chainId, ());

describe("Eosio.System", () => {
  testPromise("getProducers", ~timeout, () =>
    eos
    |. Eosio.System.getProducers(~limit=50, ())
    |> Js.Promise.then_(
         (producers: Eos.TableRows.t(Eosio.System.ProducerInfo.t)) =>
         producers.rows
         |. Js.Array.length
         |. expect
         |> toBe(50)
         |. Js.Promise.resolve
       )
  );

  testPromise("getGlobalState", ~timeout, () =>
    eos
    |. Eosio.System.getGlobalState()
    |> Js.Promise.then_((globalState: option(Eosio.System.GlobalState.t)) =>
         (
           switch (globalState) {
           | Some(globalState) =>
             globalState.perVoteBucket |. expect |> toBeGreaterThan(100)
           | None => fail("global state is empty")
           }
         )
         |. Js.Promise.resolve
       )
  );
});
