#include "dsktest.h"
#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);
extern DskCtrlIProto DskControlProto;

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

DskCtrlIProto DskLayoutContainerProto;

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

DSK_DEFINE_TEST(tests_DskLayoutContainerProto) {
	DSK_ASSERT(DskLayoutContainerProto.add_child != DskControlProto.add_child);
	DSK_END_TEST();
}
DSK_TEST_CLOSE