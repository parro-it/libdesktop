#include "napi_utils.h"
#include "widget.h"
#include "control.h"

#include <yoga.h>
#include <assert.h>

#include "libdesktop.h"
#define MODULE "textfield"



LIBUI_FUNCTION(textfieldNew) {
    /*INIT_ARGS(2);

   	GtkWidget* widget = gtk_entry_new();
    dsk_wrap_widget(env,widget,this);
    gtk_widget_add_events(widget,GDK_BUTTON_PRESS_MASK);

    napi_value events;
    DSK_NAPI_CALL(napi_create_object(env,&events));
    DSK_NAPI_CALL(napi_set_named_property(env,this,"events",events));
    napi_value click = dsk_event_new_for_widget(env, "activate", this);
    DSK_NAPI_CALL(napi_set_named_property(env,events,"click",click));
    
    if (dsk_set_properties(env, argv[0], this)) {
        napi_throw_error(env,NULL,"Error while setting widget properties.\n");
        return NULL;
    }
    */
    return NULL;
}

napi_value textfield_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    

    dsk_define_class(env,module,"Textfield",textfieldNew,NULL);

    return exports;
}

