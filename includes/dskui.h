#ifndef DSK_CONTROL_H__
#define DSK_CONTROL_H__ 1

//#include "napi_utils.h"
#include "yoga/Yoga.h"

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget);
void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node);

typedef void *UIHandle;
void dsk_platform_container_add_child(UIHandle parent, UIHandle child);
void dsk_add_child(napi_env env, UIHandle parentHandle, UIHandle childHandle);
void dsk_append_all_children(napi_env env, UIHandle widget, napi_value children);

void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint);

void dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this);
void dsk_widget_reposition(napi_env env, UIHandle container, UIHandle widget, float x, float y,
						   float width, float height);
// UIHandle dsk_unwrap_widget(napi_env env, napi_value this);
napi_value dsk_widget_wrapper(napi_env env, UIHandle widget);
void dsk_calculate_layout(napi_env env, UIHandle container, YGNodeRef root, float availableWidth,
						  float availableHeight);

bool dsk_set_properties(napi_env env, napi_value props, napi_value target);
void dsk_set_children_preferred_sizes(YGNodeRef node, UIHandle widget);
void dsk_get_preferred_sizes(UIHandle widget, int *width, int *height);

napi_value dsk_event_new_for_widget(napi_env env, const char *eventname, napi_value sender);

struct dsk_event_args {
	napi_env env;
	napi_ref sender;
	napi_ref event;
};

void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args);

void dsk_ui_set_prop_s(void *instance, char *value, void **datas);
char *dsk_ui_get_prop_s(void *instance, void **datas);
void dsk_ui_set_prop_i32(void *instance, int32_t value, void **datas);
int32_t dsk_ui_get_prop_i32(void *instance, void **datas);
void dsk_ui_set_prop_bool(void *instance, bool value, void **datas);
bool dsk_ui_get_prop_bool(void *instance, void **datas);
/*
#define DSK_UNWRAP_WIDGET()                                                                        \
	{                                                                                              \
		napi_status status = napi_unwrap(env, this, (void **)&widget);                             \
		CHECK_STATUS_THROW(status, napi_unwrap);                                                   \
	}

#define LOAD_PROP_NAME()                                                                           \
	char *propname;                                                                                \
	{                                                                                              \
		napi_status status = napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&propname);    \
		CHECK_STATUS_THROW(status, napi_get_cb_info);                                              \
	}
*/
#define DSK_UI_PROP_S(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                       \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropSTR, dsk_setPropSTR,              \
						((void *[]){dsk_ui_get_prop_s, dsk_ui_set_prop_s, UI_NAME}))

#define DSK_UI_PROP_I32(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                     \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropI32, dsk_setPropI32,              \
						((void *[]){dsk_ui_get_prop_i32, dsk_ui_set_prop_i32, UI_NAME}))

#define DSK_UI_PROP_BOOL(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                    \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropBOOL, dsk_setPropBOOL,            \
						((void *[]){dsk_ui_get_prop_bool, dsk_ui_set_prop_bool, UI_NAME}))

/*
#define DSK_RWPROP_S(NAME, UI_NAME)                                                                \
	(napi_property_descriptor) {                                                                   \
		.utf8name = #NAME, .getter = widgetGetPropS, .setter = widgetSetPropS, .data = UI_NAME     \
	}
#define DSK_RWPROP_I32(NAME, UI_NAME)                                                              \
	(napi_property_descriptor) {                                                                   \
		.utf8name = #NAME, .getter = widgetGetPropI32, .setter = widgetSetPropI32, .data = UI_NAME \
	}
#define DSK_CHILDPROP_I32(NAME, UI_NAME)                                                           \
	(napi_property_descriptor) {                                                                   \
		.utf8name = #NAME, .getter = widgetGetChildPropI32, .setter = widgetSetChildPropI32,       \
		.data = UI_NAME                                                                            \
	}
#define DSK_RWPROP_BOOL(NAME, UI_NAME)                                                             \
	(napi_property_descriptor) {                                                                   \
		.utf8name = #NAME, .getter = widgetGetPropBool, .setter = widgetSetPropBool,               \
		.data = UI_NAME                                                                            \
	}

LIBUI_FUNCTION(widgetSetPropS);
LIBUI_FUNCTION(widgetGetPropS);
LIBUI_FUNCTION(widgetSetPropI32);
LIBUI_FUNCTION(widgetGetPropI32);
LIBUI_FUNCTION(widgetSetChildPropI32);
LIBUI_FUNCTION(widgetGetChildPropI32);
LIBUI_FUNCTION(widgetSetPropBool);
LIBUI_FUNCTION(widgetGetPropBool);
*/
#endif