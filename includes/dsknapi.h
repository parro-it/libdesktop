#ifndef DSK_NAPI_H
#define DSK_NAPI_H 1 
    
#define DSK_NAPI_CALL(FN) do {                                    \
    napi_status status = FN;                                      \
    if (status != napi_ok) {                                      \
        const napi_extended_error_info *err;                      \
        napi_get_last_error_info(env, &err);                      \
        napi_throw_error(env, NULL, err->error_message);          \
        goto dsk_error;                                           \
    }                                                             \
} while(0)

#endif