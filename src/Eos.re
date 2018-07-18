include Eos_Types;
include Eos_Chain;
module Js = Eos_Js;

let make = Js.make;
let getInfo = Js.getInfo;
let getCode = Js.getCode;
let getTableRows = Js.getTableRows;
let getTableSingleton = Js.getTableSingleton;

let accountName = AccountName.fromString;
let permissionName = PermissionName.fromString;
let tokenName = TokenName.fromString;
let tableName = TableName.fromString;
let scopeName = ScopeName.fromString;
let actionName = ActionName.fromString;
let typeName = TypeName.fromString;
