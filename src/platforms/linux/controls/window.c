#include "libdesktop.h"
#include <gtk/gtk.h>

#define MODULE "win"

/*
static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}
*/
DSK_EXTEND_MODULE(libdesktop);
DSK_EXTEND_CLASS(libdesktop, Container);

struct on_resize_args {
	GtkWidget *container;
	napi_env env;
};

static bool on_window_resize(GtkWindow *window, GdkEvent *event, gpointer data) {
	return false;
	/*
		struct on_resize_args *args = data;
		napi_env env = args->env;
		char *dsk_error_msg;
		napi_handle_scope handle_scope;
		DSK_NAPI_CALL(napi_open_handle_scope(env, &handle_scope));

		napi_value wrapper = dsk_widget_get_wrapper(env, args->container);

		YGNodeRef root = dsk_widget_get_node(args->env, wrapper);

		dsk_set_children_preferred_sizes(root, args->container);

		int uw;
		int uh;
		gtk_window_get_size(window, &uw, &uh);

		// // printf("dsk_calculate_layout %dx%d\n",uw,uh);

		dsk_calculate_layout(args->env, args->container, root, (float)uw, (float)uh);

		DSK_NAPI_CALL(napi_close_handle_scope(env, handle_scope));

		return false;
	dsk_error:;
		napi_value uncErr;
		napi_value err_msg;
		napi_create_string_utf8(env, dsk_error_msg, NAPI_AUTO_LENGTH, &err_msg);
		napi_create_error(env, NULL, err_msg, &uncErr);
		napi_fatal_exception(env, uncErr);
		return false;
	*/
}

// extern DskCtrlIProto DskRootContainerProto;

DSK_EXTEND_CLASS(libdesktop, RootContainer)

DSK_DEFINE_CLASS(libdesktop, Window) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	// // printf("WINDOWS NEW\n");
	GtkWindow *window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	/*GError *err = NULL;
	gtk_window_set_icon_from_file(
		window, "/home/parroit/Desktop/parro-it/libdesktop/example/icon.png", &err);
	if (err != NULL) {
		DSK_FAILURE(err->message);
	}*/

	gtk_widget_add_events(GTK_WIDGET(window), GDK_BUTTON_PRESS_MASK);
	gtk_window_set_resizable(window, true);

	DskCtrlI *ctrl;
	DSK_CTRLI_CALL_STATIC(&DskControlProto, init, env, window, this, &ctrl);
	DSK_CTRLI_CALL(ctrl, assign_props, argv[0]);

	napi_value props;

	DSK_NAPI_CALL(napi_create_object(env, &props));

	bool hasStyle;
	DSK_NAPI_CALL(napi_has_named_property(env, argv[0], "style", &hasStyle));
	if (hasStyle) {
		napi_value containerStyle;
		DSK_NAPI_CALL(napi_get_named_property(env, argv[0], "style", &containerStyle));
		DSK_NAPI_CALL(napi_set_named_property(env, props, "style", containerStyle));
	}

	napi_value cntr_wrapper =
		dsk_new_instance(env, libdesktop_RootContainer_ref, 2, (napi_value[]){props, argv[1]});

	DskCtrlI *cntr_ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, cntr_wrapper, &cntr_ctrl));

	GtkWidget *cntr_gtk = cntr_ctrl->ctrl_handle;

	gtk_container_add(GTK_CONTAINER(window), cntr_gtk);

	gtk_window_set_position(window, GTK_WIN_POS_CENTER);
	gtk_widget_show_all(GTK_WIDGET(window));

	DSK_CTRLI_CALL(cntr_ctrl, reposition, -1, -1, -1, -1);

	float w = YGNodeLayoutGetWidth(cntr_ctrl->yg_node);
	float h = YGNodeLayoutGetHeight(cntr_ctrl->yg_node);
	float pd = YGNodeLayoutGetPadding(cntr_ctrl->yg_node, YGEdgeRight);

	int uw;
	int uh;
	gtk_window_get_size(window, &uw, &uh);

	gtk_window_resize(window, (int)w + pd, (int)h);

	struct on_resize_args *args = malloc(sizeof(struct on_resize_args));
	args->env = env;
	args->container = cntr_gtk;
	g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(on_window_resize), args);

	return this;
}

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdk.h>

DSK_DEFINE_METHOD(libdesktop, Window, saveAsPNGImage) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);

	napi_value filename = argv[0];

	struct DskCtrlI *ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, this, &ctrl));

	GtkWindow *win = ctrl->ctrl_handle;

	gint width, height;
	gtk_window_get_size(win, &width, &height);

	GdkWindow *g_win = gtk_widget_get_window(GTK_WIDGET(win));

	char *c_filename;
	DSK_NAPI_CALL(dsk_get_utf8_cstr(env, filename, &c_filename));

	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
	cairo_t *cr = cairo_create(surface);

	gdk_cairo_set_source_window(cr, g_win, 0, 0);

	cairo_paint(cr);

	cairo_surface_write_to_png(surface, c_filename);

	return NULL;
}

DSK_DEFINE_METHOD(libdesktop, Window, close) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(0);

	struct DskCtrlI *ctrl;
	DSK_NAPI_CALL(dsk_CtrlI_from_wrapper(env, this, &ctrl));

	GtkWindow *win = ctrl->ctrl_handle;
	gtk_window_close(win);

	return NULL;
}

DSK_UI_PROP(libdesktop, Window, left, dsk_prop_i32, "x");
DSK_UI_PROP(libdesktop, Window, top, dsk_prop_i32, "y");
DSK_UI_PROP(libdesktop, Window, width, dsk_prop_i32, "default-width");
DSK_UI_PROP(libdesktop, Window, height, dsk_prop_i32, "default-height");
DSK_UI_PROP(libdesktop, Window, title, dsk_prop_str, "title");
DSK_UI_PROP(libdesktop, Window, visible, dsk_prop_bool, "visible");
