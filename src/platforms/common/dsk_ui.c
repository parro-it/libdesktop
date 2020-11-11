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

void dsk_add_child(napi_env env, UIHandle parentHandle, UIHandle childHandle) {
	napi_value parent = dsk_widget_wrapper(env, parentHandle);
	napi_value child = dsk_widget_wrapper(env, childHandle);

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

#define DSK_VOID()

void dsk_add_children(napi_env env, UIHandle widget, napi_value children) {
	DSK_ONERROR_UNCAUGHT_RET(DSK_VOID());

	uint32_t len;
	DSK_NAPI_CALL(napi_get_array_length(env, children, &len));
	// printf("len %d\n", len);
	for (uint32_t i = 0; i < len; i++) {
		// printf("i %d\n", i);
		napi_value idx;
		DSK_NAPI_CALL(napi_create_uint32(env, i, &idx));
		napi_value child;
		DSK_NAPI_CALL(napi_get_property(env, children, idx, &child));
		// napi_valuetype type;
		// DSK_NAPI_CALL(napi_typeof(env, child, &type));
		// printf("napi_get_property %d %p\n", type==napi_null, child);

		UIHandle childHandle = NULL;
		DSK_NAPI_CALL(napi_unwrap(env, child, (void **)&childHandle));
		// printf("napi_unwrap childHandle %d: %p\n", i, childHandle);

		dsk_add_child(env, widget, childHandle);
	}
}

static void widget_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
	napi_value this = (napi_value)finalize_hint;
	YGNodeRef node = dsk_widget_get_node(env, this);
	YGNodeFree(node);
}

DSK_EXTEND_CLASS(libdesktop, Style);

static bool set_properties(napi_env env, napi_value props, napi_value target) {
	char *dsk_error_msg = NULL;
	// printf("set_properties 1 %p %p\n", props, target);
	napi_value names;
	DSK_NAPI_CALL(napi_get_property_names(env, props, &names));
	// printf("napi_get_property_names\n");
	uint32_t len;
	DSK_NAPI_CALL(napi_get_array_length(env, names, &len));

	// printf("set_properties 2\n");
	for (uint32_t i = 0; i < len; i++) {
		// printf("set_properties 3 %d\n", i);
		napi_value idx;
		napi_value propName;
		bool hasProp;

		DSK_NAPI_CALL(napi_create_uint32(env, i, &idx));
		DSK_NAPI_CALL(napi_get_property(env, names, idx, &propName));

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
				// printf("recurse on %s\n",propName_s);
				DSK_NAPI_CALL(napi_get_property(env, target, propName, &styleProp));
				if (set_properties(env, propValue, styleProp)) {
					goto dsk_error;
				}
				continue;
			}

			// printf("set property %s\n",propName_s);

			DSK_NAPI_CALL(napi_set_property(env, target, propName, propValue));
		}
	}
	// printf("set_properties 1000\n");
	return false;
dsk_error:
	napi_throw_error(env, NULL, dsk_error_msg);
	return true;
}

napi_status dsk_wrap_widget(napi_env env, UIHandle widget, napi_value this, napi_value props) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	assert(widget != NULL);
	assert(this != NULL);

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

	return napi_ok;
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