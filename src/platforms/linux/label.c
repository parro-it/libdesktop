#include "libdesktop.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <yoga/Yoga.h>

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Label) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	GtkWidget *widget = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(widget), GTK_ALIGN_END);

	// GdkRGBA color;
	// gdk_rgba_parse (&color, "rgba(100,90,80,1)");
	// gtk_widget_override_background_color(widget,GTK_STATE_FLAG_NORMAL, &color);
	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this, argv));

	napi_value events;
	DSK_NAPI_CALL(napi_get_named_property(env, this, "events", &events));

	napi_value click = dsk_event_new_for_widget(env, "click", this);
	DSK_NAPI_CALL(napi_set_named_property(env, events, "click", click));

	return this;
}

DSK_UI_PROP(libdesktop, Label, left, dsk_prop_i32, "x");
DSK_UI_PROP(libdesktop, Label, top, dsk_prop_i32, "y");
DSK_UI_PROP(libdesktop, Label, text, dsk_prop_str, "label");
DSK_UI_PROP(libdesktop, Label, visible, dsk_prop_bool, "visible");
