#include "libdesktop-host.h"
#include "libdesktop.h"
#include <assert.h>
#include <stdbool.h>

static uv_barrier_t all_threads_are_waiting;
static uv_barrier_t all_threads_are_awaked;

static napi_ref event_loop_closed_cb_ref = NULL;
static napi_ref event_loop_started_cb_ref = NULL;
static napi_env resolution_env = NULL;

static uv_async_t *keep_alive;

static uv_thread_t thread;
static uv_timer_t main_thread_timer;

/*
	wait for pending events

*/
void wait_node_io(int timeout) {
	int ret;
	DSK_DEBUG_F("--- entering wait with timeout %d", timeout);
	do {
		ret = dsk_wait_node_events(uv_default_loop(), timeout);
	} while (ret == -1 && errno == EINTR);
	DSK_DEBUG("--- wait done");
}

/*
	This function is executed in the
	background thread and is responsible for continuosly polling
	the node backend for pending events.

	When pending node events are found, the main GUI
	thread, if it's waiting, is wake up by calling dsk_wakeup_ui_loop().
*/
static void background_thread(void *arg) {
	DSK_DEBUG("--- start background_thread ");

	while (!ln_get_main_thread_quitted()) {
		DSK_DEBUG("--- wait on all_threads_are_waiting.");

		ln_set_background_thread_waiting(true);

		// wait for the main thread
		// to be blocked waiting for GUI events
		uv_barrier_wait(&all_threads_are_waiting);
		DSK_DEBUG("--- all_threads_are_waiting passed.");

		int timeout = uv_backend_timeout(uv_default_loop());
		DSK_DEBUG_F("--- uv_backend_timeout == %d", timeout);

		// if timeout == 0, it means there are
		// already some callback to execute, so no
		// IO wait is necessary.
		// If timeout == -1, it means to wait forever.
		// When timeout > 0. it means there are one or more timers
		// pending, and the first one will expire in
		// `timeout` ms

		if (timeout != 0) {
			DSK_DEBUG("--- wait node io");
			wait_node_io(timeout);
			DSK_DEBUG("--- done wait node io");

			if (ln_get_main_thread_waiting()) {
				DSK_DEBUG("--- wake up main thread");
				ln_set_background_thread_waiting(false);
				dsk_wakeup_ui_loop();
			}
		} else {
			ln_set_background_thread_waiting(false);
		}

		// wait for the main thread
		// to be unblocked.
		uv_barrier_wait(&all_threads_are_awaked);
	}

	DSK_DEBUG("--- background terminating.");
}

/*
	This function run all pending native GUI event in the loop
	using libui calls.

	It first do a blocking call to dsk_process_ui_event that
	wait for pending GUI events. This blocking call also exit
	when there are pending node events, because dsk_wakeup_ui_loop
	function posts a GUI event
	from the background thread for this purpose.
 */
static void main_thread(uv_timer_t *handle) {
	enum ln_loop_status status = ln_get_loop_status();
	DSK_DEBUG_F("+++ start main_thread with status %d", status);
	uv_timer_stop(handle);
	if (status == starting) {
		assert(event_loop_started_cb_ref != NULL);

		DSK_DEBUG("🧐 LOOP STARTED");
		napi_ref ref = event_loop_started_cb_ref;
		event_loop_started_cb_ref = NULL;
		ln_set_loop_status(started);
		dsk_call_cb(resolution_env, ref);
	}

	DSK_DEBUG("+++ wait on all_threads_are_waiting");

	// wait for the background thread
	// to be blocked waiting for node events
	uv_barrier_wait(&all_threads_are_waiting);

	DSK_DEBUG("+++ passed all_threads_are_waiting");

	int timeout = uv_backend_timeout(uv_default_loop());

	int gui_running = 1;

	if (timeout != 0) {

		DSK_DEBUG("+++ wait GUI events");

		ln_set_main_thread_waiting(true);
		DSK_DEBUG("+++ ln_set_main_thread_waiting true");

		gui_running = dsk_process_ui_event(true);
		DSK_DEBUG("+++ uiWaitForEvents done");

		ln_set_main_thread_waiting(false);
		DSK_DEBUG("+++ ln_set_main_thread_waiting false");
	}

	if (ln_get_background_thread_waiting()) {
		DSK_DEBUG("+++ wake up background thread");
		uv_async_send(keep_alive);
	}

	/* dequeue and run every event pending */
	while (gui_running && dsk_ui_events_pending()) {
		gui_running = dsk_process_ui_event(false);
	}
	DSK_DEBUG("+++ all GUI events worked.");

	if (!gui_running && ln_get_loop_status() == stopping) {
		ln_set_main_thread_quitted(true);
	}

	// wait for the background thread
	// to be unblocked from waiting for node events
	DSK_DEBUG("+++ wait all_threads_are_awaked");
	uv_barrier_wait(&all_threads_are_awaked);
	DSK_DEBUG("+++ passed all_threads_are_awaked");

	DSK_DEBUG_F("+++ libui is running: %d", gui_running);

	if (gui_running || ln_get_loop_status() != stopping) {
		DSK_DEBUG("+++ schedule next main_thread call.");
		uv_timer_start(&main_thread_timer, main_thread, 10, 0);
	} else {
		// uv_close((uv_handle_t *)&main_thread_timer, NULL);
		DSK_DEBUG("+++ main_thread_timer closed");

		/* await for the background thread to finish */
		DSK_DEBUG("uv_thread_join wait");
		uv_thread_join(&thread);
		DSK_DEBUG("uv_thread_join done");

		/* stop keep alive timer */
		uv_close((uv_handle_t *)keep_alive, NULL);
		DSK_DEBUG("uv_close keep_alive done");

		assert(event_loop_closed_cb_ref != NULL);

		DSK_DEBUG("🧐 LOOP STOPPED");
		napi_ref ref = event_loop_closed_cb_ref;
		event_loop_closed_cb_ref = NULL;
		ln_set_loop_status(stopped);
		dsk_call_cb(resolution_env, ref);

		DSK_DEBUG("resolved stop promise");
	}
}

DSK_EXTEND_MODULE(libdesktop);

/* This function start the event loop and exit immediately */
DSK_DEFINE_FUNCTION(libdesktop, startEventLoop) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);
	napi_ref cb_ref;
	DSK_NAPI_CALL(napi_create_reference(env, argv[0], 1, &cb_ref));

	if (event_loop_closed_cb_ref != NULL) {
		napi_throw_error(env, NULL, "Cannot start. A stop loop operation is pending.");
		return NULL;
	}

	if (event_loop_started_cb_ref != NULL) {
		napi_throw_error(env, NULL, "Cannot start. A start loop operation is pending.");
		return NULL;
	}

	DSK_DEBUG("🧐 LOOP STARTING");
	uv_barrier_init(&all_threads_are_waiting, 2);
	uv_barrier_init(&all_threads_are_awaked, 2);
	DSK_DEBUG("barrier passed");

	event_loop_started_cb_ref = cb_ref;

	resolution_env = env;

	ln_set_loop_status(starting);
	ln_set_main_thread_waiting(false);
	ln_set_background_thread_waiting(false);
	ln_set_main_thread_quitted(false);

	DSK_DEBUG("libui loop initialized");

	/* start the background thread that check for node evnts pending */
	uv_thread_create(&thread, background_thread, NULL);
	DSK_DEBUG("background thread started...");

	// Add dummy handle for libuv, otherwise libuv would quit when there is
	// nothing to do.
	keep_alive = malloc(sizeof(uv_async_t));
	uv_async_init(uv_default_loop(), keep_alive, NULL);

	/* start main_thread timer */
	uv_timer_init(uv_default_loop(), &main_thread_timer);
	uv_timer_start(&main_thread_timer, main_thread, 1, 0);

	DSK_DEBUG("startLoop async started");

	return NULL;
}

DSK_DEFINE_FUNCTION(libdesktop, stopEventLoop) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);
	napi_ref cb_ref;
	DSK_NAPI_CALL(napi_create_reference(env, argv[0], 1, &cb_ref));

	DSK_DEBUG("🧐 LOOP STOPPING");

	if (event_loop_closed_cb_ref != NULL) {
		napi_throw_error(env, NULL, "Cannot start. A stop loop operation is pending.");
		return NULL;
	}

	if (event_loop_started_cb_ref != NULL) {
		napi_throw_error(env, NULL, "Cannot start. A start loop operation is pending.");
		return NULL;
	}

	ln_set_loop_status(stopping);

	event_loop_closed_cb_ref = cb_ref;
	resolution_env = env;

	// destroy_all_children(env, visible_windows);
	// clear_children(env, visible_windows);
	// visible_windows = create_children_list();

	DSK_DEBUG("visible windows cleaned up");

	dsk_quit();

	DSK_DEBUG("dsk_quit called");

	return NULL;
}

// this function signal background thread
// to stop awaiting node events, allowing it
// to update the list of handles it's
// awaiting on.
DSK_DEFINE_FUNCTION(libdesktop, wakeupBackgroundThread) {
	if (uv_is_active((const uv_handle_t *)keep_alive)) {
		uv_async_send(keep_alive);
	}
	return NULL;
}
