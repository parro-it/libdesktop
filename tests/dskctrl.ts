import test from 'tape-async'
const {NativeTests} = require("../build/Release/desktop.node")
console.log({NativeTests})

test('dsk_CtrlIFuncs_mk_default', NativeTests.tests_dsk_CtrlIFuncs_mk_default);
test('dsk_CtrlI_from_UIHandle',NativeTests.tests_dsk_CtrlI_from_UIHandle);
test('dsk_CtrlI_from_wrapper',NativeTests.tests_dsk_CtrlI_from_wrapper);