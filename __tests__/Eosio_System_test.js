// Generated by BUCKLESCRIPT VERSION 4.0.0, PLEASE EDIT WITH CARE
'use strict';

var Eos = require("../src/Eos.js");
var Jest = require("@glennsl/bs-jest/src/jest.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Eosio_System = require("../src/Eosio_System.js");

var httpEndpoint = "http://api.eosnewyork.io";

var chainId = "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906";

var eos = Eos.make(httpEndpoint, undefined, undefined, chainId, undefined, undefined, undefined, /* () */0);

describe("Eosio.System", (function () {
        Jest.testPromise(10000, "getProducers", (function () {
                return Curry._6(Eosio_System.getProducers(eos), undefined, undefined, undefined, undefined, 50, /* () */0).then((function (producers) {
                              return Promise.resolve(Jest.Expect[/* toBe */2](50, Jest.Expect[/* expect */0](producers[/* rows */0].length)));
                            }));
              }));
        return Jest.testPromise(10000, "getGlobalState", (function () {
                      return Curry._2(Eosio_System.getGlobalState(eos), undefined, /* () */0).then((function (globalState) {
                                    return Promise.resolve(globalState !== undefined ? Jest.Expect[/* toBeGreaterThan */5](100, Jest.Expect[/* expect */0](globalState[/* perVoteBucket */22])) : Jest.fail("global state is empty"));
                                  }));
                    }));
      }));

var timeout = 10000;

exports.timeout = timeout;
exports.httpEndpoint = httpEndpoint;
exports.chainId = chainId;
exports.eos = eos;
/* eos Not a pure module */
