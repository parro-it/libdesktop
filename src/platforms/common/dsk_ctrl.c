#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);

/////////////////////////////////////////
//  TESTS
/////////////////////////////////////////

DSK_DEFINE_CLASS(libdesktop, NativeTests) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(0);
	return this;
}

napi_status dsk_assert(napi_env env, napi_value t, bool assertion) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	napi_value assertTrue;
	DSK_NAPI_CALL(napi_get_named_property(env, t, "true", &assertTrue));
	napi_value ignored;
	napi_value assertion_res;
	DSK_NAPI_CALL(napi_get_boolean(env, assertion, &assertion_res));
	DSK_NAPI_CALL(napi_call_function(env, t, assertTrue, 1, &assertion_res, &ignored));

	return napi_ok;
}

napi_status dsk_end_test(napi_env env, napi_value t) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	napi_value end;
	napi_value ignored;
	DSK_NAPI_CALL(napi_get_named_property(env, t, "end", &end));
	DSK_NAPI_CALL(napi_call_function(env, t, end, 0, NULL, &ignored));

	return napi_ok;
}

#define DSK_ASSERT(ASSERTION) DSK_NAPI_CALL(dsk_assert(env, t, ASSERTION))
#define DSK_END_TEST() DSK_NAPI_CALL(dsk_end_test(env, t))

#define DSK_TEST_CLOSE }

#define DSK_DEFINE_TEST(NAME)                                                                      \
	DSK_DEFINE_STATIC_METHOD(libdesktop, NativeTests, NAME) {                                      \
		DSK_JS_FUNC_INIT();                                                                        \
		DSK_EXACTLY_NARGS(1);                                                                      \
		napi_value t = argv[0];

void *dsk_new_test_widget();

// test functions
void dsk_initui_for_test();

/////////////////////////////////////////
// END TESTS FN
/////////////////////////////////////////

static napi_status def_get_prop_t(struct DskCtrlI *self, const char *prop_name, void **prop_value) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

static napi_status def_set_prop_t(struct DskCtrlI *self, const char *prop_name, void *prop_value) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

static napi_status def_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

static napi_status def_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

static napi_status def_add_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

static napi_status def_remove_child_t(struct DskCtrlI *self, UIHandle child) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);
	DSK_NAPI_CALL(napi_throw_error(env, NULL, "Not implemented"));
	return napi_pending_exception;
}

static void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
	/*napi_value this = (napi_value)finalize_data;
	YGNodeRef node = dsk_widget_get_node(env, this);
	YGNodeFree(node);*/
}

static napi_status def_wrap_t(DskCtrlIProto *proto, napi_env env, UIHandle ctrl_handle,
							  napi_value js_wrapper, napi_value props, napi_value children,
							  struct DskCtrlI **ctrl) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	struct DskCtrlI *instance = calloc(1, sizeof(struct DskCtrlI));
	if (instance == NULL) {
		return napi_generic_failure;
	}
	instance->env = env;
	instance->proto = proto;

	instance->yg_node = YGNodeNew();
	YGNodeSetContext(instance->yg_node, instance);

	instance->ctrl_handle = ctrl_handle;
	DSK_NAPI_CALL(dsk_CtrlI_link_UIHandle(ctrl_handle, instance));

	DSK_NAPI_CALL(
		napi_wrap(env, js_wrapper, instance, widget_finalize, NULL, &instance->js_wrapper_ref));

	*ctrl = instance;
	return napi_ok;
}

DskCtrlIProto DskCtrlDefaultProto = {
	.get_prop = def_get_prop_t,
	.set_prop = def_set_prop_t,
	.get_preferred_size = def_get_preferred_size_t,
	.reposition = def_reposition_t,
	.add_child = def_add_child_t,
	.remove_child = def_remove_child_t,
	.wrap = def_wrap_t,
};

static napi_status new_wrapped_Ctrl(napi_env env, DskCtrlI **ctrl, UIHandle *widget,
									napi_value *wrapper) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	DSK_NAPI_CALL(napi_create_object(env, wrapper));
	dsk_initui_for_test();

	*widget = dsk_new_test_widget();

	DSK_CTRLI_CALL_STATIC(&DskCtrlDefaultProto, wrap, env, *widget, *wrapper, NULL, NULL, ctrl);
	return napi_ok;
}

DSK_DEFINE_TEST(tests_dsk_CtrlIFuncs_mk_default) {
	DSK_ASSERT(DskCtrlDefaultProto.wrap != NULL);
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	DSK_ASSERT(ctrl != NULL);
	DSK_ASSERT(ctrl->env == env);
	DSK_ASSERT(ctrl->ctrl_handle == widget);
	DSK_ASSERT(ctrl->js_wrapper_ref != NULL);
	DSK_ASSERT(ctrl->proto == &DskCtrlDefaultProto);

	void *ctrl_from_wrapper;
	DSK_NAPI_CALL(napi_unwrap(env, wrapper, &ctrl_from_wrapper));
	DSK_ASSERT(ctrl == ctrl_from_wrapper);

	void *ctrl_from_yg_node = YGNodeGetContext(ctrl->yg_node);
	DSK_ASSERT(ctrl->yg_node != NULL);
	DSK_ASSERT(ctrl_from_yg_node == ctrl);

	DskCtrlI *ctrl_from_uihandle = NULL;
	DSK_NAPI_CALL(dsk_CtrlI_from_UIHandle(widget, &ctrl_from_uihandle));
	// printf("ctrl_from_uihandle %p\n", ctrl_from_uihandle);
	DSK_ASSERT(ctrl == ctrl_from_uihandle);

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE

napi_status dsk_CtrlI_from_YGNode(YGNodeRef node_ref, DskCtrlI **ctrl) {
	*ctrl = YGNodeGetContext(node_ref);
	return napi_ok;
}

DSK_DEFINE_TEST(tests_dsk_CtrlI_from_UIHandle) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	DskCtrlI *ctrl_from_YGNode = NULL;
	DSK_NAPI_CALL(dsk_CtrlI_from_YGNode(ctrl->yg_node, &ctrl_from_YGNode));
	DSK_ASSERT(ctrl == ctrl_from_YGNode);

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE

napi_status dsk_CtrlI_from_wrapper(napi_env env, napi_value wrapper, DskCtrlI **ctrl) {
	return napi_unwrap(env, wrapper, (void **)ctrl);
}

DSK_DEFINE_TEST(tests_dsk_CtrlI_from_wrapper) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	DskCtrlI *ctrl_from_wrapper = NULL;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, wrapper, &ctrl_from_wrapper));
	DSK_ASSERT(ctrl == ctrl_from_wrapper);

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE
