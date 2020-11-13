#include "libdesktop.h"

#import <Cocoa/Cocoa.h>

@interface DskTestCtrl : NSTextField
@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
@end

@implementation DskTestCtrl
@end

void dsk_initui_for_test() {}

void *dsk_new_test_widget() { return [[DskTestCtrl alloc] init]; }

napi_status dsk_CtrlI_set_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl) {
	DskTestCtrl *ns_ctrl = UI_ctrl;
	ns_ctrl.DskCtrlI = ctrl;
	return napi_ok;
}

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl) {
	DskTestCtrl *ns_ctrl = UI_ctrl;
	*ctrl = ns_ctrl.DskCtrlI;
	return napi_ok;
}