
#include "libdesktop.h"
#include "napi_utils.h"
#import "yoga/Yoga.h"
#import <Cocoa/Cocoa.h>
#define MODULE "textfield"

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

LIBUI_FUNCTION(textfieldNew) {
	INIT_ARGS(2);

	DskTextField *widget;
	widget = [[DskTextField alloc] init];
	[widget setEditable:true];
	[widget setSelectable:true];
	[widget setHidden:false];
	dsk_wrap_widget(env, widget, this);

	if (dsk_set_properties(env, argv[0], this)) {
		napi_throw_error(env, NULL, "Error while setting widget properties.\n");
		return NULL;
	}
	return this;
}

static LIBUI_FUNCTION(setStringValue) {
	INIT_ARGS(1);
	ARG_STRING(val, 0)

	NSTextField *widget;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&widget));

	[widget setStringValue:[NSString stringWithUTF8String:val]];

	return NULL;
}

static LIBUI_FUNCTION(getStringValue) {
	INIT_EMPTY_ARGS();
	NSTextField *widget;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&widget));

	NSString *str = [widget stringValue];
	napi_value res;
	DSK_NAPI_CALL(napi_create_string_utf8(env, [str cStringUsingEncoding:NSUTF8StringEncoding],
										  NAPI_AUTO_LENGTH, &res));
	return res;
}

napi_value textfield_init(napi_env env, napi_value exports) {
	DEFINE_MODULE()

	dsk_define_class(env, module, "Textfield", textfieldNew,
					 ((napi_property_descriptor[]){
						 {.utf8name = "text", .getter = getStringValue, .setter = setStringValue},
						 // DSK_RWPROP_BOOL(visible,"enabled"),
						 // DSK_CHILDPROP_I32(left,"x"),
						 // DSK_CHILDPROP_I32(top,"y")
					 }));
	return exports;
}