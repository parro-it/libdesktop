#include "napi_utils.h"
#include "control.h"

void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    napi_value this=(napi_value)finalize_hint;
    YGNodeRef node = dsk_widget_get_node(env, this);
    YGNodeFree(node);
}

extern napi_ref StyleRef;

void dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this) {
    
    napi_wrap(env, this, widget, widget_finalize, this, NULL);
   
    YGNodeRef node = YGNodeNew();
    YGNodeStyleSetWidth(node,50);
    YGNodeStyleSetHeight(node,50);
    dsk_widget_set_node(env, this, node);
    YGNodeSetContext(node, widget);
    

    napi_value constructor;
    napi_value style;
    napi_value null;

    napi_get_reference_value(env, StyleRef, &constructor);
    napi_get_null(env,&null);
    napi_new_instance(env,constructor,2,(napi_value[]){null,this},&style);
    napi_set_named_property(env, this, "style", style);
}
