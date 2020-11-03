#include "control.h"
#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, Event) {

	DSK_JS_FUNC_INIT();
	DSK_AT_LEAST_NARGS(2);

	napi_value eventname = argv[0];
	napi_value sender = argv[1];

	napi_value listeners;
	DSK_NAPI_CALL(napi_create_array(env, &listeners));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "listeners", listeners));

	napi_value false_val;
	DSK_NAPI_CALL(napi_get_boolean(env, false, &false_val));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "initialized", false_val));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "sender", sender));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "eventname", eventname));

	return this;
}

DSK_DEFINE_METHOD(libdesktop, Event, listen) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);

	napi_value listener = argv[0];

	napi_value initialized_js;
	bool initialized;
	DSK_NAPI_CALL(napi_get_named_property(env, this, "initialized", &initialized_js));
	DSK_NAPI_CALL(napi_get_value_bool(env, initialized_js, &initialized));

	if (!initialized) {
		napi_value true_val;
		DSK_NAPI_CALL(napi_get_boolean(env, true, &true_val));
		DSK_NAPI_CALL(napi_set_named_property(env, this, "initialized", true_val));

		napi_value sender;
		DSK_NAPI_CALL(napi_get_named_property(env, this, "sender", &sender));
		napi_valuetype sender_type;
		DSK_NAPI_CALL(napi_typeof(env, sender, &sender_type));
		if (sender_type != napi_null) {
			UIHandle widget;
			DSK_NAPI_CALL(napi_unwrap(env, sender, (void **)&widget));

			napi_value eventname;
			DSK_NAPI_CALL(napi_get_named_property(env, this, "eventname", &eventname));
			char eventname_s[100];
			DSK_NAPI_CALL(napi_get_value_string_utf8(env, eventname, eventname_s, 100, NULL));

			struct dsk_event_args *args = malloc(sizeof(struct dsk_event_args));

			DSK_NAPI_CALL(napi_create_reference(env, this, 1, &args->event));
			DSK_NAPI_CALL(napi_create_reference(env, sender, 1, &args->sender));

			args->env = env;
			dsk_connect_event(widget, eventname_s, args);
		}
	}

	napi_value listeners;
	napi_value push;

	DSK_NAPI_CALL(napi_get_named_property(env, this, "listeners", &listeners));
	DSK_NAPI_CALL(napi_get_named_property(env, listeners, "push", &push));

	napi_value result;
	DSK_NAPI_CALL(napi_call_function(env, listeners, push, 1, (napi_value[]){listener}, &result));

	return NULL;
}

DSK_DEFINE_METHOD(libdesktop, Event, invoke) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);
	napi_value listeners;

	DSK_NAPI_CALL(napi_get_named_property(env, this, "listeners", &listeners));

	uint32_t len;
	DSK_NAPI_CALL(napi_get_array_length(env, listeners, &len));

	napi_value null;
	DSK_NAPI_CALL(napi_get_null(env, &null));
	napi_value result;

	for (uint32_t i = 0; i < len; i++) {
		napi_value idx;
		napi_value listener;
		DSK_NAPI_CALL(napi_create_uint32(env, i, &idx));
		DSK_NAPI_CALL(napi_get_property(env, listeners, idx, &listener));
		DSK_NAPI_CALL(napi_call_function(env, null, listener, 0, NULL, &result));
	}
	return NULL;
}

napi_value dsk_event_new_for_widget(napi_env env, const char *eventname, napi_value sender) {
	char *dsk_error_msg;

	napi_value constructor;
	napi_value event;
	napi_value eventNameJs;

	DSK_NAPI_CALL(napi_create_string_utf8(env, eventname, NAPI_AUTO_LENGTH, &eventNameJs));
	DSK_NAPI_CALL(napi_get_reference_value(env, libdesktop_Event_ref, &constructor));

	napi_value args[2] = {eventNameJs, sender};
	DSK_NAPI_CALL(napi_new_instance(env, constructor, 2, args, &event));

	return event;
dsk_error:
	napi_throw_error(env, NULL, dsk_error_msg);
	return NULL;
}

void dsk_on_event(UIHandle *uihandle, void *data) {
	struct dsk_event_args *args = data;
	napi_env env = args->env;
	char *dsk_error_msg;

	napi_handle_scope handle_scope;
	DSK_NAPI_CALL(napi_open_handle_scope(env, &handle_scope));

	napi_value sender;
	napi_value event;

	DSK_NAPI_CALL(napi_get_reference_value(env, args->sender, &sender));
	DSK_NAPI_CALL(napi_get_reference_value(env, args->event, &event));

	napi_value resource_object;
	DSK_NAPI_CALL(napi_create_object(env, &resource_object));

	napi_value async_resource_name;
	DSK_NAPI_CALL(
		napi_create_string_utf8(env, "libdesktop", NAPI_AUTO_LENGTH, &async_resource_name));

	napi_async_context async_context;
	DSK_NAPI_CALL(napi_async_init(env, NULL, async_resource_name, &async_context));

	napi_value invoke;
	DSK_NAPI_CALL(napi_get_named_property(env, event, "invoke", &invoke));

	napi_value result;
	napi_status res_status =
		napi_make_callback(env, async_context, event, invoke, 1, (napi_value[]){sender}, &result);
	if (res_status == napi_pending_exception) {
		napi_value last_exception;
		napi_get_and_clear_last_exception(env, &last_exception);
		napi_fatal_exception(env, last_exception);
		return;
	}
	DSK_NAPI_CALL(res_status);

	DSK_NAPI_CALL(napi_close_handle_scope(env, handle_scope));

	return;
dsk_error:;
	napi_value uncErr;
	napi_value err_msg;
	napi_create_string_utf8(env, dsk_error_msg, NAPI_AUTO_LENGTH, &err_msg);
	napi_create_error(env, NULL, err_msg, &uncErr);
	napi_fatal_exception(env, uncErr);
	return;
}