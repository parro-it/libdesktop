#ifndef DSK_TEST_H__
#define DSK_TEST_H__ 1

#include "dsknapi.h"

DSK_EXTEND_MODULE(libdesktop);

DSK_EXTEND_CLASS(libdesktop, NativeTests);

#define DSK_ASSERT(ASSERTION) DSK_NAPI_CALL(dsk_assert(env, t, ASSERTION))
#define DSK_END_TEST() DSK_NAPI_CALL(dsk_end_test(env, t))

#define DSK_TEST_CLOSE                                                                             \
	return NULL;                                                                                   \
	}

#define DSK_DEFINE_TEST(NAME)                                                                      \
	DSK_DEFINE_STATIC_METHOD(libdesktop, NativeTests, NAME) {                                      \
		DSK_JS_FUNC_INIT();                                                                        \
		DSK_EXACTLY_NARGS(1);                                                                      \
		napi_value t = argv[0];

void *dsk_new_test_widget();

// test functions
void dsk_initui_for_test();

napi_status dsk_assert(napi_env env, napi_value t, bool assertion);
napi_status dsk_end_test(napi_env env, napi_value t);
#endif