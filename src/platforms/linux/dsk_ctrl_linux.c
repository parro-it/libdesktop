#include "libdesktop.h"

#include <gtk/gtk.h>

void dsk_initui_for_test() {
	gtk_init(0, NULL);
}

void *dsk_new_test_widget() {
	return gtk_label_new("this is a test control");
}

napi_status dsk_CtrlI_set_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	g_object_set_data(G_OBJECT(UI_ctrl), "DskCtrlI", ctrl);
	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	*ctrl = g_object_get_data(G_OBJECT(UI_ctrl), "DskCtrlI");
	return napi_ok;
}