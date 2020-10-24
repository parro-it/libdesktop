#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#include "windows.h"
#include "winuser.h"
#include <strsafe.h>

void ErrorExit(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw); 
}
#define MODULE "label"
extern HWND dummy;

int icc = 0;

LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);
    HINSTANCE hInstance = GetModuleHandle(NULL);

    char* lbl;
    if (icc%3==0) {
        lbl = "111ciao win32";
    } else if (icc%3==1) {
        lbl = "222ciao win32";
    } else  {
        lbl = "333ciao win32";
    } 
    icc++;

    HWND widget = CreateWindow(
		"STATIC",lbl,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT,
		// use the raw width and height for now
		// this will get CW_USEDEFAULT (hopefully) predicting well
		// even if it doesn't, we're adjusting it later
		100, 20,
		dummy, NULL, hInstance, NULL);
	//printf("CREATED LABEL %p\n",widget );
	if (widget==NULL) {
		ErrorExit("CreateWindow");
	}
/*
    SetWindowPos(widget, 0, 0, 0, 0, 0, 
  	SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
*/
	// printf("done\n");
    
    dsk_wrap_widget(env, widget, this);

	// printf("wrapped done\n");

    return this;
}

	

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    
    dsk_define_class(env,module,"Label",labelNew,NULL);
    return exports;
}