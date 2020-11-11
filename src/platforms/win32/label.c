
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

int icc = 0;

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Label) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);
	HINSTANCE hInstance = GetModuleHandle(NULL);

	char *lbl;
	if (icc % 3 == 0) {
		lbl = "111ciao win32";
	} else if (icc % 3 == 1) {
		lbl = "222ciao win32";
	} else {
		lbl = "333ciao win32";
	}
	icc++;

	HWND widget =
		CreateWindow("STATIC", lbl, WS_CHILD | WS_VISIBLE | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
					 // use the raw width and height for now
					 // this will get CW_USEDEFAULT (hopefully) predicting well
					 // even if it doesn't, we're adjusting it later
					 100, 20, dummy, NULL, hInstance, NULL);
	// printf("CREATED LABEL %p\n",widget );
	if (widget == NULL) {
		ErrorExit("CreateWindow");
	}
	/*
		SetWindowPos(widget, 0, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	*/
	// printf("done\n");

	printf("CREATED LABEL\n");
	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this, argv[0]));

	return this;
}
