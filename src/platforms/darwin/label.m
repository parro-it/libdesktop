
#include "libdesktop.h"
#import "yoga/Yoga.h"
#import <Cocoa/Cocoa.h>
#define MODULE "label"

@interface DskLabel : NSTextField
@property(nonatomic, readwrite) napi_ref wrapper;
@property(nonatomic, readwrite) YGNodeRef yoganode;
@end

@implementation DskLabel
@end

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Label) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	DskLabel *widget;
	widget = [[DskLabel alloc] init];

	[widget setEditable:NO];
	[widget setSelectable:NO];
	[widget setDrawsBackground:NO];
	[widget setHidden:NO];
	[widget setBezeled:NO];
	[widget setAlignment:NSTextAlignmentRight];

	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this, argv));

	return this;
}

DSK_UI_PROP(libdesktop, Label, left, dsk_prop_i32, "x");
DSK_UI_PROP(libdesktop, Label, top, dsk_prop_i32, "y");
DSK_UI_PROP(libdesktop, Label, text, dsk_prop_str, "stringValue");
DSK_UI_PROP(libdesktop, Label, enabled, dsk_prop_bool, "enabled");
