#include "libdesktop.h"

#include <gtk/gtk.h>

void dsk_initui_for_test() {
	gtk_init(0, NULL);
}

void *dsk_new_test_widget() {
	return gtk_label_new("this is a test control");
}
/*
YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget) {
	// printf("unwrap %p\n", widget);
	GObject *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		// printf("non ok \n");
		const napi_extended_error_info *result;
		napi_get_last_error_info(env, &result);
		// printf("napi_unwrap failed with code %d: %s\n", result->engine_error_code,
		// result->error_message);
		return NULL;
	}
	// printf("widgetG %p\n",widgetG);
	YGNodeRef node = g_object_get_data(widgetG, "yoganode");
	return node;
}
*/
napi_status dsk_CtrlI_set_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	g_object_set_data(G_OBJECT(UI_ctrl), "DskCtrlI", ctrl);
	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	*ctrl = g_object_get_data(G_OBJECT(UI_ctrl), "DskCtrlI");
	return napi_ok;
}