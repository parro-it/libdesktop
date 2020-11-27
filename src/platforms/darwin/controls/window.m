
#include "libdesktop.h"
#import <Cocoa/Cocoa.h>

DSK_EXTEND_MODULE(libdesktop);

extern DskCtrlIProto DskRootContainerProto;

DSK_EXTEND_CLASS(libdesktop, RootContainer)

@interface DskWindow : NSWindow
	@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
@end

@implementation DskWindow
- (BOOL)acceptsFirstResponder {
	return YES;
}
@end

DSK_DEFINE_CLASS(libdesktop, Window) {
	// // printf("libdesktop, Window\n");
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	DskWindow *win = [[DskWindow alloc]
		initWithContentRect:NSMakeRect(0, 0, (CGFloat)10, (CGFloat)10)
				  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
							NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
					backing:NSBackingStoreBuffered
					  defer:NO];

	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, win, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);
	
	napi_value props;
	napi_create_object(env, &props);

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

	NSView *child_gtk=cntr_ctrl->ctrl_handle;
	printf("child_gtk %p\n",child_gtk);

	win.contentView = child_gtk;



	DSK_CTRLI_CALL(cntr_ctrl, reposition, -1, -1, -1, -1);

	//[win setStyleMask: (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
	// NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable)];

	float w = YGNodeLayoutGetWidth(cntr_ctrl->yg_node);
	float h = YGNodeLayoutGetHeight(cntr_ctrl->yg_node);
	// float pd = YGNodeLayoutGetPadding(root, YGEdgeRight);

	int uw = win.frame.size.width;
	int uh = win.frame.size.height;

	 printf("window: %dx%d layout:%.0fx%.0f\n", uw, uh, w, h);

	[win setContentSize:NSMakeSize(w, h)];
	[win center];

	//[win makeKeyAndOrderFront:win];
	//[win setInitialResponder:win];

	[win performSelector:@selector(makeKeyAndOrderFront:) withObject:nil afterDelay:0];

	return this;
}


DSK_DEFINE_METHOD(libdesktop, Window, saveAsPNGImage) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);

	napi_value filename = argv[0];

	struct DskCtrlI *ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, this, &ctrl));

	NSView *win = ctrl->ctrl_handle;

	char *c_filename;
	DSK_NAPI_CALL(dsk_get_utf8_cstr(env, filename, &c_filename));

	return NULL;
}


DSK_UI_PROP(libdesktop, Window, visible, dsk_prop_bool, "visible");
DSK_UI_PROP(libdesktop, Window, title, dsk_prop_str, "title");
