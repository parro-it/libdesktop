#ifndef DSK_UI_H__
#define DSK_UI_H__ 1

#include "yoga/Yoga.h"

typedef void *UIHandle;
/*

// widget interface, implemented in every native platform
// reimplemented on top of DskCtrlI

 YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget);
 void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node);
 void dsk_get_preferred_sizes(UIHandle widget, int *width, int *height);

 void dsk_platform_container_add_child(UIHandle parent, UIHandle child);
 void dsk_widget_reposition(napi_env env, UIHandle container, UIHandle widget,
										   float x, float y, float width, float height);

// native properties, implemented in every native platform
// reimplemented on top of DskCtrlI get_prop & set_prop

 void dsk_ui_set_prop_s(void *instance, char *value, void **datas);
 char *dsk_ui_get_prop_s(void *instance, void **datas);
 void dsk_ui_set_prop_i32(void *instance, int32_t value, void **datas);
 int32_t dsk_ui_get_prop_i32(void *instance, void **datas);
 void dsk_ui_set_prop_bool(void *instance, bool value, void **datas);
 bool dsk_ui_get_prop_bool(void *instance, void **datas);

// yoga specific container interface, implemented in common platform

// dsk_add_child reimpl as a yoga container specific add_child DskCtrlI method. also customize
// DskCtrlI remove_child method to remove child from yoga nodes graph
 void dsk_add_child(napi_env env, UIHandle parentHandle, UIHandle childHandle);

// this 2 reimpl as custom reposition method on DskCtrlI
 void dsk_calculate_layout(napi_env env, UIHandle container, YGNodeRef root,
										  float availableWidth, float availableHeight);
 void dsk_set_children_preferred_sizes(YGNodeRef node, UIHandle widget);

// widget helpers, implemented in common platform
// change to use DskCtrlI object
// reimplemented on top of DskCtrlI
 napi_status dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this,
											napi_value *argv);
 UIHandle dsk_unwrap_widget(napi_env env, napi_value this);
 napi_value dsk_widget_get_wrapper(napi_env env, UIHandle widget);
*/

/* reimplemented on top of DskCtrlI ???????? */

napi_value dsk_event_new_for_widget(napi_env env, const char *eventname, napi_value sender);

struct dsk_event_args {
	napi_env env;
	napi_ref sender;
	napi_ref event;
};
void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args);

#endif