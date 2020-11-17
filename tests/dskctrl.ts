import test from 'tape-async'
const {NativeTests} = require("../build/Release/desktop.node")
/*console.log({NativeTests})*/

test('dsk_CtrlIFuncs_init', NativeTests.tests_dsk_CtrlIFuncs_init);
test('dsk_CtrlI_from_UIHandle',NativeTests.tests_dsk_CtrlI_from_UIHandle);
test('dsk_CtrlI_from_wrapper',NativeTests.tests_dsk_CtrlI_from_wrapper);
test('dsk_CtrlI_get_wrapper',NativeTests.tests_dsk_CtrlI_get_wrapper);
test('def_add_children_t',NativeTests.tests_def_add_children_t);
test('def_add_children_t',NativeTests.tests_def_add_children_t);
test('dsk_platform_get_preferred_size_t',NativeTests.tests_dsk_platform_get_preferred_size_t);
test('DskLayoutContainerProto',NativeTests.tests_DskLayoutContainerProto);
test('DskRootContainerProto',NativeTests.tests_DskRootContainerProto);
test('dsk_platform_set_prop_t',NativeTests.tests_dsk_platform_set_prop_t);

test('def_assign_props_t',NativeTests.tests_def_assign_props_t);
