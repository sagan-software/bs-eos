open Jest;
open Expect;
open! Expect.Operators;

let timeout = 10000;

describe("Asset", () => {
  test("fromString/toString", () =>
    "10.0000 SYS"
    |> Eos.Asset.fromString
    |> Eos.Asset.toString
    |> expect
    |> toBe("10.0000 SYS")
  );

  test("amount", () =>
    "10.0000 EOS"
    |> Eos.Asset.fromString
    |> Eos.Asset.amount
    |> expect
    |> toBe("10.0000")
  );

  test("precision", () =>
    "10.0000 EOS"
    |> Eos.Asset.fromString
    |> Eos.Asset.precision
    |> expect
    |> toBe(4)
  );

  test("symbol", () =>
    "10.0000 EOS"
    |> Eos.Asset.fromString
    |> Eos.Asset.symbol
    |> expect
    |> toBe("EOS")
  );

  test("decode", () =>
    "10.0000 EOS"
    |> Js.Json.string
    |> Eos.Asset.decode
    |> Eos.Asset.toString
    |> expect
    |> toBe("10.0000 EOS")
  );

  test("encode", () =>
    "10.0000 EOS"
    |> Eos.Asset.fromString
    |> Eos.Asset.encode
    |> expect
    |> toBe(Js.Json.string("10.0000 EOS"))
  );
});

let publicKey = "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV";
let privateKey = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3";

describe("PublicKey", () => {
  test("isValid with valid key", () =>
    publicKey |. Eos.PublicKey.isValidString |. expect |> toBe(true)
  );
  test("isValid with invalid key", () =>
    "test" |. Eos.PublicKey.isValidString |. expect |> toBe(false)
  );
});

describe("PrivateKey", () => {
  test("isValid with valid key", () =>
    privateKey |. Eos.PrivateKey.isValidString |. expect |> toBe(true)
  );
  test("isValid with invalid key", () =>
    "test" |. Eos.PrivateKey.isValidString |. expect |> toBe(false)
  );
  testPromise("random", ~timeout, () =>
    Eos.PrivateKey.random()
    |> Js.Promise.then_(key =>
         key
         |. Eos.PrivateKey.isValid
         |. expect
         |> toBe(true)
         |. Js.Promise.resolve
       )
  );
  test("toPublic", () =>
    privateKey
    |. Eos.PrivateKey.fromString
    |. Eos.PrivateKey.toPublic
    |. Eos.PublicKey.toString
    |. expect
    |> toBe(publicKey)
  );
});

describe("AccountName", () =>
  describe("resultFromString", () => {
    test("empty names", () =>
      ""
      |. Eos.AccountName.resultFromString
      |. expect
      |> toEqual(Belt.Result.Error("Empty string is not a name"))
    );
    test("all dots", () =>
      ".."
      |. Eos.AccountName.resultFromString
      |. expect
      |> toEqual(
           Belt.Result.Error("Names must include characters other than dots"),
         )
    );
    test("capital letters", () =>
      "helloWORLD"
      |. Eos.AccountName.resultFromString
      |. expect
      |> toEqual(Belt.Result.Error("Invalid character: 'W'"))
    );

    test("more than 12 characters", () =>
      "1234512345123"
      |. Eos.AccountName.resultFromString
      |. expect
      |> toEqual(Belt.Result.Error("A name can be up to 12 characters long"))
    );

    test("numbers greater than 5", () =>
      "qwerty6"
      |. Eos.AccountName.resultFromString
      |. expect
      |> toEqual(Belt.Result.Error("Invalid character: '6'"))
    );

    test("valid name", () =>
      "hello"
      |. Eos.AccountName.resultFromString
      |. expect
      |> toEqual(Belt.Result.Ok("hello" |. Eos.AccountName.fromString))
    );
  })
);

let httpEndpoint = "http://api.eosnewyork.io";
let chainId = "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906";

let eos = Eos.make(~httpEndpoint, ~chainId, ());

describe("Info", () =>
  testPromise("get", ~timeout, () =>
    eos
    |. Eos.getInfo
    |> Js.Promise.then_((info: Eos.Info.t) =>
         info.headBlockNum
         |> expect
         |> toBeGreaterThan(6287534)
         |> Js.Promise.resolve
       )
  )
);

let lazyDecoder = (json: Js.Json.t) => json;

describe("TableRows", () =>
  testPromise("get", ~timeout, () =>
    eos
    |. Eos.getTableRows(
         ~rowDecoder=lazyDecoder,
         ~code=Eos.accountName("eosio"),
         ~scope=Eos.accountName("eosio"),
         ~table=Eos.tableName("producers"),
         (),
       )
    |> Js.Promise.then_((tableRows: Eos.TableRows.t('asdf)) =>
         tableRows.rows
         |> Js.Array.length
         |> expect
         |> toBeGreaterThan(0)
         |> Js.Promise.resolve
       )
  )
);

describe("Code", () =>
  testPromise("getCode", ~timeout, () =>
    eos
    |. Eos.getCode(~accountName=Eos.accountName("eosio"))
    |> Js.Promise.then_((code: Eos.Code.t) =>
         (
           code.accountName |. Eos.AccountName.toString,
           code.abi |. Belt.Option.isSome,
         )
         |. expect
         |> toEqual(("eosio", true))
         |. Js.Promise.resolve
       )
  )
);

describe("BlockTimestamp", () => {
  let json = "2018-07-17T19:47:20.500" |. Js.Json.string;

  test("decode", () =>
    json
    |. Eos.BlockTimestamp.decode
    |. Eos.BlockTimestamp.toDate
    |. Js.Date.toISOString
    |. expect
    |> toEqual("2018-07-17T19:47:20.500Z")
  );

  test("encode", () =>
    json
    |. Eos.BlockTimestamp.decode
    |. Eos.BlockTimestamp.encode
    |. expect
    |> toEqual(json)
  );
});

describe("TimePoint", () => {
  let json = "1529459862123456" |. Js.Json.string;
  test("decode", () =>
    json
    |. Eos.TimePoint.decode
    |. Eos.TimePoint.toDate
    |. Js.Date.toISOString
    |. expect
    |> toEqual("2018-06-20T01:57:42.123Z")
  );
  test("encode", () =>
    json
    |. Eos.TimePoint.decode
    |. Eos.TimePoint.encode
    |. expect
    |> toEqual(json)
  );
});
