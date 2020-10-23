#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#import <Cocoa/Cocoa.h>

#define MODULE "win"

static napi_ref DSK_WindowRef;

static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

extern napi_ref ContainerRef;



@interface DskWindow : NSWindow 
	@property (nonatomic, readwrite) napi_value wrapper;
    @property (nonatomic, readwrite) YGNodeRef yoganode;
@end

@implementation DskWindow
@end

LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);
    DskWindow* win =[[DskWindow alloc] initWithContentRect:NSMakeRect(0, 0, (CGFloat) 800, (CGFloat) 600)
		styleMask:NSWindowStyleMaskTitled
		backing:NSBackingStoreBuffered
		defer:0];
    
    NSTextField *widget1;
	widget1 = [[NSTextField alloc] initWithFrame:NSZeroRect];
	[widget1 setStringValue: @"ciao mac"];

    NSTextField *widget2;
	widget2 = [[NSTextField alloc] initWithFrame:NSZeroRect];
	[widget2 setStringValue: @"2222 label"];

    NSTextField *widget3;
	widget3 = [[NSTextField alloc] initWithFrame:NSZeroRect];
	[widget3 setStringValue: @"ciao cioa"];

    [widget1 setFrame:NSMakeRect(20,20,100,50)];
    [widget2 setFrame:NSMakeRect(40,80,120,50)];
    [widget3 setFrame:NSMakeRect(60,140,140,50)];
    
    [win.contentView addSubview: widget1];
    [win.contentView addSubview: widget2];
    [win.contentView addSubview: widget3];
    [win setStyleMask: (NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask)];
    [win makeKeyAndOrderFront:win];


    /*
    printf("WINDOWS NEW\n");
   	
    DskWindow* win =[[DskWindow alloc] initWithContentRect:NSMakeRect(0, 0, (CGFloat) 800, (CGFloat) 600)
		styleMask:NSWindowStyleMaskTitled
		backing:NSBackingStoreBuffered
		defer:0];
    dsk_wrap_widget(env, win, this);


    napi_value Container;
    napi_value container;
    napi_value null;

    napi_get_reference_value(env, ContainerRef, &Container);
    napi_get_null(env,&null);
    napi_new_instance(env, Container,2,(napi_value[]){null,argv[1]},&container);
    napi_set_named_property(env, this, "container", container);

    NSView* child_gtk;
    napi_unwrap(env,container,(void**)&child_gtk);
    win.contentView = child_gtk;

    YGNodeRef root = dsk_widget_get_node(env, container);
    dsk_calculate_layout(env, child_gtk, root);
    
    void* winhnd = win;
    [win makeKeyAndOrderFront:win];
    */
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

