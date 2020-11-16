#include "dsktest.h"
#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);

static void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
	/*napi_value this = (napi_value)finalize_data;
	YGNodeRef node = dsk_widget_get_node(env, this);
	YGNodeFree(node);*/
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

static napi_status def_add_children_t(struct DskCtrlI *self, napi_value children) {
	napi_env env = self->env;

	DSK_ONERROR_THROW_RET(napi_pending_exception);

	DSK_ARRAY_FOREACH(children, {
		struct DskCtrlI *child;
		DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, dsk_iter_item, &child));
		printf("add_Child %p\n", self->proto->add_child);
		DSK_CTRLI_CALL(self, add_child, child->ctrl_handle);
	});
	return napi_ok;
}

// implemented by each platform
napi_status dsk_platform_get_prop_t(struct DskCtrlI *self, const char *prop_name,
									dsk_prop_types prop_type, ...);
napi_status dsk_platform_set_prop_t(struct DskCtrlI *self, const char *prop_name,
									dsk_prop_types prop_type, ...);
napi_status dsk_platform_get_preferred_size_t(struct DskCtrlI *self, int *width, int *height);
napi_status dsk_platform_reposition_t(struct DskCtrlI *self, int x, int y, int width, int height);
napi_status dsk_platform_add_child_t(struct DskCtrlI *self, UIHandle child);
napi_status dsk_platform_remove_child_t(struct DskCtrlI *self, UIHandle child);

DskCtrlIProto DskControlProto = {
	.get_prop = dsk_platform_get_prop_t,
	.set_prop = dsk_platform_set_prop_t,
	.get_preferred_size = dsk_platform_get_preferred_size_t,
	.reposition = dsk_platform_reposition_t,
	.add_child = dsk_platform_add_child_t,
	.remove_child = dsk_platform_remove_child_t,
	.init = def_init_t,
	.assign_props = def_assign_props_t,
	.add_children = def_add_children_t,
};

napi_status new_wrapped_Ctrl(DskCtrlIProto *proto, napi_env env, DskCtrlI **ctrl, UIHandle *widget,
							 napi_value *wrapper) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	DSK_NAPI_CALL(napi_create_object(env, wrapper));
	napi_value num, style;
	DSK_NAPI_CALL(napi_create_uint32(env, 42, &num));
	DSK_NAPI_CALL(napi_set_named_property(env, *wrapper, "num", num));
	DSK_NAPI_CALL(napi_create_object(env, &style));
	DSK_NAPI_CALL(napi_set_named_property(env, style, "stylenum", num));
	DSK_NAPI_CALL(napi_set_named_property(env, *wrapper, "style", style));

	dsk_initui_for_test();

	*widget = dsk_new_test_widget();

	DSK_CTRLI_CALL_STATIC(proto, init, env, *widget, *wrapper, ctrl);
	return napi_ok;
}

static napi_status test_add_child_t(struct DskCtrlI *self, UIHandle child) {
	self->yg_node = (void *)((uintptr_t)child + (uintptr_t)self->yg_node);
	return napi_ok;
}

DSK_DEFINE_TEST(tests_def_add_children_t) {
	DskCtrlI *ctrl_parent, *ctrl_child1, *ctrl_child2;
	UIHandle ui_parent, ui_child1, ui_child2;
	napi_value wrapper_parent, wrapper_child1, wrapper_child2;
	{ // setup
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_parent, &ui_parent, &wrapper_parent));
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_child1, &ui_child1, &wrapper_child1));
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_child2, &ui_child2, &wrapper_child2));

		napi_value children;
		DSK_NAPI_CALL(napi_create_array_with_length(env, 2, &children));
		DSK_NAPI_CALL(napi_set_element(env, children, 0, wrapper_child1));
		DSK_NAPI_CALL(napi_set_element(env, children, 1, wrapper_child2));

		add_child_t *old_add_child = ctrl_parent->proto->add_child;
		ctrl_parent->yg_node = 0;
		ctrl_parent->proto->add_child = test_add_child_t;
		def_add_children_t(ctrl_parent, children);
		ctrl_parent->proto->add_child = old_add_child;
		// printf("1 %ld\n", (uintptr_t)ui_child1);
		// printf("2 %ld\n", (uintptr_t)ui_child2);
		// printf("1+2 %ld\n", (uintptr_t)ui_child1 + (uintptr_t)ui_child2);
		// printf("yoga %ld\n", (uintptr_t)ctrl_parent->yg_node);
		DSK_ASSERT(ctrl_parent->yg_node == (void *)((uintptr_t)ui_child1 + (uintptr_t)ui_child2));
	}

	DSK_END_TEST();
}
DSK_TEST_CLOSE

DSK_DEFINE_TEST(tests_def_assign_props_t) {
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	{ // setup
		DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

		napi_value num, props, style;
		DSK_NAPI_CALL(napi_create_uint32(env, 4242, &num));
		DSK_NAPI_CALL(napi_create_object(env, &props));
		DSK_NAPI_CALL(napi_create_object(env, &style));
		DSK_NAPI_CALL(napi_set_named_property(env, style, "stylenum", num));
		DSK_NAPI_CALL(napi_set_named_property(env, props, "num", num));
		DSK_NAPI_CALL(napi_set_named_property(env, props, "style", style));
		DSK_NAPI_CALL(napi_set_named_property(env, props, "nonexistent", num));
		DSK_NAPI_CALL(napi_set_named_property(env, style, "nonexistent", num));

		def_assign_props_t(ctrl, props);
	}

	{ // already existing props are setted in wrapper
		napi_value num;
		DSK_NAPI_CALL(napi_get_named_property(env, wrapper, "num", &num));

		uint32_t res;
		DSK_NAPI_CALL(napi_get_value_uint32(env, num, &res));
		DSK_ASSERT(res == 4242);
	}

	{ // non already existing props are not setted in wrapper
		napi_value num;
		DSK_NAPI_CALL(napi_get_named_property(env, wrapper, "nonexistent", &num));
		napi_valuetype res;
		DSK_NAPI_CALL(napi_typeof(env, num, &res));
		DSK_ASSERT(res == napi_undefined);
	}

	{ // props of sub-objects are deeply assigned
		napi_value style;
		DSK_NAPI_CALL(napi_get_named_property(env, wrapper, "style", &style));

		napi_value stylenum;
		DSK_NAPI_CALL(napi_get_named_property(env, style, "stylenum", &stylenum));

		uint32_t res;
		DSK_NAPI_CALL(napi_get_value_uint32(env, stylenum, &res));
		DSK_ASSERT(res == 4242);
	}

	{ // non already existing props are not setted in sub-objects
		napi_value style;
		DSK_NAPI_CALL(napi_get_named_property(env, wrapper, "style", &style));

		napi_value stylenum;
		DSK_NAPI_CALL(napi_get_named_property(env, style, "nonexistent", &stylenum));

		napi_valuetype res;
		DSK_NAPI_CALL(napi_typeof(env, stylenum, &res));
		DSK_ASSERT(res == napi_undefined);
	}
	DSK_END_TEST();
}
DSK_TEST_CLOSE

DSK_DEFINE_TEST(tests_dsk_CtrlIFuncs_init) {
	DSK_ASSERT(DskControlProto.init != NULL);
	DskCtrlI *ctrl = NULL;
	UIHandle widget;
	napi_value wrapper;
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

	DSK_ASSERT(ctrl != NULL);
	DSK_ASSERT(ctrl->env == env);
	DSK_ASSERT(ctrl->ctrl_handle == widget);
	DSK_ASSERT(ctrl->js_wrapper_ref != NULL);
	DSK_ASSERT(ctrl->proto == &DskControlProto);

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
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

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
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

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
	DSK_NAPI_CALL(new_wrapped_Ctrl(&DskControlProto, env, &ctrl, &widget, &wrapper));

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

napi_status dsk_ui_getter(void *self, void **datas, ...) {

	DskCtrlI *ctrl = self;
	napi_env env = ctrl->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	dsk_prop_types prop_type = (dsk_prop_types)datas[2];
	char *prop_name = datas[3];

	va_list value_valist;

	va_start(value_valist, datas);

	void *value = va_arg(value_valist, void *);
	DSK_CTRLI_CALL(ctrl, set_prop, prop_name, prop_type, value);

	va_end(value_valist); /* Clean up. */

	return napi_ok;
}

napi_status dsk_ui_setter(void *self, void **datas, ...) {

	DskCtrlI *ctrl = self;
	napi_env env = ctrl->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	char *prop_name = datas[3];
	dsk_prop_types prop_type = (dsk_prop_types)datas[2];

	va_list value_valist;

	va_start(value_valist, datas);

	switch (prop_type) {
	case dsk_prop_i32: {
		int32_t value = va_arg(value_valist, int32_t);
		DSK_CTRLI_CALL(ctrl, set_prop, prop_name, prop_type, value);

		break;
	}
	case dsk_prop_str: {
		char *value = va_arg(value_valist, char *);
		DSK_CTRLI_CALL(ctrl, set_prop, prop_name, prop_type, value);

		break;
	}
	case dsk_prop_f64: {
		double value = va_arg(value_valist, double);
		DSK_CTRLI_CALL(ctrl, set_prop, prop_name, prop_type, value);

		break;
	}
	case dsk_prop_bool: {
		int value = va_arg(value_valist, int);
		DSK_CTRLI_CALL(ctrl, set_prop, prop_name, prop_type, value);

		break;
	}
	case dsk_prop_date: {
		break;
	}
	}

	va_end(value_valist); /* Clean up. */

	return napi_ok;
}
