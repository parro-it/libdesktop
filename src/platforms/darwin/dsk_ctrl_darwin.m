#include "dsktest.h"
#include "libdesktop.h"

#import <Cocoa/Cocoa.h>

@interface DskTestCtrl : NSTextField
@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
@end

@implementation DskTestCtrl
@end

void dsk_initui_for_test() {}

void *dsk_new_test_widget() {
	DskTestCtrl *ctrl = [[DskTestCtrl alloc] init];
	ctrl.preferredMaxLayoutWidth = 100000;

	ctrl.stringValue = [NSString stringWithUTF8String:"Hello world"];
	return ctrl;
}

napi_status dsk_CtrlI_link_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	DskTestCtrl *ns_ctrl = UI_ctrl;
	ns_ctrl.DskCtrlI = ctrl;
	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	DskTestCtrl *ns_ctrl = UI_ctrl;
	*ctrl = ns_ctrl.DskCtrlI;
	return napi_ok;
}

napi_status dsk_platform_get_prop_t(struct DskCtrlI *self, const char *prop_name,
									void **prop_value) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_set_prop_t(struct DskCtrlI *self, const char *prop_name,
									void *prop_value) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height) {
	NSView *view = self->ctrl_handle;
	[view setFrame:NSMakeRect(x, y, width, height)];

	return napi_ok;
}

napi_status dsk_platform_add_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_remove_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height) {
	NSView *view = self->ctrl_handle;
	self.aTextField.preferredMaxLayoutWidth = self.aTextField.frame.size.width;

	NSSize sz = [view fittingSize];

	*width = sz.width;
	*height = sz.height;
	return napi_ok;
}

napi_status new_wrapped_Ctrl(napi_env env, DskCtrlI **ctrl, UIHandle *widget, napi_value *wrapper);

DSK_DEFINE_TEST(tests_dsk_platform_get_preferred_size_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	int width, height;
	NSWindow *window = [[NSWindow alloc]
		initWithContentRect:NSMakeRect(0, 0, (CGFloat)10, (CGFloat)10)
				  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
							NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
					backing:NSBackingStoreBuffered
					  defer:NO];

	window.contentView = widget;
	[window makeKeyAndOrderFront:window];

	dsk_platform_get_preferred_size_t(ctrl, &width, &height);
	printf("%d x %d\n", width, height);
	DSK_ASSERT(width == 129);
	DSK_ASSERT(height == 17);

	[window close];

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE
