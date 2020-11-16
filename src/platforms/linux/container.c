#include "libdesktop.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>

DSK_EXTEND_MODULE(libdesktop);
/*
void dsk_widget_reposition(napi_env env, UIHandle container, UIHandle widget, float xcoord,
						   float ycoord, float width, float height) {
	GValue x = G_VALUE_INIT;
	g_value_init(&x, G_TYPE_INT);
	g_value_set_int(&x, xcoord);
	GValue y = G_VALUE_INIT;
	g_value_init(&y, G_TYPE_INT);
	g_value_set_int(&y, ycoord);
	gtk_container_child_set_property(container, widget, "x", &x);
	gtk_container_child_set_property(container, widget, "y", &y);
	gtk_widget_set_size_request(widget, (int)width, (int)height);
}

void dsk_platform_container_add_child(UIHandle parent, UIHandle child) {
	gtk_container_add(GTK_CONTAINER(parent), GTK_WIDGET(child));
}
*/
DSK_DEFINE_CLASS(libdesktop, Container) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	GtkWidget *widget = gtk_fixed_new();
	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this, argv));

	return this;
}
/*

DSK_DEFINE_METHOD(libdesktop, Container, append) {
	DSK_JS_FUNC_INIT();

	// GtkWidget *widget;
	// DSK_UNWRAP_WIDGET();

	return this;
}
napi_ref ContainerRef;

napi_value container_init(napi_env env, napi_value exports) {
	DEFINE_MODULE()

	dsk_define_class_ref(env, module, "Container", containerNew,
						 ((napi_property_descriptor[]){
							 DSK_RWPROP_BOOL(visible, "visible"),
							 DSK_RWPROP_BOOL(enabled, "enabled"),
						 }),
						 &ContainerRef);

	return exports;
}
*/