#include "libdesktop.h"
#include <windows.h>

static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {}

#define windowClass L"libui_uiWindowClass"

static LRESULT CALLBACK windowWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	/*LONG_PTR ww;
	uiWindow *w;
	CREATESTRUCTW *cs = (CREATESTRUCTW *) lParam;
	WINDOWPOS *wp = (WINDOWPOS *) lParam;
	MINMAXINFO *mmi = (MINMAXINFO *) lParam;
	int width, height;
	LRESULT lResult;

	ww = GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	if (ww == 0) {
		if (uMsg == WM_CREATE)
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR) (cs->lpCreateParams));
		// fall through to DefWindowProc() anyway
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
	w = uiWindow((void *) ww);
	if (handleParentMessages(hwnd, uMsg, wParam, lParam, &lResult) != FALSE)
		return lResult;
	switch (uMsg) {
	case WM_COMMAND:
		// not a menu
		if (lParam != 0)
			break;
		// IsDialogMessage() will also generate IDOK and IDCANCEL when pressing Enter and Escape
	(respectively) on some controls, like EDIT controls
		// swallow those too; they'll cause runMenuEvent() to panic
		// TODO fix the root cause somehow
		if (HIWORD(wParam) != 0 || LOWORD(wParam) <= IDCANCEL)
			break;
		runMenuEvent(LOWORD(wParam), uiWindow(w));
		return 0;
	case WM_WINDOWPOSCHANGED:
		if ((wp->flags & SWP_NOSIZE) != 0)
			break;
		if (w->onContentSizeChanged != NULL)		// TODO figure out why this is happening too
	early if (!w->changingSize)
				(*(w->onContentSizeChanged))(w, w->onContentSizeChangedData);
		windowRelayout(w);
		return 0;
	case WM_GETMINMAXINFO:
		// ensure the user cannot resize the window smaller than its minimum size
		lResult = DefWindowProcW(hwnd, uMsg, wParam, lParam);
		uiWindowsControlMinimumSize(uiWindowsControl(w), &width, &height);
		// width and height are in client coordinates; ptMinTrackSize is in window coordinates
		clientSizeToWindowSize(w->hwnd, &width, &height, w->hasMenubar);
		mmi->ptMinTrackSize.x = width;
		mmi->ptMinTrackSize.y = height;
		return lResult;
	case WM_PRINTCLIENT:
		// we do no special painting; just erase the background
		// don't worry about the return value; we let DefWindowProcW() handle this message
		SendMessageW(hwnd, WM_ERASEBKGND, wParam, lParam);
		return 0;
	case WM_CLOSE:
		if ((*(w->onClosing))(w, w->onClosingData))
			uiControlDestroy(uiControl(w));
		return 0;		// we destroyed it already
	}*/
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

ATOM registerWindowClass(HICON hDefaultIcon, HCURSOR hDefaultCursor) {
	WNDCLASSW wc;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	ZeroMemory(&wc, sizeof(WNDCLASSW));
	wc.lpszClassName = windowClass;
	wc.lpfnWndProc = windowWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = hDefaultIcon;
	wc.hCursor = hDefaultCursor;
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	return RegisterClassW(&wc);
}

DSK_EXTEND_MODULE(libdesktop);
DSK_EXTEND_CLASS(libdesktop, Container);
DSK_EXTEND_CLASS(libdesktop, RootContainer)

DSK_DEFINE_CLASS(libdesktop, Window) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	HINSTANCE hInstance = GetModuleHandle(NULL);
	// printf("WIDNWOS NEW\n");
	HWND win =
		CreateWindowExW(0, windowClass, L"prova", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
						// use the raw width and height for now
						// this will get CW_USEDEFAULT (hopefully) predicting well
						// even if it doesn't, we're adjusting it later
						800, 600, NULL, NULL, hInstance, NULL);

	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, win, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);

	napi_value props;

	DSK_NAPI_CALL(napi_create_object(env, &props));

	bool hasStyle;
	DSK_NAPI_CALL(napi_has_named_property(env, argv[0], "style", &hasStyle));
	if (hasStyle) {
		napi_value containerStyle;
		DSK_NAPI_CALL(napi_get_named_property(env, argv[0], "style", &containerStyle));
		DSK_NAPI_CALL(napi_set_named_property(env, props, "style", containerStyle));
	}

	napi_value cntr_wrapper =
		dsk_new_instance(env, libdesktop_RootContainer_ref, 2, (napi_value[]){props, argv[1]});

	DskCtrlI *cntr_ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, cntr_wrapper, &cntr_ctrl));

	HWND cntr_gtk = cntr_ctrl->ctrl_handle;

	SetParent(cntr_gtk, win);

	DSK_CTRLI_CALL(cntr_ctrl, reposition, -1, -1, -1, -1);

	float w = YGNodeLayoutGetWidth(cntr_ctrl->yg_node);
	float h = YGNodeLayoutGetHeight(cntr_ctrl->yg_node);
	float pd = YGNodeLayoutGetPadding(cntr_ctrl->yg_node, YGEdgeRight);

	SetWindowPos(win, NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
	return this;
}

DSK_DEFINE_METHOD(libdesktop, Window, close) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(0);

	struct DskCtrlI *ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, this, &ctrl));

	HWND win = ctrl->ctrl_handle;
	printf("HIDE\n");
	ShowWindow(win, SW_HIDE);
	printf("DESTROY\n");
	DestroyWindow(win);
	printf("DESTROIED\n");

	return NULL;
}


DSK_DEFINE_METHOD(libdesktop, Window, saveAsPNGImage) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);
	napi_value filename = argv[0];
	char *c_filename;
	DSK_NAPI_CALL(dsk_get_utf8_cstr(env, filename, &c_filename));
	printf ("SAVED\n");
	return NULL;
}
