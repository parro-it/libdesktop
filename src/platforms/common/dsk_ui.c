#include "libdesktop.h"

static void widget_children_reposition(napi_env env, UIHandle container, YGNodeRef root) {
	uint32_t childrenCount = YGNodeGetChildCount(root);
	for (uint32_t i = 0; i < childrenCount; i++) {
		YGNodeRef childNode = YGNodeGetChild(root, i);
		UIHandle childHandle = YGNodeGetContext(childNode);
		dsk_widget_reposition(env, container, childHandle, YGNodeLayoutGetLeft(childNode),
							  YGNodeLayoutGetTop(childNode), YGNodeLayoutGetWidth(childNode),
							  YGNodeLayoutGetHeight(childNode));
		widget_children_reposition(env, childHandle, childNode);
	}
}

void dsk_calculate_layout(napi_env env, UIHandle container, YGNodeRef root, float availableWidth,
						  float availableHeight) {
	// YGNodePrint(root, YGPrintOptionsChildren | YGPrintOptionsStyle);
	YGNodeCalculateLayout(root, availableWidth, availableHeight, YGDirectionInherit);
	widget_children_reposition(env, container, root);
	// YGNodePrint(root, YGPrintOptionsChildren | YGPrintOptionsLayout);
}

void dsk_set_children_preferred_sizes(YGNodeRef node, UIHandle widget) {
	uint32_t childrenCount = YGNodeGetChildCount(node);

	if (childrenCount == 0) {
		int width, height;
		dsk_get_preferred_sizes(widget, &width, &height);

		YGNodeStyleSetWidth(node, width);
		YGNodeStyleSetHeight(node, height);
	} else {

		for (uint32_t i = 0; i < childrenCount; i++) {
			YGNodeRef child = YGNodeGetChild(node, i);
			UIHandle child_ui = YGNodeGetContext(child);
			dsk_set_children_preferred_sizes(child, child_ui);
		}
	}
}

void dsk_add_child(napi_env env, UIHandle parentHandle, UIHandle childHandle) {
	napi_value parent = dsk_widget_get_wrapper(env, parentHandle);
	napi_value child = dsk_widget_get_wrapper(env, childHandle);

	// napi_ref ref;
	// napi_create_reference(env,child,1,&ref);

	// printf("handles: %p->%p   values: %p->%p\n",childHandle,parentHandle,child,parent);
	// printf("dsk_platform_container_add_child 1\n");
	dsk_platform_container_add_child(parentHandle, childHandle);

	YGNodeRef node = dsk_widget_get_node(env, parent);
	// printf("node %p\n",node);
	YGNodeRef childNode = dsk_widget_get_node(env, child);
	// printf("YGNodeInsertChild %p %p\n",node,childNode);
	uint32_t childrenCount = YGNodeGetChildCount(node);
	YGNodeInsertChild(node, childNode, childrenCount);
}

static bool add_children(napi_env env, UIHandle widget, napi_value children) {
	DSK_ONERROR_THROW_RET(true);
	DSK_ARRAY_FOREACH(children, {
		UIHandle childHandle = NULL;
		DSK_NAPI_CALL(napi_unwrap(env, dsk_iter_item, (void **)&childHandle));
		dsk_add_child(env, widget, childHandle);
	});
	return false;
}

static bool set_properties(napi_env env, napi_value props, napi_value target) {
	DSK_ONERROR_THROW_RET(true);
	// printf("set_properties 1 %p %p\n", props, target);
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
				if (set_properties(env, propValue, styleProp)) {
					goto dsk_error;
				}
				continue;
			}

			DSK_NAPI_CALL(napi_set_property(env, target, propName, propValue));
		}
	});
	return false;
}

static void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
	napi_value this = (napi_value)finalize_hint;
	YGNodeRef node = dsk_widget_get_node(env, this);
	YGNodeFree(node);
}

DSK_EXTEND_CLASS(libdesktop, Style);

napi_status dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this, napi_value *argv) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	assert(argv != NULL);
	assert(widget != NULL);
	assert(this != NULL);

	napi_value props = argv[0];
	napi_value children = argv[1];

	DSK_NAPI_CALL(napi_wrap(env, this, widget, widget_finalize, this, NULL));

	YGNodeRef node = YGNodeNew();

	assert(node != NULL);

	dsk_widget_set_node(env, this, node);
	YGNodeSetContext(node, widget);

	;
	napi_value null;
	DSK_NAPI_CALL(napi_get_null(env, &null));

	napi_value style = dsk_new_instance(env, libdesktop_Style_ref, 2, (napi_value[]){null, this});
	if (style == NULL) {
		return napi_pending_exception;
	}

	DSK_NAPI_CALL(napi_set_named_property(env, this, "style", style));

	napi_value events;
	DSK_NAPI_CALL(napi_create_object(env, &events));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "events", events));

	if (set_properties(env, props, this)) {
		return napi_pending_exception;
	}

	if (add_children(env, widget, children)) {
		return napi_pending_exception;
	}

	return napi_ok;
}
