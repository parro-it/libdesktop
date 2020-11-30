
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
		CreateWindowEx(WS_EX_CLIENTEDGE,"edit", "", 
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT | ES_NOHIDESEL | WS_TABSTOP, 
			0, 0, 
					 // use the raw width and height for now
					 // this will get CW_USEDEFAULT (hopefully) predicting well
					 // even if it doesn't, we're adjusting it later
					 100, 20, dummy, NULL, hInstance, NULL);

	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, widget, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);

	return NULL;
}

DSK_UI_PROP(libdesktop, Textfield, text, dsk_prop_str, "text");