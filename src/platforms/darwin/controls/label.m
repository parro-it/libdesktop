
#include "libdesktop.h"
#import "yoga/Yoga.h"
#import <Cocoa/Cocoa.h>

@interface DskLabel : NSTextField
	@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
@end

@implementation DskLabel

@end

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Label) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	DskLabel *widget = [[DskLabel alloc] init];

	[widget setEditable:NO];
	[widget setSelectable:NO];
	[widget setDrawsBackground:NO];
	[widget setHidden:NO];
	[widget setBezeled:NO];
	[widget setAlignment:NSTextAlignmentRight];

	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, widget, this, &ctrl);

	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);

	return this;
}

DSK_UI_PROP(libdesktop, Label, left, dsk_prop_i32, "x");
DSK_UI_PROP(libdesktop, Label, top, dsk_prop_i32, "y");
DSK_UI_PROP(libdesktop, Label, label, dsk_prop_str, "stringValue");
DSK_UI_PROP(libdesktop, Label, enabled, dsk_prop_bool, "enabled");
