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

#define DSK_TEST_CLOSE                                                                             \
	return NULL;                                                                                   \
	}

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

static napi_status def_add_children_t(struct DskCtrlI *self, napi_value children) {
	return napi_ok;
}

static napi_status def_init_t(DskCtrlIProto *proto, napi_env env, UIHandle ctrl_handle,
							  napi_value js_wrapper, struct DskCtrlI **ctrl) {
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

static napi_status set_properties(napi_env env, napi_value props, napi_value target) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	napi_value names;
	DSK_NAPI_CALL(napi_get_property_names(env, props, &names));
	DSK_ARRAY_FOREACH(names, {
		napi_value propName = dsk_iter_item;
		bool hasProp;

		size_t len;
		DSK_NAPI_CALL(napi_get_value_string_utf8(env, propName, NULL, 0, &len));
		char propName_s[1000];
		DSK_NAPI_CALL(napi_get_value_string_utf8(env, propName, propName_s, len + 1, NULL));

		DSK_NAPI_CALL(napi_has_property(env, target, propName, &hasProp));
		if (hasProp) {
			napi_value propValue;
			DSK_NAPI_CALL(napi_get_property(env, props, propName, &propValue));

			napi_valuetype type;
			DSK_NAPI_CALL(napi_typeof(env, propValue, &type));

			if (type == napi_object) {
				napi_value styleProp;
				DSK_NAPI_CALL(napi_get_property(env, target, propName, &styleProp));
				DSK_NAPI_CALL(set_properties(env, propValue, styleProp));
				continue;
			}

			DSK_NAPI_CALL(napi_set_property(env, target, propName, propValue));
		}
	});

	return napi_ok;
}

static napi_status def_assign_props_t(struct DskCtrlI *self, napi_value props) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	napi_value target;
	DSK_NAPI_CALL(dsk_CtrlI_get_wrapper(self, &target));

	return set_properties(env, props, target);
}

DskCtrlIProto DskCtrlDefaultProto = {
	.get_prop = def_get_prop_t,
	.set_prop = def_set_prop_t,
	.get_preferred_size = def_get_preferred_size_t,
	.reposition = def_reposition_t,
	.add_child = def_add_child_t,
	.remove_child = def_remove_child_t,
	.init = def_init_t,
	.assign_props = def_assign_props_t,
	.add_children = def_add_children_t,
};

static napi_status new_wrapped_Ctrl(napi_env env, DskCtrlI **ctrl, UIHandle *widget,
									napi_value *wrapper) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	DSK_NAPI_CALL(napi_create_object(env, wrapper));
	napi_value num;
	DSK_NAPI_CALL(napi_create_uint32(env, 42, &num));
	DSK_NAPI_CALL(napi_set_named_property(env, *wrapper, "num", num));

	dsk_initui_for_test();

	*widget = dsk_new_test_widget();

	DSK_CTRLI_CALL_STATIC(&DskCtrlDefaultProto, init, env, *widget, *wrapper, ctrl);
	return napi_ok;
}

DSK_DEFINE_TEST(tests_def_assign_props_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	napi_value num, props;
	DSK_NAPI_CALL(napi_create_uint32(env, 4242, &num));
	DSK_NAPI_CALL(napi_create_object(env, &props));
	DSK_NAPI_CALL(napi_set_named_property(env, props, "num", num));

	def_assign_props_t(ctrl, props);

	napi_value num2;
	DSK_NAPI_CALL(napi_get_named_property(env, wrapper, "num", &num2));

	uint32_t res;
	DSK_NAPI_CALL(napi_get_value_uint32(env, num2, &res));
	DSK_ASSERT(res == 4242);

	DSK_END_TEST();
}
DSK_TEST_CLOSE

DSK_DEFINE_TEST(tests_dsk_CtrlIFuncs_init) {
	DSK_ASSERT(DskCtrlDefaultProto.init != NULL);
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

napi_status dsk_CtrlI_get_wrapper(DskCtrlI *ctrl, napi_value *wrapper) {
	return napi_get_reference_value(ctrl->env, ctrl->js_wrapper_ref, wrapper);
}

DSK_DEFINE_TEST(tests_dsk_CtrlI_get_wrapper) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(env, &ctrl, &widget, &wrapper));

	napi_value wrapper2;
	DSK_NAPI_CALL(dsk_CtrlI_get_wrapper(ctrl, &wrapper2));
	DSK_ASSERT(wrapper2 != NULL);

	napi_value num;
	DSK_NAPI_CALL(napi_get_named_property(env, wrapper2, "num", &num));

	uint32_t res;
	DSK_NAPI_CALL(napi_get_value_uint32(env, num, &res));
	DSK_ASSERT(res == 42);

	DSK_END_TEST();
	return NULL;
}
DSK_TEST_CLOSE
