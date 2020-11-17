#include "dsktest.h"
#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);
DskCtrlIProto DskRootContainerProto;
extern DskCtrlIProto DskLayoutContainerProto;

static napi_status set_preferred_sizes(struct DskCtrlI *self) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	uint32_t childrenCount = YGNodeGetChildCount(self->yg_node);

	if (childrenCount == 0) {
		int width, height;
		DSK_CTRLI_CALL(self, get_preferred_size, &width, &height);

		YGNodeStyleSetWidth(self->yg_node, width);
		YGNodeStyleSetHeight(self->yg_node, height);
	} else {

		for (uint32_t i = 0; i < childrenCount; i++) {
			YGNodeRef child = YGNodeGetChild(self->yg_node, i);
			DskCtrlI *childI;
			DSK_NAPI_CALL(dsk_CtrlI_from_YGNode(child, &childI));
			DSK_NAPI_CALL(set_preferred_sizes(childI));
		}
	}

	return napi_ok;
}

static napi_status reposition(struct DskCtrlI *self, int x, int y, int width, int height) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	set_preferred_sizes(self);

	YGNodePrint(self->yg_node, YGPrintOptionsChildren | YGPrintOptionsStyle);

	YGNodeCalculateLayout(self->yg_node, width == -1 ? YGUndefined : width,
						  height == -1 ? YGUndefined : height, YGDirectionInherit);

	YGNodePrint(self->yg_node, YGPrintOptionsChildren | YGPrintOptionsLayout);

	uint32_t childrenCount = YGNodeGetChildCount(self->yg_node);

	for (uint32_t i = 0; i < childrenCount; i++) {
		YGNodeRef childNode = YGNodeGetChild(self->yg_node, i);

		DskCtrlI *child;
		DSK_NAPI_CALL(dsk_CtrlI_from_YGNode(childNode, &child));

		DSK_CTRLI_CALL(child, reposition, YGNodeLayoutGetLeft(childNode),
					   YGNodeLayoutGetTop(childNode), YGNodeLayoutGetWidth(childNode),
					   YGNodeLayoutGetHeight(childNode));
	}

	DSK_NAPI_CALL(DskLayoutContainerProto.reposition(
		self, -1, -1, YGNodeLayoutGetWidth(self->yg_node), YGNodeLayoutGetHeight(self->yg_node)));

	return napi_ok;
}

NAPI_C_CTOR(init_root_ctrn) {
	DskRootContainerProto.get_prop = DskLayoutContainerProto.get_prop;
	DskRootContainerProto.set_prop = DskLayoutContainerProto.set_prop;
	DskRootContainerProto.get_preferred_size = DskLayoutContainerProto.get_preferred_size;
	DskRootContainerProto.reposition = reposition;
	DskRootContainerProto.add_child = DskLayoutContainerProto.add_child;
	DskRootContainerProto.remove_child = DskLayoutContainerProto.remove_child;
	DskRootContainerProto.init = DskLayoutContainerProto.init;
	DskRootContainerProto.assign_props = DskLayoutContainerProto.assign_props;
	DskRootContainerProto.add_children = DskLayoutContainerProto.add_children;
}

napi_status new_wrapped_Ctrl(DskCtrlIProto *proto, napi_env env, DskCtrlI **ctrl, UIHandle *widget,
							 napi_value *wrapper);

DSK_DEFINE_TEST(tests_DskRootContainerProto) {
	DSK_ASSERT(DskRootContainerProto.reposition == reposition);

	DskCtrlI *ctrl_parent, *ctrl_child1, *ctrl_child2;
	UIHandle ui_parent, ui_child1, ui_child2;
	napi_value wrapper_parent, wrapper_child1, wrapper_child2;
	{ // setup
		DSK_NAPI_CALL(new_wrapped_Ctrl(&DskRootContainerProto, env, &ctrl_parent, &ui_parent,
									   &wrapper_parent));
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_child1, &ui_child1, &wrapper_child1));
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_child2, &ui_child2, &wrapper_child2));

		napi_value children;
		DSK_NAPI_CALL(napi_create_array_with_length(env, 2, &children));
		DSK_NAPI_CALL(napi_set_element(env, children, 0, wrapper_child1));
		DSK_NAPI_CALL(napi_set_element(env, children, 1, wrapper_child2));

		DSK_NAPI_CALL(DskRootContainerProto.add_children(ctrl_parent, children));
		DSK_NAPI_CALL(DskRootContainerProto.reposition(ctrl_parent, 0, 0, 0, 0));
	}
	{ // yoga child node added
		DSK_ASSERT(YGNodeGetChildCount(ctrl_parent->yg_node) == 2);
	}
	DSK_END_TEST();
}
DSK_TEST_CLOSE

#include <gtk/gtk.h>

DSK_DEFINE_CLASS(libdesktop, RootContainer) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	void *widget = gtk_fixed_new();
	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskRootContainerProto, init, env, widget, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);
	DSK_CTRLI_CALL(ctrl, add_children, argv[1]);

	return this;
}