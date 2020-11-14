#include "libdesktop.h"

#import <Cocoa/Cocoa.h>

@interface DskTestCtrl : NSTextField
@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
@end

@implementation DskTestCtrl
@end

void dsk_initui_for_test() {}

void *dsk_new_test_widget() { return [[DskTestCtrl alloc] init]; }

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

napi_status dsk_platform_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
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