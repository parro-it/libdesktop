#include "libdesktop.h"
#include <windows.h>
#include <winuser.h>

void dsk_initui_for_test() {
	gtk_init(0, NULL);
}

void *dsk_new_test_widget() {
	return CreateWindow("STATIC", lbl, WS_CHILD | WS_VISIBLE | WS_BORDER, CW_USEDEFAULT,
						CW_USEDEFAULT,
						// use the raw width and height for now
						// this will get CW_USEDEFAULT (hopefully) predicting well
						// even if it doesn't, we're adjusting it later
						100, 20, dummy, NULL, hInstance, NULL);
}

napi_status dsk_CtrlI_set_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	SetWindowLongPtr(UI_ctrl, GWLP_USERDATA, (LONG_PTR)ctrl);

	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	*ctrl = (void *)GetWindowLongPtr(UI_ctrl, GWLP_USERDATA);
	return napi_ok;
}