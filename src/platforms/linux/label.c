#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

#define MODULE "label"


static void label_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

    printf("LABEL NEW\n");
   	GtkWidget* widget = gtk_label_new("Hello guys & gals.");
    napi_status status = napi_wrap(env, this, widget, label_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    return this;
}

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    

    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
    }));

    return exports;
}

