#include "napi_utils.h"
#include "widget.h"
#import <Cocoa/Cocoa.h>

#define MODULE "win"

static napi_ref DSK_WindowRef;

static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}





LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);

    printf("WINDOWS NEW\n");
   	
    NSWindow* win =[[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, (CGFloat) 800, (CGFloat) 600)
		styleMask:NSWindowStyleMaskTitled
		backing:NSBackingStoreBuffered
		defer:0];
    void* winhnd = win;
    napi_status status = napi_wrap(env, this, winhnd, window_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          
    [win makeKeyAndOrderFront:win];

    return this;
}


napi_value win_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    
    const napi_property_descriptor properties[] = {
        DSK_RWPROP_S(title),
        /*DSK_RWPROP_I32(width,"default-width"),
        DSK_RWPROP_I32(height,"default-height"),
        DSK_RWPROP_BOOL(visible,"visible"),*/
    };
    
    napi_status status;
    napi_value Window;

    status = napi_define_class(env, "Window", NAPI_AUTO_LENGTH, windowNew, NULL, 1, properties, &Window);
    CHECK_STATUS_THROW(status, napi_define_class);                                          
	
    status = napi_create_reference(env, Window, 1, &DSK_WindowRef);
    CHECK_STATUS_THROW(status, napi_create_reference);                                          

    status = napi_set_named_property(env, module, "Window", Window);                                  
	CHECK_STATUS_THROW(status, napi_set_named_property);   

    return exports;
}

