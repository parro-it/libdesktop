#include "napi_utils.h"
#include "widget.h"

#define MODULE "label"

napi_value label_init(napi_env env, napi_value exports) {
    /*DEFINE_MODULE()
    

    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
    }));
    */
    return exports;
}