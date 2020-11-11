
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
	/*INIT_ARGS(2);

	GtkWidget* widget = gtk_entry_new();
	dsk_wrap_widget(env,widget,this);
	gtk_widget_add_events(widget,GDK_BUTTON_PRESS_MASK);

	napi_value events;
	DSK_NAPI_CALL(napi_create_object(env,&events));
	DSK_NAPI_CALL(napi_set_named_property(env,this,"events",events));
	napi_value click = dsk_event_new_for_widget(env, "activate", this);
	DSK_NAPI_CALL(napi_set_named_property(env,events,"click",click));

	if (dsk_set_properties(env, argv[0], this)) {
		napi_throw_error(env,NULL,"Error while setting widget properties.\n");
		return NULL;
	}
	*/
	HINSTANCE hInstance = GetModuleHandle(NULL);

	HWND widget = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
					 // use the raw width and height for now
					 // this will get CW_USEDEFAULT (hopefully) predicting well
					 // even if it doesn't, we're adjusting it later
					 100, 20, dummy, NULL, hInstance, NULL);


	dsk_wrap_widget(env, widget, this);

	if (dsk_set_properties(env, argv[0], this)) {
		napi_throw_error(env, NULL, "Error while setting widget properties.\n");
		return NULL;
	}
	
	return NULL;
}
