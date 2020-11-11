
#include "libdesktop.h"

#include "windows.h"
#include "winuser.h"
#include <assert.h>
#include <yoga.h>

extern HWND dummy;

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Textfield) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	HINSTANCE hInstance = GetModuleHandle(NULL);

	HWND widget =
		CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
					 // use the raw width and height for now
					 // this will get CW_USEDEFAULT (hopefully) predicting well
					 // even if it doesn't, we're adjusting it later
					 100, 20, dummy, NULL, hInstance, NULL);

	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this));

	return NULL;
}
