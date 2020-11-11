#ifndef DSK_HOST__
#define DSK_HOST__ 1

#include <node_api.h>
#include <uv.h>

int dsk_ui_events_pending();
int dsk_wakeup_ui_loop();

const char *dsk_init();

int dsk_process_ui_event(int wait);
void dsk_quit(void);

int dsk_wait_node_events(uv_loop_t *loop, int timeout);

enum ln_loop_status { stopped, stopping, started, starting };

enum ln_loop_status ln_get_loop_status();
napi_status ln_set_loop_status(enum ln_loop_status new_status);

void ln_set_main_thread_waiting(bool value);
void ln_set_background_thread_waiting(bool value);
bool ln_get_main_thread_waiting();
bool ln_get_background_thread_waiting();

void ln_set_main_thread_quitted(bool value);
bool ln_get_main_thread_quitted();

napi_status ln_init_loop_status();

#ifdef _WIN32
#define atomic_bool bool
#else
#define atomic_bool bool _Atomic
#endif

#endif
