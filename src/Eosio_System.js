// Generated by BUCKLESCRIPT VERSION 4.0.0, PLEASE EDIT WITH CARE
'use strict';

var BigNumber = require("@sagan-software/bs-bignumber/src/BigNumber.js");
var Eos_Types = require("./Eos_Types.js");
var Caml_format = require("bs-platform/lib/js/caml_format.js");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");

var GlobalState = /* module */[];

function decode(j) {
  var partial_arg = Json_decode.either(Json_decode.$$float, (function (param) {
          return Json_decode.map(Caml_format.caml_float_of_string, Json_decode.string, param);
        }));
  return /* record */[
          /* owner */Json_decode.field("owner", Eos_Types.AccountName[/* decode */0], j),
          /* totalVotes */Json_decode.field("total_votes", BigNumber.decodeAny, j),
          /* producerKey */Json_decode.field("producer_key", Json_decode.string, j),
          /* isActive */Json_decode.field("is_active", (function (param) {
                  return Json_decode.map((function (x) {
                                return x === 1;
                              }), Json_decode.$$int, param);
                }), j),
          /* url */Json_decode.field("url", Json_decode.string, j),
          /* unpaidBlocks */Json_decode.field("unpaid_blocks", Json_decode.$$int, j),
          /* lastClaimTime */Json_decode.field("last_claim_time", (function (param) {
                  return Json_decode.map((function (us) {
                                return new Date(us / 1000);
                              }), partial_arg, param);
                }), j),
          /* location */Json_decode.field("location", Json_decode.$$int, j)
        ];
}

function encode(d) {
  var match = d[/* isActive */3];
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "owner",
                Eos_Types.AccountName[/* encode */1](d[/* owner */0])
              ],
              /* :: */[
                /* tuple */[
                  "total_votes",
                  BigNumber.encode(d[/* totalVotes */1])
                ],
                /* :: */[
                  /* tuple */[
                    "producer_key",
                    d[/* producerKey */2]
                  ],
                  /* :: */[
                    /* tuple */[
                      "is_active",
                      match ? 1 : 0
                    ],
                    /* :: */[
                      /* tuple */[
                        "url",
                        d[/* url */4]
                      ],
                      /* :: */[
                        /* tuple */[
                          "unpaid_blocks",
                          d[/* unpaidBlocks */5]
                        ],
                        /* :: */[
                          /* tuple */[
                            "last_claim_time",
                            d[/* lastClaimTime */6].getTime() * 1000 | 0
                          ],
                          /* :: */[
                            /* tuple */[
                              "location",
                              d[/* location */7]
                            ],
                            /* [] */0
                          ]
                        ]
                      ]
                    ]
                  ]
                ]
              ]
            ]);
}

var ProducerInfo = /* module */[
  /* decode */decode,
  /* encode */encode
];

var VoterInfo = /* module */[];

var UserResources = /* module */[];

var DelegatedBandwidth = /* module */[];

var Connector = /* module */[];

var ExchangeState = /* module */[];

var RefundRequest = /* module */[];

var NameBidInfo = /* module */[];

exports.GlobalState = GlobalState;
exports.ProducerInfo = ProducerInfo;
exports.VoterInfo = VoterInfo;
exports.UserResources = UserResources;
exports.DelegatedBandwidth = DelegatedBandwidth;
exports.Connector = Connector;
exports.ExchangeState = ExchangeState;
exports.RefundRequest = RefundRequest;
exports.NameBidInfo = NameBidInfo;
/* BigNumber Not a pure module */