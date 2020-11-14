#include "libdesktop.h"
#include <windows.h>
#include <winuser.h>

void dsk_initui_for_test() {}

void *dsk_new_test_widget() {
	HINSTANCE hInstance = GetModuleHandle(NULL);

	return CreateWindow("STATIC", "this is a test control", WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
						0, 0, NULL, NULL, hInstance, NULL);
}

napi_status dsk_CtrlI_link_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	SetWindowLongPtr(UI_ctrl, GWLP_USERDATA, (LONG_PTR)ctrl);

	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	*ctrl = (void *)GetWindowLongPtr(UI_ctrl, GWLP_USERDATA);
	return napi_ok;
}