
#include "libdesktop.h"
#import "yoga/Yoga.h"
#import <Cocoa/Cocoa.h>

@interface DskTextField : NSTextField
	@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
@end

@implementation DskTextField
- (BOOL)acceptsFirstResponder {
	return YES;
}
- (BOOL)needsPanelToBecomeKey {
	return YES;
}
- (BOOL)canBecomeKeyView {
	return YES;
}

@end

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Textfield) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	DskTextField *widget = [[DskTextField alloc] init];
	[widget setEditable:true];
	[widget setSelectable:true];
	[widget setHidden:false];

	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, widget, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);

	return this;
}

DSK_UI_PROP(libdesktop, Textfield, left, dsk_prop_i32, "x");
DSK_UI_PROP(libdesktop, Textfield, top, dsk_prop_i32, "y");
DSK_UI_PROP(libdesktop, Textfield, text, dsk_prop_str, "stringValue");
DSK_UI_PROP(libdesktop, Textfield, enabled, dsk_prop_bool, "enabled");
