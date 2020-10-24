#include "napi_utils.h"
#include "widget.h"
#include <windows.h>
#include "control.h"
#include <yoga/Yoga.h>

#define MODULE "container"

void dsk_widget_reposition(napi_env env, UIHandle container, UIHandle widget, float xcoord, float ycoord) {
    //printf("SET POS %p TO %d, %d\n", widget,(int)xcoord,(int)ycoord);
    bool ret = SetWindowPos((HWND)widget,HWND_TOP,(int)xcoord,(int)ycoord,0,0 ,SWP_NOSIZE|SWP_NOZORDER);
	if (!ret) {
		printf("ERROR\n");
	}
}

void dsk_platform_container_add_child(UIHandle parent, UIHandle child) {
    SetParent((HWND)child,(HWND)parent);

}
extern HWND dummy;

LIBUI_FUNCTION(containerNew) {
    INIT_ARGS(2);

    HINSTANCE hInstance = GetModuleHandle(NULL);
    HWND widget = CreateWindowExW(WS_EX_CONTROLPARENT,
		L"DSKcontainerClass", L"",
		WS_CHILD ,
		CW_USEDEFAULT, CW_USEDEFAULT,
		// use the raw width and height for now
		// this will get CW_USEDEFAULT (hopefully) predicting well
		// even if it doesn't, we're adjusting it later
		800, 600,
		dummy, NULL, hInstance, NULL);

	// printf("CREATED container\n");
  
    dsk_wrap_widget(env, widget, this);
        
    dsk_append_all_children(env, widget, argv[1]);

    SetWindowPos(widget, 0, 0, 0, 0, 0, 
  	SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);

    return this;
}

napi_ref ContainerRef;
ATOM initContainer();

napi_value container_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    initContainer();
    //napi_property_descriptor empty[0];
    dsk_define_class_ref(env,module,"Container",containerNew,NULL, &ContainerRef);

    return exports;
}


static LRESULT CALLBACK containerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*RECT r;
	HDC dc;
	PAINTSTRUCT ps;
	CREATESTRUCTW *cs = (CREATESTRUCTW *) lParam;
	WINDOWPOS *wp = (WINDOWPOS *) lParam;
	MINMAXINFO *mmi = (MINMAXINFO *) lParam;
	struct containerInit *init;
	uiWindowsControl *c;
	void (*onResize)(uiWindowsControl *);
	int minwid, minht;
	LRESULT lResult;

	if (handleParentMessages(hwnd, uMsg, wParam, lParam, &lResult) != FALSE)
		return lResult;
	switch (uMsg) {
	case WM_CREATE:
		init = (struct containerInit *) (cs->lpCreateParams);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR) (init->onResize));
		SetWindowLongPtrW(hwnd, 0, (LONG_PTR) (init->c));
		break;		// defer to DefWindowProc()
	case WM_WINDOWPOSCHANGED:
		if ((wp->flags & SWP_NOSIZE) != 0)
			break;	// defer to DefWindowProc();
		onResize = (void (*)(uiWindowsControl *)) GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		c = (uiWindowsControl *) GetWindowLongPtrW(hwnd, 0);
		(*(onResize))(c);
		return 0;
	case WM_GETMINMAXINFO:
		lResult = DefWindowProcW(hwnd, uMsg, wParam, lParam);
		c = (uiWindowsControl *) GetWindowLongPtrW(hwnd, 0);
		uiWindowsControlMinimumSize(c, &minwid, &minht);
		mmi->ptMinTrackSize.x = minwid;
		mmi->ptMinTrackSize.y = minht;
		return lResult;
	case WM_PAINT:
		dc = BeginPaint(hwnd, &ps);
		if (dc == NULL) {
			logLastError(L"error beginning container paint");
			// bail out; hope DefWindowProc() catches us
			break;
		}
		r = ps.rcPaint;
		paintContainerBackground(hwnd, dc, &r);
		EndPaint(hwnd, &ps);
		return 0;
	// tab controls use this to draw the background of the tab area
	case WM_PRINTCLIENT:
		uiWindowsEnsureGetClientRect(hwnd, &r);
		paintContainerBackground(hwnd, (HDC) wParam, &r);
		return 0;
	case WM_ERASEBKGND:
		// avoid some flicker
		// we draw the whole update area anyway
		return 1;
	}*/
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

ATOM initContainer() {
	WNDCLASSW wc;

	ZeroMemory(&wc, sizeof (WNDCLASSW));
	wc.lpszClassName = L"DSKcontainerClass";
	wc.lpfnWndProc = containerWndProc;
	//wc.hInstance = hInstance;
	//wc.hIcon = hDefaultIcon;
	//wc.hCursor = hDefaultCursor;
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.cbWndExtra = sizeof (void *);
	return RegisterClassW(&wc);
}

void uninitContainer(void)
{
	//if (UnregisterClassW(L"DSKcontainerClass", NULL) == 0)
		//logLastError(L"error unregistering container window class");
}