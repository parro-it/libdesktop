#include "napi_utils.h"
#include <windows.h>
#define MODULE "win"

static napi_ref WindowRef;

static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

#define windowClass L"libui_uiWindowClass"

static LRESULT CALLBACK windowWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
		// IsDialogMessage() will also generate IDOK and IDCANCEL when pressing Enter and Escape (respectively) on some controls, like EDIT controls
		// swallow those too; they'll cause runMenuEvent() to panic
		// TODO fix the root cause somehow
		if (HIWORD(wParam) != 0 || LOWORD(wParam) <= IDCANCEL)
			break;
		runMenuEvent(LOWORD(wParam), uiWindow(w));
		return 0;
	case WM_WINDOWPOSCHANGED:
		if ((wp->flags & SWP_NOSIZE) != 0)
			break;
		if (w->onContentSizeChanged != NULL)		// TODO figure out why this is happening too early
			if (!w->changingSize)
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

ATOM registerWindowClass(HICON hDefaultIcon, HCURSOR hDefaultCursor)
{
	WNDCLASSW wc;
HINSTANCE hInstance = GetModuleHandle(NULL);
	ZeroMemory(&wc, sizeof (WNDCLASSW));
	wc.lpszClassName = windowClass;
	wc.lpfnWndProc = windowWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = hDefaultIcon;
	wc.hCursor = hDefaultCursor;
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	return RegisterClassW(&wc);
}

LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);
    HINSTANCE hInstance = GetModuleHandle(NULL);
    printf("WIDNWOS NEW\n");
    HWND widget = CreateWindowExW(0,
		windowClass, L"prova",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		// use the raw width and height for now
		// this will get CW_USEDEFAULT (hopefully) predicting well
		// even if it doesn't, we're adjusting it later
		800, 600,
		NULL, NULL, hInstance, NULL);
	ShowWindow(widget, SW_SHOW);

    napi_status status = napi_wrap(env, this, widget, window_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    return this;
}


napi_value win_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    
    const napi_property_descriptor properties[1];// = {
        //DSK_RWPROP_S(title),
        //DSK_RWPROP_I32(width,"default-width"),
        //DSK_RWPROP_I32(height,"default-height"),
        //DSK_RWPROP_BOOL(visible,"visible"),
    //};
    
    napi_status status;
    napi_value Window;

    status = napi_define_class(env, "Window", NAPI_AUTO_LENGTH, windowNew, NULL, 0, properties, &Window);
    CHECK_STATUS_THROW(status, napi_define_class);                                          
	
    status = napi_create_reference(env, Window, 1, &WindowRef);
    CHECK_STATUS_THROW(status, napi_create_reference);                                          

    status = napi_set_named_property(env, module, "Window", Window);                                  
	CHECK_STATUS_THROW(status, napi_set_named_property);   

    return exports;
}
