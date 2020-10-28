#include "libdesktop.h"

DSK_DEFINE_MODULE(libdesktop)

DSK_DEFINE_FUNCTION(libdesktop, sum) {
    DSK_JS_FUNC_INIT();
    
    double a,b;
    napi_value result;

    DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&a));
    DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&b));
    DSK_NAPI_CALL(napi_create_double(env,a+b,&result));

    return result;
}

DSK_DEFINE_FUNCTION(libdesktop, sum_at_least_two) {
    DSK_JS_FUNC_INIT();
    DSK_AT_LEAST_NARGS(2);
    
    double a,b;
    napi_value result;

    DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&a));
    DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&b));
    DSK_NAPI_CALL(napi_create_double(env,a+b,&result));

    return result;
}


DSK_DEFINE_FUNCTION(libdesktop, sum_exactly_two) {
    DSK_JS_FUNC_INIT();
    DSK_EXACTLY_NARGS(2);
    
    double a,b;
    napi_value result;

    DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&a));
    DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&b));
    DSK_NAPI_CALL(napi_create_double(env,a+b,&result));

    return result;
}
