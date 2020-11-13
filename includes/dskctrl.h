#ifndef DSK_CTRL_H__
#define DSK_CTRL_H__ 1

#include "dsknapi.h"
#include "node_api.h"
#include "yoga/Yoga.h"

struct DskCtrlI;
struct DskCtrlIProto;

typedef void *UIHandle;

typedef napi_status get_prop_t(struct DskCtrlI *self, const char *prop_name, void **prop_value);
typedef napi_status set_prop_t(struct DskCtrlI *self, const char *prop_name, void *prop_value);
typedef napi_status get_preferred_size_t(struct DskCtrlI *self, int *width, int *height);
typedef napi_status reposition_t(struct DskCtrlI *self, int x, int y, int width, int height);
typedef napi_status add_child_t(struct DskCtrlI *self, UIHandle child);
typedef napi_status remove_child_t(struct DskCtrlI *self, UIHandle child);
typedef napi_status free_t(struct DskCtrlI *self);
typedef napi_status wrap_t(struct DskCtrlIProto *proto, napi_env env, UIHandle ctrl_handle,
						   napi_value js_wrapper, napi_value props, napi_value children,
						   struct DskCtrlI **ctrl);

typedef struct DskCtrlIProto {
	get_prop_t *get_prop;
	set_prop_t *set_prop;
	get_preferred_size_t *get_preferred_size;
	reposition_t *reposition;
	add_child_t *add_child;
	remove_child_t *remove_child;
	free_t *free;
	wrap_t *wrap;
} DskCtrlIProto;

typedef struct DskCtrlI {
	napi_env env;
	UIHandle ctrl_handle;
	YGNodeRef yg_node;
	napi_ref js_wrapper_ref;
	DskCtrlIProto *proto;
} DskCtrlI;

extern DskCtrlIProto DskCtrlDefaultProto;

#define DSK_CTRLI_CALL(INSTANCE, METHOD, ...)                                                      \
	DSK_NAPI_CALL((INSTANCE)->proto->METHOD((INSTANCE), __VA_ARGS__))

#define DSK_CTRLI_CALL_STATIC(INSTANCE, METHOD, ...)                                               \
	DSK_NAPI_CALL((INSTANCE)->METHOD((INSTANCE), __VA_ARGS__))

napi_status dsk_CtrlI_get_wrapper(DskCtrlI *ctrl, napi_value *wrapper);

napi_status dsk_CtrlI_set_UIHandle(UIHandle UI_ctrl, DskCtrlI *ctrl);

napi_status dsk_CtrlI_from_UIHandle(UIHandle UI_ctrl, DskCtrlI **ctrl);
napi_status dsk_CtrlI_from_YGNode(YGNodeRef node_ref, DskCtrlI **ctrl);
napi_status dsk_CtrlI_from_wrapper(napi_value wrapper, DskCtrlI **ctrl);

/* reimplemented on top of DskCtrlI ???????? */
/*
napi_value dsk_event_new_for_widget(napi_env env, const char *eventname, napi_value sender);

struct dsk_event_args {
	napi_env env;
	napi_ref sender;
	napi_ref event;
};
void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args);

#define DSK_UI_PROP_S(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                       \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropSTR, dsk_setPropSTR,              \
						((void *[]){dsk_ui_get_prop_s, dsk_ui_set_prop_s, UI_NAME}))

#define DSK_UI_PROP_I32(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                     \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropI32, dsk_setPropI32,              \
						((void *[]){dsk_ui_get_prop_i32, dsk_ui_set_prop_i32, UI_NAME}))

#define DSK_UI_PROP_BOOL(MODNAME, CLASSNAME, PROPNAME, UI_NAME)                                    \
	DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, dsk_getPropBOOL, dsk_setPropBOOL,            \
						((void *[]){dsk_ui_get_prop_bool, dsk_ui_set_prop_bool, UI_NAME}))
*/
#endif