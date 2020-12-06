#include "libdesktop.h"
#include <gtk/gtk.h>
#include <stdarg.h>

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
	GtkContainer *container = GTK_CONTAINER(gtk_widget_get_parent(widget));

	// printf("dsk_platform_reposition_t %d:%d %dx%d\n", x, y, width, height);

	if (x != -1 || y != -1) {
		GValue gx = G_VALUE_INIT;
		g_value_init(&gx, G_TYPE_INT);
		g_value_set_int(&gx, x);

		GValue gy = G_VALUE_INIT;
		g_value_init(&gy, G_TYPE_INT);
		g_value_set_int(&gy, y);

		gtk_container_child_set_property(container, widget, "x", &gx);
		gtk_container_child_set_property(container, widget, "y", &gy);
	}
	gtk_widget_set_size_request(widget, (int)width, (int)height);

	return napi_ok;
}

#include "dsktest.h"
extern DskCtrlIProto DskControlProto;
napi_status new_wrapped_Ctrl(DskCtrlIProto *proto, napi_env env, DskCtrlI **ctrl, UIHandle *widget,
							 napi_value *wrapper);

extern DskCtrlIProto DskControlProto;

DSK_DEFINE_TEST(tests_dsk_platform_get_preferred_size_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

	int width, height;
	GtkWindow *window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(widget));
	gtk_widget_show_all(GTK_WIDGET(window));

	dsk_platform_get_preferred_size_t(ctrl, &width, &height);
	printf("%d x %d\n", width, height);
	DSK_ASSERT(width == 129 /*ubuntu*/ || width == 134 /*deepin*/ || width == 132 /*ubuntu 20*/);
	DSK_ASSERT(height == 17 /*ubuntu*/ || height == 22 /*deepin*/);

	gtk_window_close(window);

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE

napi_status dsk_platform_add_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	gtk_container_add(GTK_CONTAINER(self->ctrl_handle), GTK_WIDGET(child));
	return napi_ok;
}

napi_status dsk_platform_remove_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_get_prop_t(struct DskCtrlI *self, const char *prop_name,
									dsk_prop_types prop_type, ...) {
	printf("dsk_platform_get_prop_t\n");

	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	va_list value_valist;

	va_start(value_valist, prop_type);

	void *value = va_arg(value_valist, void *);
	g_object_get(self->ctrl_handle, prop_name, value, NULL);

	va_end(value_valist); /* Clean up. */

	return napi_ok;
}
/*

*/
napi_status dsk_platform_set_prop_t(struct DskCtrlI *self, const char *prop_name,
									dsk_prop_types prop_type, ...) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	va_list value_valist;

	va_start(value_valist, prop_type);

	switch (prop_type) {
	case dsk_prop_i32: {
		int32_t value = va_arg(value_valist, int32_t);
		g_object_set(self->ctrl_handle, prop_name, value, NULL);
		break;
	}
	case dsk_prop_str: {
		char *value = va_arg(value_valist, char *);
		g_object_set(self->ctrl_handle, prop_name, value, NULL);
		break;
	}
	case dsk_prop_f64: {
		double value = va_arg(value_valist, double);
		g_object_set(self->ctrl_handle, prop_name, value, NULL);
		break;
	}
	case dsk_prop_bool: {
		int value = va_arg(value_valist, int);
		g_object_set(self->ctrl_handle, prop_name, (bool)value, NULL);
		break;
	}
	case dsk_prop_date: {
		break;
	}
	}

	va_end(value_valist); /* Clean up. */

	return napi_ok;
}

DSK_DEFINE_TEST(tests_dsk_platform_set_prop_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

	DSK_CTRLI_CALL(ctrl, set_prop, "label", dsk_prop_str, "ciao mondo");

	char *str;
	DSK_CTRLI_CALL(ctrl, get_prop, "label", dsk_prop_str, &str);
	DSK_ASSERT(strcmp(str, "ciao mondo") == 0);

	// how to throw here????
	napi_status res = ctrl->proto->set_prop(ctrl, "non-existent", dsk_prop_str, "ciao mondo");
	DSK_NAPI_CALL(res);

	DSK_CTRLI_CALL(ctrl, set_prop, "angle", dsk_prop_f64, 42.42);

	double dval;
	DSK_CTRLI_CALL(ctrl, get_prop, "angle", dsk_prop_f64, &dval);
	DSK_ASSERT(dval == 42.42);

	DSK_CTRLI_CALL(ctrl, set_prop, "width-chars", dsk_prop_i32, 42);

	int32_t i32val;
	DSK_CTRLI_CALL(ctrl, get_prop, "width-chars", dsk_prop_i32, &i32val);
	DSK_ASSERT(i32val == 42);

	DSK_CTRLI_CALL(ctrl, set_prop, "wrap", dsk_prop_bool, true);

	bool bval;
	DSK_CTRLI_CALL(ctrl, get_prop, "wrap", dsk_prop_bool, &bval);
	DSK_ASSERT(bval == true);

	DSK_END_TEST();
}
DSK_TEST_CLOSE
