#include "__widget.h"
#include "napi_utils.h"

#include <gtk/gtk.h>

#include <assert.h>
#include <yoga/Yoga.h>

#include "libdesktop.h"
#define MODULE "label"

LIBUI_FUNCTION(labelNew) {
	INIT_ARGS(2);

	GtkWidget *widget = gtk_label_new("");

	// GdkRGBA color;
	// gdk_rgba_parse (&color, "rgba(100,90,80,1)");
	// gtk_widget_override_background_color(widget,GTK_STATE_FLAG_NORMAL, &color);
	dsk_wrap_widget(env, widget, this);
	gtk_label_set_xalign(GTK_LABEL(widget), GTK_ALIGN_END);

	napi_value events;
	DSK_NAPI_CALL(napi_create_object(env, &events));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "events", events));

	napi_value click = dsk_event_new_for_widget(env, "click", this);
	DSK_NAPI_CALL(napi_set_named_property(env, events, "click", click));

	if (dsk_set_properties(env, argv[0], this)) {
		napi_throw_error(env, NULL, "Error while setting widget properties.\n");
		return NULL;
	}

	return this;
}

// napi_value dsk_define_class(napi_env env, const char* name, napi_callback constructor,const
// napi_property_descriptor properties[], size_t propertiesCount,napi_ref* ref);

napi_value label_init(napi_env env, napi_value exports) {
	DEFINE_MODULE()

	dsk_define_class(env, module, "Label", labelNew,
					 ((napi_property_descriptor[]){
						 DSK_RWPROP_BOOL(visible, "visible"),
						 // DSK_RWPROP_BOOL(visible,"enabled"),
						 DSK_RWPROP_S(text, "label"),
						 (napi_property_descriptor){
							 .utf8name = "setLabel", .method = widgetSetPropS, .data = "label"},
						 DSK_CHILDPROP_I32(left, "x"),
						 DSK_CHILDPROP_I32(top, "y"),
					 }));

	return exports;
}
