#include "control.h"
#include "libdesktop.h"
#include "napi_utils.h"
#include "widget.h"
#import <Cocoa/Cocoa.h>

#define MODULE "win"

static napi_ref DSK_WindowRef;

extern napi_ref ContainerRef;

@interface DskWindow : NSWindow
@property(nonatomic, readwrite) napi_ref wrapper;
@property(nonatomic, readwrite) YGNodeRef yoganode;
@end

@implementation DskWindow
- (BOOL)acceptsFirstResponder {
	return YES;
}
@end

LIBUI_FUNCTION(windowNew) {
	INIT_ARGS(2);

	DskWindow *win = [[DskWindow alloc]
		initWithContentRect:NSMakeRect(0, 0, (CGFloat)10, (CGFloat)10)
				  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
							NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
					backing:NSBackingStoreBuffered
					  defer:NO];
	dsk_wrap_widget(env, win, this);

	if (dsk_set_properties(env, argv[0], this)) {
		napi_throw_error(env, NULL, "Error while setting widget properties.\n");
		return NULL;
	}

	napi_value Container;
	napi_value container;
	napi_value props;

	napi_create_object(env, &props);
	napi_get_reference_value(env, ContainerRef, &Container);

	bool hasStyle;
	DSK_NAPI_CALL(napi_has_named_property(env, argv[0], "style", &hasStyle));
	if (hasStyle) {
		napi_value containerStyle;
		DSK_NAPI_CALL(napi_get_named_property(env, argv[0], "style", &containerStyle));
		DSK_NAPI_CALL(napi_set_named_property(env, props, "style", containerStyle));
	}

	napi_new_instance(env, Container, 2, (napi_value[]){props, argv[1]}, &container);
	napi_set_named_property(env, this, "container", container);

	NSView *child_gtk;
	napi_unwrap(env, container, (void **)&child_gtk);
	win.contentView = child_gtk;

	YGNodeRef root = dsk_widget_get_node(env, container);
	dsk_set_children_preferred_sizes(root, child_gtk);
	dsk_calculate_layout(env, child_gtk, root, YGUndefined, YGUndefined);

	//[win setStyleMask: (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
	//NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable)];

	float w = YGNodeLayoutGetWidth(root);
	float h = YGNodeLayoutGetHeight(root);
	float pd = YGNodeLayoutGetPadding(root, YGEdgeRight);

	int uw = win.frame.size.width;
	int uh = win.frame.size.height;

	// printf("window: %dx%d layout:%.0fx%.0f\n",uw,uh, w,h);

	[win setContentSize:NSMakeSize(w, h)];
	[win center];

	//[win makeKeyAndOrderFront:win];
	//[win setInitialResponder:win];

	[win performSelector:@selector(makeKeyAndOrderFront:) withObject:nil afterDelay:0];

	return this;
}

LIBUI_FUNCTION(setTitle) {
	INIT_ARGS(1);
	ARG_STRING(val, 0)

	NSWindow *widget;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&widget));

	widget.title = [NSString stringWithUTF8String:val];

	return NULL;
}

LIBUI_FUNCTION(getTitle) {
	INIT_EMPTY_ARGS();
	NSWindow *widget;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&widget));

	NSString *str = widget.title;
	napi_value res;
	DSK_NAPI_CALL(napi_create_string_utf8(env, [str cStringUsingEncoding:NSUTF8StringEncoding],
										  NAPI_AUTO_LENGTH, &res));
	return res;
}

#include <ApplicationServices/ApplicationServices.h>

napi_value win_init(napi_env env, napi_value exports) {
	DEFINE_MODULE()
	ProcessSerialNumber psn = {0, kCurrentProcess};
	TransformProcessType(&psn, kProcessTransformToForegroundApplication);

	const napi_property_descriptor properties[] = {
		{.utf8name = "title", .getter = getTitle, .setter = setTitle},
		/*DSK_RWPROP_I32(width,"default-width"),
		DSK_RWPROP_I32(height,"default-height"),
		DSK_RWPROP_BOOL(visible,"visible"),*/
	};

	napi_status status;
	napi_value Window;

	status =
		napi_define_class(env, "Window", NAPI_AUTO_LENGTH, windowNew, NULL, 1, properties, &Window);
	CHECK_STATUS_THROW(status, napi_define_class);

	status = napi_create_reference(env, Window, 1, &DSK_WindowRef);
	CHECK_STATUS_THROW(status, napi_create_reference);

	status = napi_set_named_property(env, module, "Window", Window);
	CHECK_STATUS_THROW(status, napi_set_named_property);

	return exports;
}
