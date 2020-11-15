#include "dsktest.h"
#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);
extern DskCtrlIProto DskControlProto;
DskCtrlIProto DskLayoutContainerProto;

static napi_status add_child(struct DskCtrlI *self, UIHandle child_ui) {
	napi_env env = self->env;
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	DSK_NAPI_CALL(DskControlProto.add_child(self, child_ui));

	DskCtrlI *child;
	DSK_NAPI_CALL(dsk_CtrlI_from_UIHandle(child_ui, &child));

	uint32_t childrenCount = YGNodeGetChildCount(self->yg_node);
	YGNodeInsertChild(self->yg_node, child->yg_node, childrenCount);

	return napi_ok;
}

NAPI_C_CTOR(init) {
	DskLayoutContainerProto.get_prop = DskControlProto.get_prop;
	DskLayoutContainerProto.set_prop = DskControlProto.set_prop;
	DskLayoutContainerProto.get_preferred_size = DskControlProto.get_preferred_size;
	DskLayoutContainerProto.reposition = DskControlProto.reposition;
	DskLayoutContainerProto.add_child = add_child;
	DskLayoutContainerProto.remove_child = DskControlProto.remove_child;
	DskLayoutContainerProto.init = DskControlProto.init;
	DskLayoutContainerProto.assign_props = DskControlProto.assign_props;
	DskLayoutContainerProto.add_children = DskControlProto.add_children;
}

napi_status new_wrapped_Ctrl(DskCtrlIProto *proto, napi_env env, DskCtrlI **ctrl, UIHandle *widget,
							 napi_value *wrapper);

DSK_DEFINE_TEST(tests_DskLayoutContainerProto) {
	DSK_ASSERT(DskLayoutContainerProto.add_child == add_child);

	DskCtrlI *ctrl_parent, *ctrl_child1, *ctrl_child2;
	UIHandle ui_parent, ui_child1, ui_child2;
	napi_value wrapper_parent, wrapper_child1, wrapper_child2;
	{ // setup
		DSK_NAPI_CALL(new_wrapped_Ctrl(&DskLayoutContainerProto, env, &ctrl_parent, &ui_parent,
									   &wrapper_parent));
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_child1, &ui_child1, &wrapper_child1));
		DSK_NAPI_CALL(
			new_wrapped_Ctrl(&DskControlProto, env, &ctrl_child2, &ui_child2, &wrapper_child2));

		napi_value children;
		DSK_NAPI_CALL(napi_create_array_with_length(env, 2, &children));
		DSK_NAPI_CALL(napi_set_element(env, children, 0, wrapper_child1));
		DSK_NAPI_CALL(napi_set_element(env, children, 1, wrapper_child2));

		DSK_NAPI_CALL(DskLayoutContainerProto.add_children(ctrl_parent, children));
	}
	{ // yoga child node added
		DSK_ASSERT(YGNodeGetChildCount(ctrl_parent->yg_node) == 2);
	}
	DSK_END_TEST();
}
DSK_TEST_CLOSE