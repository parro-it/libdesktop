
#include "libdesktop.h"
#import "yoga/Yoga.h"
#import <Cocoa/Cocoa.h>

@interface DskTextField : NSTextField
@property(nonatomic, readwrite) napi_ref wrapper;
@property(nonatomic, readwrite) YGNodeRef yoganode;
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

	DskTextField *widget;
	widget = [[DskTextField alloc] init];
	[widget setEditable:true];
	[widget setSelectable:true];
	[widget setHidden:false];
	DSK_NAPI_CALL(dsk_wrap_widget(env, widget, this, argv));

	return this;
}

DSK_UI_PROP_S(libdesktop, Textfield, text, "stringValue");
