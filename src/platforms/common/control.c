#include "napi_utils.h"
#include "control.h"
#include <assert.h>

void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    napi_value this=(napi_value)finalize_hint;
    YGNodeRef node = dsk_widget_get_node(env, this);
    YGNodeFree(node);
}

extern napi_ref StyleRef;

void dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this) {
    assert(widget != NULL);
    assert(this != NULL);
    // printf("1\n");
    napi_status  status = napi_wrap(env, this, widget, widget_finalize, this, NULL);
    assert(status == napi_ok);
    // printf("2\n");
    YGNodeRef node = YGNodeNew();
    
    
    // printf("4 node %p\n", node);
    dsk_widget_set_node(env, this, node);
    // printf("5\n");
    YGNodeSetContext(node, widget);
    // printf("6\n");

    napi_value constructor;
    napi_value style;
    napi_value null;

    status = napi_get_reference_value(env, StyleRef, &constructor);
    assert(status == napi_ok);
    // printf("7\n");
    status = napi_get_null(env,&null);
    assert(status == napi_ok);
    // printf("8 %p %p %p\n",constructor,this,null);
    napi_value args[2] = {null,this};
    status = napi_new_instance(env,constructor,2,args,&style);
    assert(status == napi_ok);
    // printf("9\n");
    status = napi_set_named_property(env, this, "style", style);
    assert(status == napi_ok);
    // printf("10\n");
}
