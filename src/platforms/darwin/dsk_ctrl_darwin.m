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
	[ctrl setEditable:NO];

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


napi_status dsk_platform_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height) {
	NSView *view = self->ctrl_handle;
	[view setFrame:NSMakeRect(x, y, width, height)];

	return napi_ok;
}

napi_status dsk_platform_add_child_t(struct DskCtrlI *self, UIHandle child) {

	NSView *parent = (NSView *)self->ctrl_handle;
	[parent addSubview:(NSView *)child];

	return napi_ok;
}

napi_status dsk_platform_remove_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

napi_status dsk_platform_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height) {
	NSView *view = self->ctrl_handle;

	NSSize sz = [view fittingSize];

	*width = sz.width;
	*height = sz.height;
	return napi_ok;
}

extern DskCtrlIProto DskControlProto;
napi_status new_wrapped_Ctrl(DskCtrlIProto *proto, napi_env env, DskCtrlI **ctrl, UIHandle *widget,
							 napi_value *wrapper);

DSK_DEFINE_TEST(tests_dsk_platform_get_preferred_size_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

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
	// printf("%d x %d\n", width, height);
	DSK_ASSERT(width == 72);
	DSK_ASSERT(height == 21);

	[window close];

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE
/*
void dsk_ui_set_prop_s(void *instance, char *value, void **datas) {
	NSObject *widget = instance;
	char *propname = datas[3];
	[widget setValue:[NSString stringWithUTF8String:value]
			  forKey:[NSString stringWithUTF8String:propname]];
}

char *dsk_ui_get_prop_s(void *instance, void **datas) {
	NSObject *widget = instance;
	char *propname = datas[3];
	NSString *ns_result = [widget valueForKey:[NSString stringWithUTF8String:propname]];
	char *result = (char *)[ns_result UTF8String];
	return result;
}
*/


napi_status dsk_platform_get_prop_t(struct DskCtrlI *self, const char *prop_name,
									dsk_prop_types prop_type, ...) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	va_list value_valist;

	va_start(value_valist, prop_type);


	NSView* widget = self->ctrl_handle;
	NSString* key = [NSString stringWithUTF8String:prop_name];

	
	switch (prop_type) {
	case dsk_prop_i32: {
		int32_t* value = va_arg(value_valist, int32_t*);
		*value = ((NSNumber*)[widget valueForKey:key]).intValue;
		
		break;
	}
	case dsk_prop_str: {
		char **value = va_arg(value_valist, char **);
		NSString *ns_result = [widget valueForKey:key];
		*value = (char *)[ns_result UTF8String];
		break;
	}
	case dsk_prop_f64: {
		double *value = va_arg(value_valist, double*);
		*value = ((NSNumber*)[widget valueForKey:key]).doubleValue;
		break;
	}
	case dsk_prop_bool: {
		bool *value = va_arg(value_valist, bool*);
		*value = (bool)((NSNumber*)[widget valueForKey:key]).intValue;
		break;
	}
	case dsk_prop_date: {
		break;
	}
	}

	va_end(value_valist); /* Clean up. */

	return napi_ok;
}

napi_status dsk_platform_set_prop_t(struct DskCtrlI *self, const char *prop_name,
									dsk_prop_types prop_type, ...) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	va_list value_valist;

	va_start(value_valist, prop_type);
printf("set prop %s\n",prop_name);
	NSView* widget = self->ctrl_handle;

	NSString* key = [NSString stringWithUTF8String:prop_name];

	switch (prop_type) {
	case dsk_prop_i32: {
		int32_t value = va_arg(value_valist, int32_t);
		[widget setValue:[NSNumber numberWithInteger:value] forKey:key];
		break;
	}
	case dsk_prop_str: {
		char *value = va_arg(value_valist, char *);
		
		[widget setValue:[NSString stringWithUTF8String:value] forKey:key];
		break;
	}
	case dsk_prop_f64: {
		double value = va_arg(value_valist, double);
		[widget setValue:[NSNumber numberWithDouble:value] forKey:key];
		break;
	}
	case dsk_prop_bool: {
		int value = va_arg(value_valist, int);
		[widget setValue:[NSNumber numberWithInteger:value] forKey:key];
		break;
	}
	case dsk_prop_date: {
		break;
	}
	}

	va_end(value_valist); /* Clean up. */

	return napi_ok;
}

DSK_DEFINE_TEST(tests_dsk_platform_set_prop_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

	DSK_CTRLI_CALL(ctrl, set_prop, "stringValue", dsk_prop_str, "ciao mondo");

	char *str;
	DSK_CTRLI_CALL(ctrl, get_prop, "stringValue", dsk_prop_str, &str);
	DSK_ASSERT(strcmp(str, "ciao mondo") == 0);

	// how to test for throw here????
	//napi_status res = ctrl->proto->set_prop(ctrl, "non-existent", dsk_prop_str, "ciao mondo");
	//DSK_NAPI_CALL(res);
printf("11\n");
	DSK_CTRLI_CALL(ctrl, set_prop, "preferredMaxLayoutWidth", dsk_prop_f64, 42.42);

	double dval;
	DSK_CTRLI_CALL(ctrl, get_prop, "preferredMaxLayoutWidth", dsk_prop_f64, &dval);
	DSK_ASSERT(dval == 42.42);
printf("22\n");
	DSK_CTRLI_CALL(ctrl, set_prop, "maximumNumberOfLines", dsk_prop_i32, 42);

	int32_t i32val;
	DSK_CTRLI_CALL(ctrl, get_prop, "maximumNumberOfLines", dsk_prop_i32, &i32val);
	DSK_ASSERT(i32val == 42);
printf("33\n");
	DSK_CTRLI_CALL(ctrl, set_prop, "allowsCharacterPickerTouchBarItem", dsk_prop_bool, true);

	bool bval;
	DSK_CTRLI_CALL(ctrl, get_prop, "allowsCharacterPickerTouchBarItem", dsk_prop_bool, &bval);
	DSK_ASSERT(bval == true);

	DSK_END_TEST();
}
DSK_TEST_CLOSE