#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>
#include <assert.h>

#include "libdesktop.h"
#define MODULE "label"


bool dsk_set_properties(napi_env env, napi_value props, napi_value target) {
    napi_value names;
    DSK_NAPI_CALL(napi_get_property_names(env, props, &names));
    
    uint32_t len;
    DSK_NAPI_CALL(napi_get_array_length(env, names, &len));

    for (uint32_t i=0; i < len; i++) {
        napi_value idx;
        napi_value propName;
        bool hasProp;

        DSK_NAPI_CALL(napi_create_uint32(env,i,&idx));
        DSK_NAPI_CALL(napi_get_property(env,names,idx,&propName));

        size_t len;
        DSK_NAPI_CALL(napi_get_value_string_utf8(env,propName,NULL,0,&len));
        char propName_s[len+1];
        DSK_NAPI_CALL(napi_get_value_string_utf8(env,propName,propName_s,len+1,NULL));

        DSK_NAPI_CALL(napi_has_property(env,target, propName, &hasProp));
        if (hasProp) {
            napi_value propValue;
            DSK_NAPI_CALL(napi_get_property(env, props, propName, &propValue));
            
            napi_valuetype type;
            DSK_NAPI_CALL(napi_typeof(env, propValue, &type));

            if (type == napi_object) {
                napi_value styleProp;
                printf("recurse on %s\n",propName_s);
                DSK_NAPI_CALL(napi_get_property(env, target, propName, &styleProp));
                if (dsk_set_properties(env, propValue, styleProp)) {
                    goto dsk_error;
                }
                continue;
            }    
            
            printf("set property %s\n",propName_s);

            DSK_NAPI_CALL(napi_set_property(env, target, propName, propValue));
        }
    }
    return false;
    dsk_error:
        return true;

}

LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

   	GtkWidget* widget = gtk_label_new("");
    GdkRGBA color; 
    gdk_rgba_parse (&color, "rgba(100,90,80,1)");
    gtk_widget_override_background_color(widget,GTK_STATE_FLAG_NORMAL, &color);
    dsk_wrap_widget(env,widget,this);

    if (dsk_set_properties(env, argv[0], this)) {
        napi_throw_error(env,NULL,"Error while setting widget properties.\n");
        return NULL;
    }

    
    return this;
}

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
       DSK_RWPROP_S(label),
       (napi_property_descriptor){.utf8name="setLabel", .method=widgetSetPropS,.data="label"},
       DSK_CHILDPROP_I32(left,"x"),
       DSK_CHILDPROP_I32(top,"y"),
    }));

    return exports;
}

