
#include "libdesktop.h"
#include "windows.h"
#include "winuser.h"
#include <strsafe.h>

void ErrorExit(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
					  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(
		LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR),
					TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}
extern HWND dummy;


DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Label) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);
	HINSTANCE hInstance = GetModuleHandle(NULL);

	HWND widget =
		CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE , CW_USEDEFAULT, CW_USEDEFAULT,
					 // use the raw width and height for now
					 // this will get CW_USEDEFAULT (hopefully) predicting well
					 // even if it doesn't, we're adjusting it later
					 100, 20, dummy, NULL, hInstance, NULL);
	printf("CREATED LABEL %p\n",widget );
	if (widget == NULL) {
		ErrorExit("CreateWindow");
	}
	
	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, widget, this, &ctrl);

	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);


	return this;
}

DSK_UI_PROP(libdesktop, Label, text, dsk_prop_str, "text");