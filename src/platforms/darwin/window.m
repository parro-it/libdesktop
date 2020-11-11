
#include "libdesktop.h"
#import <Cocoa/Cocoa.h>

DSK_EXTEND_MODULE(libdesktop);
DSK_EXTEND_CLASS(libdesktop, Container);



@interface DskWindow : NSWindow
@property(nonatomic, readwrite) napi_ref wrapper;
@property(nonatomic, readwrite) YGNodeRef yoganode;
@end

@implementation DskWindow
- (BOOL)acceptsFirstResponder {
	return YES;
}
@end


DSK_DEFINE_CLASS(libdesktop, Window) {
	//printf("libdesktop, Window\n");
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);


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
	napi_get_reference_value(env, libdesktop_Container_ref , &Container);

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
	// NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable)];

	float w = YGNodeLayoutGetWidth(root);
	float h = YGNodeLayoutGetHeight(root);
	//float pd = YGNodeLayoutGetPadding(root, YGEdgeRight);

	int uw = win.frame.size.width;
	int uh = win.frame.size.height;

	 printf("window: %dx%d layout:%.0fx%.0f\n",uw,uh, w,h);

	[win setContentSize:NSMakeSize(w, h)];
	[win center];

	//[win makeKeyAndOrderFront:win];
	//[win setInitialResponder:win];

	[win performSelector:@selector(makeKeyAndOrderFront:) withObject:nil afterDelay:0];

	return this;
}

DSK_UI_PROP_S(libdesktop, Window, title, "title");
DSK_UI_PROP_BOOL(libdesktop, Window, visible, "visible");
