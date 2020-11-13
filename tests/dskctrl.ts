import test from 'tape-async'
const {NativeTests} = require("../build/Release/desktop.node")
console.log({NativeTests})
test('dsk_CtrlIFuncs_mk_default', NativeTests.tests_dsk_CtrlIFuncs_mk_default);
