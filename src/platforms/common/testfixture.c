#include "libdesktop.h"

DSK_JS_FUNC(sum) {
    DSK_JS_FUNC_INIT();
    
    double a,b;
    napi_value result;

    DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&a));
    DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&b));
    DSK_NAPI_CALL(napi_create_double(env,a+b,&result));

    return result;
}

DSK_JS_FUNC(sum_at_least_two) {
    DSK_JS_FUNC_INIT();
    DSK_AT_LEAST_NARGS(2);
    
    double a,b;
    napi_value result;

    DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&a));
    DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&b));
    DSK_NAPI_CALL(napi_create_double(env,a+b,&result));

    return result;
}


DSK_JS_FUNC(sum_exactly_two) {
    DSK_JS_FUNC_INIT();
    DSK_EXACTLY_NARGS(2);
    
    double a,b;
    napi_value result;

    DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&a));
    DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&b));
    DSK_NAPI_CALL(napi_create_double(env,a+b,&result));

    return result;
}

#define DSK_FUNC_DESCR_WITH_NAME(FN, NAME) ((napi_property_descriptor){       \
    .method = FN,                                                               \
    .utf8name = NAME,                                                          \
    .attributes = napi_enumerable,                                             \
})                                                                        \

#define DSK_FUNC_DESCR(FN) DSK_FUNC_DESCR_WITH_NAME(FN, #FN)


DSK_MODINIT_FUNC(init_fixture_module) {
	DSK_ONERROR_THROW_RET(NULL);
    DSK_NAPI_CALL(napi_define_properties(env,exports,3,((napi_property_descriptor[]) {
        DSK_FUNC_DESCR(sum),
        DSK_FUNC_DESCR(sum_exactly_two),
        DSK_FUNC_DESCR(sum_at_least_two),
    })));
	return exports;
}