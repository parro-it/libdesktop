#include "libdesktop.h"

#include "yoga.h"
#include <gtk/gtk.h>

#include <dlfcn.h> // see future.c
#include <gtk/gtk.h>
#include <inttypes.h>
#include <langinfo.h>
#include <math.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <uv.h>

const char *dsk_init() {
	GError *err = NULL;
	const char *msg;

	if (gtk_init_with_args(NULL, NULL, NULL, NULL, NULL, &err) == FALSE) {
		msg = g_strdup(err->message);
		g_error_free(err);
		return msg;
	}
	return NULL;
}

// static gboolean stepsIteration(gboolean) = NULL;

static gboolean stepsQuit = FALSE;

// the only difference is we ignore the return value from gtk_main_iteration_do(), since it will
// always be TRUE if gtk_main() was never called gtk_main_iteration_do() will still run the main
// loop regardless
static gboolean stepsIteration(gboolean block) {
	gtk_main_iteration_do(block);
	return stepsQuit;
}

int dsk_process_ui_event(int wait) {
	gboolean block;

	block = FALSE;
	if (wait)
		block = TRUE;
	return stepsIteration(block) == FALSE;
}

// gtk_main_quit() may run immediately, or it may wait for other pending events; "it depends"
// (thanks mclasen in irc.gimp.net/#gtk+) PostQuitMessage() on Windows always waits, so we must do
// so too we'll do it by using an idle callback
static gboolean quit(gpointer data) {
	stepsQuit = TRUE;
	return FALSE;
}

void dsk_quit(void) {
	gdk_threads_add_idle(quit, NULL);
}

int dsk_ui_events_pending() {
	return gtk_events_pending();
}

int dsk_wait_node_events(uv_loop_t *loop, int timeout) {
	int nodeBackendFd = uv_backend_fd(loop);

	if (nodeBackendFd == -1) {
		fprintf(stderr, "Invalid node backend fd.\n");
		return 0;
	}

	struct epoll_event ev;

	return epoll_wait(nodeBackendFd, &ev, 1, timeout);
}

int dsk_wakeup_ui_loop() {
	g_main_context_wakeup(NULL);
	return 0;
}
/*
YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget) {
	// printf("unwrap %p\n", widget);
	GObject *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		// printf("non ok \n");
		const napi_extended_error_info *result;
		napi_get_last_error_info(env, &result);
		// printf("napi_unwrap failed with code %d: %s\n", result->engine_error_code,
		// result->error_message);
		return NULL;
	}
	// printf("widgetG %p\n",widgetG);
	YGNodeRef node = g_object_get_data(widgetG, "yoganode");
	return node;
}

void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node) {
	GObject *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		return;
	}

	g_object_set_data(widgetG, "yoganode", node);
	napi_ref ref;
	napi_create_reference(env, widget, 1, &ref);

	g_object_set_data(widgetG, "wrapper", ref);
}

napi_value dsk_widget_get_wrapper(napi_env env, UIHandle widget) {
	napi_ref ref = g_object_get_data(G_OBJECT(widget), "wrapper");
	if (ref == NULL) {
		return NULL;
	}
	// printf("ref is %p\n",ref);
	napi_value node;
	napi_get_reference_value(env, ref, &node);
	return node;
}

void dsk_on_event(UIHandle *uihandle, void *data);

void dsk_get_preferred_sizes(UIHandle widget, int *width, int *height) {
	int minwidth, natwidth;
	int minheight, natheight;
	gtk_widget_set_size_request(widget, 0, 0);
	gtk_widget_get_preferred_height(widget, &minheight, &natheight);
	gtk_widget_get_preferred_width(widget, &minwidth, &natwidth);

	// printf("width min %d nat %d\n", minwidth, natwidth);
	// printf("height min %d nat %d\n", minheight, natheight);
	*width = natwidth;
	*height = natheight;
}
*/
void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args) {
	g_signal_connect(G_OBJECT(widget), eventname, G_CALLBACK(dsk_on_event), args);
}
/*
void dsk_ui_set_prop_s(void *instance, char *value, void **datas) {
	GtkWidget *widget = instance;
	char *propname = datas[2];
	printf("propname %s=%s\n", propname, value);
	g_object_set(widget, propname, value, NULL);
}

char *dsk_ui_get_prop_s(void *instance, void **datas) {
	GtkWidget *widget = instance;
	char *propname = datas[2];
	char *result;
	g_object_get(widget, propname, &result, NULL);
	return result;
}

void dsk_ui_set_prop_i32(void *instance, int32_t value, void **datas) {
	GtkWidget *widget = instance;
	char *propname = datas[2];

	if (strcmp(propname, "left") == 0 || strcmp(propname, "top")) {
		GtkContainer *container = GTK_CONTAINER(gtk_widget_get_parent(widget));
		if (container == NULL) {
			printf("ERROR: THIS WIDGET HAS NO CONTAINER\n");
			return;
		}
		GValue val = G_VALUE_INIT;
		g_value_init(&val, G_TYPE_INT);
		g_value_set_int(&val, value);
		gtk_container_child_set_property(container, widget, propname, &val);
		return;
	}

	g_object_set(widget, propname, value, NULL);
}

int32_t dsk_ui_get_prop_i32(void *instance, void **datas) {
	GtkWidget *widget = instance;
	char *propname = datas[2];

	if (strcmp(propname, "left") == 0 || strcmp(propname, "top")) {
		GtkContainer *container = GTK_CONTAINER(gtk_widget_get_parent(widget));
		GValue value;
		gtk_container_child_get_property(container, widget, propname, &value);
		return g_value_get_int(&value);
	}

	int32_t result;
	g_object_get(widget, propname, &result, NULL);

	return result;
}

void dsk_ui_set_prop_bool(void *instance, bool value, void **datas) {
	GtkWidget *widget = instance;
	char *propname = datas[2];
	g_object_set(widget, propname, value, NULL);
}

bool dsk_ui_get_prop_bool(void *instance, void **datas) {
	GtkWidget *widget = instance;
	char *propname = datas[2];
	bool result;
	g_object_get(widget, propname, &result, NULL);
	return result;
}
*/