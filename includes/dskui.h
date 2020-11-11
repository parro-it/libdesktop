#ifndef DSK_UI_H__
#define DSK_UI_H__ 1

#include "yoga/Yoga.h"

typedef void *UIHandle;

typedef struct DskControlInterface {
	napi_status (*get_prop)(napi_env env, UIHandle control, const char *prop_name,
							void **prop_value);
	napi_status (*set_prop)(napi_env env, UIHandle control, const char *prop_name,
							void *prop_value);
	napi_status (*preferred_size)(napi_env env, UIHandle control, int *width, int *height);
	napi_status (*reposition)(napi_env env, UIHandle control, int x, int y, int width, int height);

	napi_status (*add_child)(napi_env env, UIHandle container, UIHandle child);
	napi_status (*remove_child)(napi_env env, UIHandle container, UIHandle child);
} DskContainerInterface;

// widget interface, implemented in every native platform

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget);
void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node);
void dsk_get_preferred_sizes(UIHandle widget, int *width, int *height);

void dsk_platform_container_add_child(UIHandle parent, UIHandle child);
void dsk_widget_reposition(napi_env env, UIHandle container, UIHandle widget, float x, float y,
						   float width, float height);

// container interface, implemented in common platform
void dsk_add_child(napi_env env, UIHandle parentHandle, UIHandle childHandle);

// void dsk_add_children(napi_env env, UIHandle widget, napi_value children);
void dsk_calculate_layout(napi_env env, UIHandle container, YGNodeRef root, float availableWidth,
						  float availableHeight);
void dsk_set_children_preferred_sizes(YGNodeRef node, UIHandle widget);

// widget interface, implemented in common platform

napi_status dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this, napi_value *argv);
UIHandle dsk_unwrap_widget(napi_env env, napi_value this);
napi_value dsk_widget_wrapper(napi_env env, UIHandle widget);

// widget events, implemented in common platform

napi_value dsk_event_new_for_widget(napi_env env, const char *eventname, napi_value sender);

struct dsk_event_args {
	napi_env env;
	napi_ref sender;
	napi_ref event;
};

void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args);

// native properties, implemented in every native platform

void dsk_ui_set_prop_s(void *instance, char *value, void **datas);
char *dsk_ui_get_prop_s(void *instance, void **datas);
void dsk_ui_set_prop_i32(void *instance, int32_t value, void **datas);
int32_t dsk_ui_get_prop_i32(void *instance, void **datas);
void dsk_ui_set_prop_bool(void *instance, bool value, void **datas);
bool dsk_ui_get_prop_bool(void *instance, void **datas);

#define DSK_UI_PROP_S(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                       \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropSTR, dsk_setPropSTR,              \
						((void *[]){dsk_ui_get_prop_s, dsk_ui_set_prop_s, UI_NAME}))

#define DSK_UI_PROP_I32(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                     \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropI32, dsk_setPropI32,              \
						((void *[]){dsk_ui_get_prop_i32, dsk_ui_set_prop_i32, UI_NAME}))

#define DSK_UI_PROP_BOOL(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                    \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropBOOL, dsk_setPropBOOL,            \
						((void *[]){dsk_ui_get_prop_bool, dsk_ui_set_prop_bool, UI_NAME}))

#endif