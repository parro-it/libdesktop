#include "libdesktop.h"

#include <yoga/Yoga.h>

DSK_EXTEND_MODULE(libdesktop);


extern DskCtrlIProto DskLayoutContainerProto;

UIHandle dsk_new_container_platform_ui_control();

DSK_DEFINE_CLASS(libdesktop, Container) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	UIHandle widget = dsk_new_container_platform_ui_control();
	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskLayoutContainerProto, init, env, widget, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);
	DSK_CTRLI_CALL(ctrl, add_children, argv[1]);

	return this;
}
