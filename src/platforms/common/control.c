#include "napi_utils.h"
#include "widget.h"

napi_value _dsk_define_class(napi_env env, napi_value exports, const char* name, napi_callback constructor,const napi_property_descriptor properties[], size_t propertiesCount,napi_ref* ref) {

    napi_status status;
    napi_value Class;
    printf("PROPERTIES %ld\n",propertiesCount);
    status = napi_define_class(env, name, NAPI_AUTO_LENGTH, constructor, NULL, propertiesCount, properties, &Class);
    CHECK_STATUS_THROW(status, napi_define_class);                                          
	
    
    napi_ref ignored_ref;
    if (ref == NULL) {
        ref = &ignored_ref;
    }

    status = napi_create_reference(env, Class, 1, ref);
    CHECK_STATUS_THROW(status, napi_create_reference);                                          

    status = napi_set_named_property(env, exports, name, Class);                                  
	CHECK_STATUS_THROW(status, napi_set_named_property);   

    return exports;
}

YGNodeRef dsg_widget_get_node(napi_env env, napi_value widget);
void dsg_widget_set_node(napi_env env, napi_value widget, YGNodeRef node);


static void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    napi_value this=(napi_value)finalize_hint;
    YGNodeRef node = dsg_widget_get_node(env, this);
    YGNodeFree(node);
}

extern napi_ref StyleRef;

void dsk_wrap_widget(napi_env env, void* widget, napi_value this) {
    
    napi_status status = napi_wrap(env, this, widget, widget_finalize, this, NULL);
   
    YGNodeRef node = YGNodeNew();
    dsg_widget_set_node(env, this, node);
    YGNodeSetContext(node, widget);


    napi_value constructor;
    napi_value style;
    napi_value null;

    status = napi_get_reference_value(env, StyleRef, &constructor);
    napi_get_null(env,&null);
    status = napi_new_instance(env,constructor,2,(napi_value[]){null,this},&style);
    napi_set_named_property(env, this, "style", style);
}