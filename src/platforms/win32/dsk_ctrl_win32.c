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

napi_status dsk_platform_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height) {
	HWND widget = (HWND)self->ctrl_handle;
	SetWindowPos((HWND)widget, HWND_TOP, x, y, width, height, SWP_NOZORDER);
	return napi_ok;
}

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

#include "dsktest.h"
napi_status new_wrapped_Ctrl(DskCtrlIProto *proto, napi_env env, DskCtrlI **ctrl, UIHandle *widget,
							 napi_value *wrapper);

napi_status dsk_platform_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height) {
	*height = 30;
	*width = 130;
	return napi_ok;
}
#define windowClass L"libui_uiWindowClass"

DSK_DEFINE_TEST(tests_dsk_platform_get_preferred_size_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

	HINSTANCE hInstance = GetModuleHandle(NULL);
	int width, height;
	HWND window =
		CreateWindowExW(0, windowClass, L"prova", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
						// use the raw width and height for now
						// this will get CW_USEDEFAULT (hopefully) predicting well
						// even if it doesn't, we're adjusting it later
						800, 600, NULL, NULL, hInstance, NULL);
	SetParent(widget, window);
	ShowWindow(window, SW_SHOW);

	dsk_platform_get_preferred_size_t(ctrl, &width, &height);
	printf("%d x %d\n", width, height);
	DSK_ASSERT(width == 130);
	DSK_ASSERT(height == 30);

	CloseWindow(window);
	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE
