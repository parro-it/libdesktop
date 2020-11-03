#include "napi_utils.h"
#include "yoga.h"
#import <Cocoa/Cocoa.h>

#include "libdesktop.h"

@interface DskControl : NSView
@property napi_ref wrapper;
@property YGNodeRef yoganode;
@end

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget) {
	// printf("unwrap %p\n", widget);
	DskControl *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		// printf("non ok \n");
		const napi_extended_error_info *result;
		napi_get_last_error_info(env, &result);
		// printf("napi_unwrap failed with code %d: %s\n", result->engine_error_code,
		// result->error_message);
		return NULL;
	}
	// printf("widgetG %p\n",widgetG);

	return widgetG.yoganode;
}

void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node) {
	DskControl *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		return;
	}

	// printf("node %p\n",node);

	widgetG.yoganode = node;

	napi_ref ref;
	napi_create_reference(env, widget, 1, &ref);

	// printf("ref %p\n",ref);
	widgetG.wrapper = ref;
}

napi_value dsk_widget_wrapper(napi_env env, UIHandle widget) {
	napi_ref ref = ((DskControl *)widget).wrapper;
	if (ref == NULL) {
		return NULL;
	}
	// printf("ref is %p\n",ref);
	napi_value node;
	napi_get_reference_value(env, ref, &node);
	return node;
}

void dsk_get_preferred_sizes(UIHandle widget, int *width, int *height) {
	NSView *view = widget;
	NSSize sz = [view fittingSize];

	*width = sz.width;
	if (*width < 130) {
		*width = 130;
	}
	*height = sz.height;
}

void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args) {}