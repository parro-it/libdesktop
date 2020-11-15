#include "libdesktop.h"
#include <gtk/gtk.h>

void dsk_initui_for_test() {
	gtk_init(0, NULL);
}

void *dsk_new_test_widget() {
	return gtk_label_new("this is a test control");
}

napi_status dsk_CtrlI_link_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	g_object_set_data(G_OBJECT(UI_ctrl), "DskCtrlI", ctrl);
	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	*ctrl = g_object_get_data(G_OBJECT(UI_ctrl), "DskCtrlI");
	return napi_ok;
}

napi_status dsk_platform_get_prop_t(struct DskCtrlI *self, const char *prop_name,
									void **prop_value) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_set_prop_t(struct DskCtrlI *self, const char *prop_name,
									void *prop_value) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height) {
	napi_env env = self->env;
	UIHandle widget = self->ctrl_handle;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	int minwidth, natwidth;
	int minheight, natheight;
	gtk_widget_set_size_request(widget, 0, 0);
	gtk_widget_get_preferred_height(widget, &minheight, &natheight);
	gtk_widget_get_preferred_width(widget, &minwidth, &natwidth);

	*width = natwidth;
	*height = natheight;

	return napi_ok;
}

napi_status dsk_platform_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	GtkWidget *widget = GTK_WIDGET(self->ctrl_handle);
	GtkContainer *container = gtk_widget_get_parent(widget);

	GValue x = G_VALUE_INIT;
	g_value_init(&x, G_TYPE_INT);
	g_value_set_int(&x, x);

	GValue y = G_VALUE_INIT;
	g_value_init(&y, G_TYPE_INT);
	g_value_set_int(&y, y);

	gtk_container_child_set_property(container, widget, "x", &x);
	gtk_container_child_set_property(container, widget, "y", &y);
	gtk_widget_set_size_request(widget, (int)width, (int)height);

	return napi_ok;
}

#include "dsktest.h"
napi_status new_wrapped_Ctrl(napi_env env, DskCtrlI **ctrl, UIHandle *widget, napi_value *wrapper);

DSK_DEFINE_TEST(tests_dsk_platform_get_preferred_size_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	int width, height;
	GtkWindow *window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(widget));
	gtk_widget_show_all(GTK_WIDGET(window));

	dsk_platform_get_preferred_size_t(ctrl, &width, &height);
	printf("%d x %d\n", width, height);
	DSK_ASSERT(width == 129);
	DSK_ASSERT(height == 17);

	gtk_window_close(window);

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE

napi_status dsk_platform_add_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_remove_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}