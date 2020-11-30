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

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height.  
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
        return NULL; //errhandler("GetObject", hwnd); 

    // Convert the color format to a count of bits.  
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure  
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
    // data structures.)  

     if (cClrBits < 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (int64_t)(1 << cClrBits)); 

     // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure.  

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag.  
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color  
    // indices and store the result in biSizeImage.  
    // The width must be DWORD aligned unless the bitmap is RLE 
    // compressed. 
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the  
    // device colors are important.  
     pbmi->bmiHeader.biClrImportant = 0; 
     return pbmi; 
 }

void CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC) 
 { 
    HANDLE hf;                 // file handle  
    BITMAPFILEHEADER hdr;       // bitmap file-header  
    PBITMAPINFOHEADER pbih;     // bitmap info-header  
    LPBYTE lpBits;              // memory pointer  
    DWORD dwTotal;              // total count of bytes  
    DWORD cb;                   // incremental count of bytes  
    BYTE *hp;                   // byte pointer  
    DWORD dwTmp; 

	PBITMAPINFO pbi = CreateBitmapInfoStruct(hBMP);
    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) 
        return; 

    // Retrieve the color table (RGBQUAD array) and the bits  
    // (array of palette indices) from the DIB.  
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
        DIB_RGB_COLORS)) 
    {
        return;  
    }

    // Create the .BMP file.  
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
       return; 
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
    // Compute the size of the entire file.  
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices.  
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file.  
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
        (LPDWORD) &dwTmp,  NULL)) 
    {
       return; 
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, ( NULL)))
        return; 

    // Copy the array of color indices into the .BMP file.  
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
          return; 

    // Close the .BMP file.  
     if (!CloseHandle(hf)) 
          return; 

    // Free memory.  
    GlobalFree((HGLOBAL)lpBits);
}

DSK_DEFINE_METHOD(libdesktop, Window, saveAsPNGImage) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);
	napi_value filename = argv[0];
	char *c_filename;
	DSK_NAPI_CALL(dsk_get_utf8_cstr(env, filename, &c_filename));
	
	struct DskCtrlI *ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, this, &ctrl));

	HWND hwnd = ctrl->ctrl_handle;

	HDC hDC       = GetDC( hwnd );
	HDC hTargetDC = CreateCompatibleDC( hDC );
	RECT rect     = {0};

	GetWindowRect( hwnd, &rect );

	HBITMAP hBitmap = CreateCompatibleBitmap( hDC, rect.right - rect.left,	rect.bottom - rect.top );
	SelectObject( hTargetDC, hBitmap );
	PrintWindow( hwnd, hTargetDC, PW_CLIENTONLY );
	
	CreateBMPFile(hwnd, c_filename, hBitmap, hTargetDC);
	
	DeleteObject( hBitmap );
	ReleaseDC( hwnd, hDC );
	DeleteDC( hTargetDC );

	return NULL;
}
