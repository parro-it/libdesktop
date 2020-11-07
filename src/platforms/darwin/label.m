
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

	dsk_wrap_widget(env, widget, this);

	if (dsk_set_properties(env, argv[0], this)) {
		napi_throw_error(env, NULL, "Error while setting widget properties.\n");
		return NULL;
	}
	return this;
}



DSK_UI_PROP_I32(libdesktop, Label, left, "x");
DSK_UI_PROP_I32(libdesktop, Label, top, "y");
DSK_UI_PROP_S(libdesktop, Label, text, "stringValue");
DSK_UI_PROP_BOOL(libdesktop, Label, enabled, "enabled");
