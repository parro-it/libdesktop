#include "libdesktop.h"
#include <assert.h>
#include <gtk/gtk.h>
#include <yoga/Yoga.h>

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Textfield) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	GtkWidget *widget = gtk_entry_new();
	/////
	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this, argv));
	gtk_widget_add_events(widget, GDK_BUTTON_PRESS_MASK);

	napi_value events;
	DSK_NAPI_CALL(napi_get_named_property(env, this, "events", &events));

	napi_value click = dsk_event_new_for_widget(env, "activate", this);
	DSK_NAPI_CALL(napi_set_named_property(env, events, "click", click));

	return this;
}

DSK_UI_PROP_I32(libdesktop, Textfield, left, "x");
DSK_UI_PROP_I32(libdesktop, Textfield, top, "y");
DSK_UI_PROP_S(libdesktop, Textfield, text, "text");
DSK_UI_PROP_BOOL(libdesktop, Textfield, visible, "visible");
