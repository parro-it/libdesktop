
#include "napi_utils.h"
#import <Cocoa/Cocoa.h>

LIBUI_FUNCTION(widgetSetPropS) {
	INIT_ARGS(1);
	ARG_STRING(value, 0);
	NSObject *widget;
	DSK_UNWRAP_WIDGET();
	LOAD_PROP_NAME();
	[widget setValue:[NSString stringWithUTF8String:value]
			  forKey:[NSString stringWithUTF8String:propname]];

	// gtk_widget_show_all(widget);
	return NULL;
}

LIBUI_FUNCTION(widgetGetPropS) {
	INIT_EMPTY_ARGS();
	NSObject *widget;
	DSK_UNWRAP_WIDGET();
	LOAD_PROP_NAME();
	// printf("w:%p\n",widget);
	NSString *result = [widget valueForKey:[NSString stringWithUTF8String:propname]];

	// printf("result:%s\n",[result UTF8String]);
	if (result == NULL) {
		napi_value null;
		napi_status status = napi_get_null(env, &null);
		CHECK_STATUS_THROW(status, napi_get_null);
		return null;
	}
	return make_utf8_string(env, [result UTF8String]);
}
/*
LIBUI_FUNCTION(widgetSetPropI32) {
	INIT_ARGS(1);
	ARG_INT32(value, 0);
	GtkWidget* widget; \
	DSK_UNWRAP_WIDGET();
	LOAD_PROP_NAME();
	// printf("SET %s %d",propname,value);

	g_object_set(widget, propname, value, NULL);

	return NULL;
}

LIBUI_FUNCTION(widgetGetPropI32) {
	INIT_EMPTY_ARGS();
	GtkWidget* widget; \
	DSK_UNWRAP_WIDGET();
	LOAD_PROP_NAME();

	int32_t result;
	g_object_get(widget, propname, &result, NULL);
	return make_int32(env, result);
}

LIBUI_FUNCTION(widgetSetPropBool) {
	INIT_ARGS(1);
	ARG_BOOL(value, 0);
	GtkWidget* widget; \
	DSK_UNWRAP_WIDGET();
	LOAD_PROP_NAME();
	// printf("SET %s %d",propname,value);
	g_object_set(widget, propname, value, NULL);

	return NULL;
}

LIBUI_FUNCTION(widgetGetPropBool) {
	INIT_EMPTY_ARGS();
	GtkWidget* widget; \
	DSK_UNWRAP_WIDGET();
	LOAD_PROP_NAME();

	bool result;
	g_object_get(widget, propname, &result, NULL);
	return make_bool(env, result);
}*/