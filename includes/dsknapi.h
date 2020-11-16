#ifndef DSK_NAPI_H
#define DSK_NAPI_H 1

#include <string.h>
#include <uv.h>

/**
 *
 * @file dsknapi.h
 *
 * @brief NAPI functions and macros helpers
 *
 * @descr
 *
 * This header file includes functions prototypes and macros that
 * helps working with Node.js NAPI framework.
 *
 */

#include <node_api.h>
#include <stdio.h>

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

///////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @group DSK_FN_CALL
 * @brief Function and macro to call NAPI and callback functions.
 *
 */

///////////////////////////////////////////////////////////////////////////////////////

//
//
//
//
//
//
//
//

/**
 * @name DSK_NAPI_CALL
 * @group DSK_FN_CALL
 * @brief wraps its FN argument with code to check that a NAPI call succeded.
 *
 * @descr
 *
 * DSK_NAPI_CALL macro wraps its FN argument
 * with code to check that a napi call succeded.
 * FN argument must be an expression that evaluates
 * to an object of `napi_status` type.
 *
 * The entire macro is wrapped in a do {} while, so it
 * doesn't pollute the calling stack with variables.
 *
 * The macro does these assumptions on the calling scope:
 *
 *  * an `dsk_env` variable is defined of type `napi_env` that contains the current
 *    NAPI environment to use.
 *  * a `dsk_error_msg` variable is declared of type char*.
 *    This variable will contain the error message in case an error
 *    occurs during the call.
 *  * a `dsk_error` label is defined. Execution will jump to that label
 *    after appropriately setting the dsk_error_msg variable.
 *
 * If you are using the DSK_NAPI_CALL macro in a function that calls
 * the DSK_JS_FUNC_INIT* macro, that macro already appropriately defines
 * in the scope all variables needed by DSK_NAPI_CALL. Otherwise, it is
 * caller responsibility to appropriately prepare the scope before
 * the call to DSK_NAPI_CALL.
 *
 * @arg FN - an expression that evaluates to `napi_status`, usually
 * 		a NAPI function call.
 *
 */
#define DSK_NAPI_CALL(FN)                                                                          \
	do {                                                                                           \
		napi_status DSK__status = (FN);                                                            \
		if (DSK__status != napi_ok) {                                                              \
			const napi_extended_error_info *err;                                                   \
			napi_get_last_error_info(env, &err);                                                   \
                                                                                                   \
			bool DSK__exception_pending;                                                           \
			if (DSK__status == napi_pending_exception) {                                           \
				printf("DSK__status == napi_pending_exception\n");                                 \
				DSK__exception_pending = true;                                                     \
			} else {                                                                               \
				printf("DSK__status != napi_pending_exception\n");                                 \
				napi_is_exception_pending(env, &DSK__exception_pending);                           \
			}                                                                                      \
			if (DSK__exception_pending) {                                                          \
				napi_value last_exception, last_exception_msg;                                     \
				napi_get_and_clear_last_exception(env, &last_exception);                           \
				napi_coerce_to_string(env, last_exception, &last_exception_msg);                   \
				dsk_get_utf8_cstr(env, last_exception_msg, &dsk_error_msg);                        \
			} else {                                                                               \
				dsk_error_msg = (char *)err->error_message;                                        \
			}                                                                                      \
                                                                                                   \
			goto dsk_error;                                                                        \
		}                                                                                          \
	} while (0)

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

///////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @group DSK_ERR
 * @brief Function and macro to declare error handlers.
 *
 */

///////////////////////////////////////////////////////////////////////////////////////

//
//
//
//
//
//
//
//

/**
 * 	@name DSK_ONERROR_THROW_RET
 *  @brief defines an error handler that throw a JavaScript error.
 *  @group DSK_ERR
 *
 *  @descr
 * 	This macro defines following variables in current scope:
 *
 *  * a `dsk_error_msg` variable of type char* that is compiled by DSK_NAPI_CALL in
 *    case of errors.
 *  * a `dsk_error` label to which execution will jump in case of error. It contains
 *    code to throw a JS error with the message contained in `dsk_error_msg` variable,
 *    and return `WHAT` argument afterward;
 *  @arg WHAT - an expression to return that signal the caller that an error occurred.
 *
 */
#define DSK_ONERROR_THROW_RET(WHAT)                                                                \
	/* error handler */                                                                            \
	char *dsk_error_msg = NULL;                                                                    \
	goto dsk_continue;                                                                             \
	goto dsk_error;                                                                                \
	dsk_error:                                                                                     \
	napi_throw_error(env, NULL, dsk_error_msg);                                                    \
	return WHAT;                                                                                   \
	dsk_continue:

/**
 * 	@name DSK_ONERROR_FATAL_RET
 *  @brief defines an error handler that bort the process
 *  @group DSK_ERR
 *  @descr
 *
 * 	This macro defines following variables in current scope:
 *
 *  * a `dsk_error_msg` variable of type char* that is compiled by DSK_NAPI_CALL in
 *    case of errors.
 *  * a `dsk_error` label to which execution will jump in case of error. It contains
 *    code to call `napi_fatal_error` with the message contained in `dsk_error_msg` variable,
 *    and return `WHAT` argument afterward. The call to `napi_fatal_error` causes the node process
 * 	  to immediately exit.
 *  @arg WHAT - an expression to return that signal the caller that an error occurred.
 *
 */
#define DSK_ONERROR_FATAL_RET(WHAT)                                                                \
	/* error handler */                                                                            \
	char *dsk_error_msg = NULL;                                                                    \
	goto dsk_continue;                                                                             \
	goto dsk_error;                                                                                \
	dsk_error:                                                                                     \
	napi_fatal_error(NULL, 0, dsk_error_msg, NAPI_AUTO_LENGTH);                                    \
	return WHAT;                                                                                   \
	dsk_continue:

/**
 * 	@name DSK_ONERROR_UNCAUGHT_RET
 *  @group DSK_ERR
 *  @brief defines an error handler that throw an uncaught JavaScript error.
 *  @descr
 * 	This macro defines following variables in current scope:
 *
 *  * a `dsk_error_msg` variable of type char* that is compiled by DSK_NAPI_CALL in
 *    case of errors.
 *  * a `dsk_error` label to which execution will jump in case of error. It contains
 *    code to throw an uncaught Javascript exception with the message contained in `dsk_error_msg`
 *    variable, and return `WHAT` argument afterward.
 *
 */
#define DSK_ONERROR_UNCAUGHT_RET(WHAT)                                                             \
	/* error handler */                                                                            \
	char *dsk_error_msg = NULL;                                                                    \
	goto dsk_continue;                                                                             \
	goto dsk_error;                                                                                \
	dsk_error : {                                                                                  \
		napi_value err, errmsg;                                                                    \
		napi_create_string_utf8(env, dsk_error_msg, NAPI_AUTO_LENGTH, &errmsg);                    \
		napi_create_error(env, NULL, errmsg, &err);                                                \
		napi_fatal_exception(env, err);                                                            \
		return WHAT;                                                                               \
	}                                                                                              \
	dsk_continue:

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

///////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @group DSK_FN
 * @brief Function and macro to declare callback functions and theis arguments.
 *
 */

///////////////////////////////////////////////////////////////////////////////////////

//
//
//
//
//
//
//
//

/**
 * @name DSK_JS_FUNC
 * @group DSK_FN
 * @brief declares a js callback function prototype
 *
 * @descr
 * DSK_JS_FUNC declares a js callback function prototype given the name.
 * It can be used to declare a function in a header file:
 *
 * ```c
 *
 *  DSK_JS_FUNC(external_function);
 *
 * ```
 *
 * or otherwise to define a function in a source file:
 *
 * ```c
 *
 * DSK_JS_FUNC(external_function) {
 *
 *  // do something interesting here...
 *
 *  // and return something useful.
 *  return NULL;
 * }
 *
 * ```
 *
 */
#define DSK_JS_FUNC(FN_NAME) napi_value FN_NAME(napi_env env, napi_callback_info info)

#define DSK_MODINIT_FUNC(FN_NAME) napi_value FN_NAME(napi_env env, napi_value exports)

/**
 * @name DSK_JS_FUNC_INIT
 * @group DSK_FN
 * @brief initialize a scope with the following variables:
 *
 * @descr
 * The macro does these assumptions on the calling scope:
 *  * an `env` variable is defined of type `napi_env` that contains the current
 *    NAPI environment to use.
 *  * an `info` variable is declared of type napi_callback_info.
 *    This variable will contain the error message in case an error
 *    occurs during the call.
 * This two requirement are already satisfied by the protype required by callback
 * functions.
 *
 * The macro uses `env` and `info` to extract arguments of the call, and defines
 * in current scope the following additional variables:
 *
 *  * a `dsk_error_msg` variable of type char* that is compiled by NAPI_CALL in case of errors.
 *  * a `dsk_error` label to which execution will jump in case of error. It contains code to
 *    throw a JS error with the message contained in `dsk_error_msg` variable, and return NULL
 * afterward;
 *  * an `argc` variable of type size_t that contains the actual number of argument
 *    passed to the function by the caller.
 *  * an `argv` array of type napi_value[] that contains the actual number of argument
 *    passed to the function by the caller. If the function is given more than 10 arguments, the
 * remaining one are ignored. If it's given less than 10 arguments, than argv elements from `argc`
 * to 10 are filled witth `napi_undefined`. If you have to use accepts more than 10 arguments, use
 * `DSK_JS_FUNC_INIT_WITH_ARGS` macro to specify a greater number.
 *  * a `this` variable, of type napi_value, that contains the receiver of the call (the JS `this`
 * value);
 *  * an `argc` variable of type size_t containing actual number of arguments received in the call.
 *
 */
#define DSK_JS_FUNC_INIT() DSK_JS_FUNC_INIT_WITH_ARGS(10)

/**
 * @name DSK_JS_FUNC_INIT_WITH_ARGS
 * @group DSK_FN
 * @brief initialize a function scope with a set of standard variables
 *
 * @descr
 *
 * You can use this function when you have to accepts more than 10 arguments in your
 * callback. Use `DSK_JS_FUNC_INIT` when <= 10 arguments are required.
 *
 * @see `DSK_JS_FUNC_INIT`
 *
 */
#define DSK_JS_FUNC_INIT_WITH_ARGS(ARGS_COUNT)                                                     \
	DSK_ONERROR_THROW_RET(NULL);                                                                   \
	napi_value argv[(ARGS_COUNT)];                                                                 \
	napi_value this;                                                                               \
	size_t argc = (ARGS_COUNT);                                                                    \
	DSK_NAPI_CALL(napi_get_cb_info(env, info, &argc, argv, &this, NULL));

/**
 * @name DSK_AT_LEAST_NARGS
 * @group DSK_FN
 * @brief Throw a "EINVAL" errors if the callback is not called with at least N arguments.
 *
 * @descr
 *
 * The calling scope must contains `env`, `argc` and `dsk_error_msg` variables, and a `dsk_error`
 * label.
 *
 */
#define DSK_AT_LEAST_NARGS(N)                                                                      \
	if (argc < (N)) {                                                                              \
		dsk_error_msg = "Too few arguments. At least " #N " required.";                            \
		goto dsk_error;                                                                            \
	}

/**
 * @name DSK_EXACTLY_NARGS
 * @group DSK_FN
 * @brief Throw a "EINVAL" errors if the callback is not called with exactly N arguments.
 *
 * @descr
 *
 * The calling scope must contains `env`, `argc` and `dsk_error_msg` variables, and a `dsk_error`
 * label.
 *
 */
#define DSK_EXACTLY_NARGS(N)                                                                       \
	if (argc != (N)) {                                                                             \
		dsk_error_msg = "Wrong number of arguments. " #N " required.";                             \
		goto dsk_error;                                                                            \
	}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

///////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @group DSK_XP
 * @brief Function and macro to exports function, objects, or classes.
 *
 */

///////////////////////////////////////////////////////////////////////////////////////

//
//
//
//
//
//
//
//

/**
 * @name dsk_def_type
 * @brief An enum used to specify the type of a dsk_export_def.
 * @group DSK_XP
 * @descr
 *
 * specifies if an export def is a class, object or method.
 *
 */
typedef enum dsk_def_type {
	dsk_def_type_object,
	dsk_def_type_class,
	dsk_def_type_function,
	dsk_def_type_exec,
} dsk_def_type;

//

/**
 * @name dsk_export_def
 * @brief contains all infos needed to create a class or function
 * @group DSK_XP
 * @descr
 *
 * dsk_export_def contains all napi_property_descriptor that defines
 * an export of the module.
 *
 * type member contains the kind of export to define.
 *
 * * when equal to dsk_def_type_function, the properties member
 *   is array of exactly 1 member, that contain the napi_property_descriptor for the
 *   function
 *
 * * when equal to dsk_def_type_object, the properties member contains the definition
 * 	 of all properties of the object.
 *
 * * when equal to dsk_def_type_class, the first property contains:
 *
 * 		1) the constructor in method field.
 * 		2) an optional addres to a napi_ref where to save a reference to the class, in data field.
 *   	3) the name of the class in utf8name field.
 *
 *   All other properties specify the properties to define in the class prototype.
 *
 */
typedef struct dsk_export_def {
	dsk_def_type type;
	// const char *name;
	// napi_callback constructor;
	size_t properties_count;
	napi_property_descriptor *properties;
	bool malloced;
} dsk_export_def;

/**
 * @name dsk_modexports_def
 * @group DSK_XP
 * @brief contains all infos needed to create all exported members of a module
 *
 */
typedef struct dsk_modexports_def {
	size_t members_count;
	/* this is a dsk_export_def** because it's an array of pointers
	to static dsk_export_def defined in module c file.	*/
	dsk_export_def **members;
} dsk_modexports_def;

/**
 * @name dsk_modexports_def_register_member
 * @group DSK_XP
 * @brief register a new member in the exports of a module.
 * @descr
 * register a new member in the exports of a module.
 *
 * @param exports:dsk_modexports_def* -
 * @param member:dsk_export_def* -
 */
void dsk_modexports_def_register_member(dsk_modexports_def *exports, dsk_export_def *member);

/**
 * @name dsk_export_def_register_member
 * @group DSK_XP
 * @brief register a new member of a class or object.
 * @descr
 * register a new member of a class or object.
 * @param group:dsk_export_def* -
 * @param member:napi_property_descriptor -
 */
void dsk_export_def_register_member(dsk_export_def *group, napi_property_descriptor member);

// free the exports data structures after module registration.
/**
 * @name dsk_modexports_def_free
 * @group DSK_XP
 * @brief free the exports data structures after module registration.
 * @descr
 * free the exports data structures after module registration.
 * @param exports:dsk_modexports_def* -
 */
void dsk_modexports_def_free(dsk_modexports_def *exports);

/**
 * @name dsk_init_module_def
 * @group DSK_XP
 * @brief default initialization function for modules.
 * @descr
 * default initialization function for modules.
 *
 * @param env:napi_env -
 * @param exports:napi_value -
 * @param exports_def:dsk_modexports_def* -
 * @return napi_value
 */
napi_value dsk_init_module_def(napi_env env, napi_value exports, dsk_modexports_def *exports_def);

#define _DSK_MOD_EXPORTS(MODNAME) MODNAME##_exports
#define _DSK_CLASS_DEFS(MODNAME, CLASSNAME) MODNAME##_##CLASSNAME##_def
#define _DSK_FUNC_DEFS(MODNAME, FUNCNAME) MODNAME##_##FUNCNAME##_def
#define _DSK_METHOD_DEFS(MODNAME, CLASSNAME, FUNCNAME) MODNAME##_##CLASSNAME##_##FUNCNAME##_def
#define _DSK_PROPERTY_DEFS(MODNAME, CLASSNAME, FUNCNAME)                                           \
	_DSK_METHOD_DEFS(MODNAME, CLASSNAME, FUNCNAME)
#define _DSK_USE_MODULE(MODNAME) dsk_modexports_def _DSK_MOD_EXPORTS(MODNAME)

/**
 * @name DSK_USE_MODULE_INITIALIZER
 * @group DSK_XP
 * @brief declare the prototype of an external module initializer function
 * @descr
 * this macro could be usefule to register multiple module in
 * the same NAPI exports
 */
#define DSK_USE_MODULE_INITIALIZER(MODNAME)                                                        \
	napi_value dsk_init_##MODNAME /**/ (napi_env env, napi_value exports)

/**
 * @name DSK_MODULE_INITIALIZER
 * @group DSK_XP
 * @descr declare a custom module initializer to use instead of `dsk_init_module_def`
 * @brief  declare a custom module initializer to use instead of `dsk_init_module_def`
 *
 */
#define DSK_MODULE_INITIALIZER(MODNAME) DSK_USE_MODULE_INITIALIZER(MODNAME)

/**
 * @name DSK_DEFINE_MODULE
 * @brief define a module initializer
 * @group DSK_XP
 * @descr
 * define a module initializer function that setup
 * the whole module exports using the static structures
 * filled during C files initialization stage.
 * Should be used in a single C file per module
 */
#define DSK_DEFINE_MODULE(MODNAME)                                                                 \
	_DSK_USE_MODULE(MODNAME);                                                                      \
                                                                                                   \
	DSK_MODULE_INITIALIZER(MODNAME) {                                                              \
		return dsk_init_module_def(env, exports, &_DSK_MOD_EXPORTS(MODNAME));                      \
	}

/**
 * @name DSK_EXTEND_MODULE
 * @group DSK_XP
 * @brief extend the definition of a module
 * @descr
 * used in a C file to extend the definition of a module defined in anoth file.
 * the extern module can be extended using DSK_DEFINE_FUNCTION or DSK_DEFINE_CLASS macro,
 * or otherwise by directly using dsk_modexports_def_register_member function.
 */
#define DSK_EXTEND_MODULE(MODNAME) extern _DSK_USE_MODULE(MODNAME)

/**
 * @name DSK_DEFINE_CLASS
 * @group DSK_XP
 * @brief define a new class constructor
 * @descr
 * define a new class constructor for a class named CLASSNAME.
 * automatically register the class in the MODNAME module exports.
 * the extern class or object definition can be extended using any of the DSK_DEFINE_*,
 * or otherwise by directly using dsk_export_def_register_member function.
 *
 */
#define DSK_DEFINE_CLASS(MODNAME, CLASSNAME)                                                       \
	/* class constructor C prototype (predeclared because it's used in the initialezer below) */   \
	DSK_JS_FUNC(MODNAME##_##CLASSNAME);                                                            \
	/* reference of constructor function napi_value */                                             \
	_DSK_USE_CLASS(MODNAME, CLASSNAME);                                                            \
	/* static dsk_export_def instance that could be used to enhance the class with */              \
	/* further methods and properties */                                                           \
	dsk_export_def _DSK_CLASS_DEFS(MODNAME, CLASSNAME) = /**/ {                                    \
		.malloced = false,                                                                         \
		.type = dsk_def_type_class,                                                                \
		.properties_count = 1, /* todo, since first property is statically allocated,              \
								  `properties` cannot be free at end .same below for function */   \
		.properties = (napi_property_descriptor[]){{                                               \
			.method = MODNAME##_##CLASSNAME,                                                       \
			.utf8name = #CLASSNAME,                                                                \
			.data = &MODNAME##_##CLASSNAME##_ref,                                                  \
		}}};                                                                                       \
                                                                                                   \
	/* this is an initializer function that register the class and add it to the property */       \
	/* definitions of the module. */                                                               \
	NAPI_C_CTOR(_dsk_register_class##MODNAME##_##CLASSNAME) {                                      \
		dsk_modexports_def *exports = &_DSK_MOD_EXPORTS(MODNAME);                                  \
                                                                                                   \
		dsk_modexports_def_register_member(exports, &_DSK_CLASS_DEFS(MODNAME, CLASSNAME));         \
	}                                                                                              \
	/* begin of implementation of class constructor function. caller is required to */             \
	/* provide the body of the function after the DSK_DEFINE_CLASS call */                         \
	DSK_JS_FUNC(MODNAME##_##CLASSNAME)

/**
 * @name DSK_EXTEND_CLASS
 * @brief extend the definition of a class or object
 * @group DSK_XP
 * @descr
 * used in a C file to extend the definition of a class or object defined in another file.
 * the extern class or object definition can be extended using any of the DSK_DEFINE_*,
 * or otherwise by directly using dsk_export_def_register_member function.
 */
#define DSK_EXTEND_CLASS(MODNAME, CLASSNAME) extern _DSK_USE_CLASS(MODNAME, CLASSNAME)

#define _DSK_USE_CLASS(MODNAME, CLASSNAME)                                                         \
	/* reference to class constructor function napi_value */                                       \
	napi_ref MODNAME##_##CLASSNAME##_ref;                                                          \
	dsk_export_def _DSK_CLASS_DEFS(MODNAME, CLASSNAME);

/**
 * @name DSK_DEFINE_FUNCTION
 * @group DSK_XP
 * @brief define a function callback
 * @descr
 * define an exported function named FUNCNAME.
 * automatically register function in the MODNAME module exports.
 *
 */
#define DSK_DEFINE_FUNCTION(MODNAME, FUNCNAME)                                                     \
	/* function C prototype (predeclared because it's used in the initialezer below) */            \
	DSK_JS_FUNC(MODNAME##_##FUNCNAME);                                                             \
	/* static dsk_export_def instance */                                                           \
	dsk_export_def _DSK_FUNC_DEFS(MODNAME, FUNCNAME) =                                             \
		/**/ {.type = dsk_def_type_function,                                                       \
			  .malloced = false,                                                                   \
			  .properties_count = 1,                                                               \
			  .properties = (napi_property_descriptor[]){                                          \
				  {.method = MODNAME##_##FUNCNAME, .utf8name = #FUNCNAME}}};                       \
                                                                                                   \
	/* this is an initializer function that add the property */                                    \
	/* definition to the module. */                                                                \
	NAPI_C_CTOR(_dsk_register_function_##MODNAME##_##FUNCNAME) {                                   \
		dsk_modexports_def *exports = &_DSK_MOD_EXPORTS(MODNAME);                                  \
		dsk_modexports_def_register_member(exports, &_DSK_FUNC_DEFS(MODNAME, FUNCNAME));           \
	}                                                                                              \
	/* begin of implementation of function. caller is required to provide the body */              \
	/* of the function after the DSK_DEFINE_FUNCTION macro call */                                 \
	DSK_JS_FUNC(MODNAME##_##FUNCNAME)

#define _DSK_DEFINE_METHOD(MODNAME, CLASSNAME, FUNCNAME, ATTRS)                                    \
	/* function C prototype (predeclared because it's used in the initialezer below) */            \
	DSK_JS_FUNC(MODNAME##_##CLASSNAME##_##FUNCNAME);                                               \
	/* static dsk_export_def instance */                                                           \
	napi_property_descriptor _DSK_METHOD_DEFS(MODNAME, CLASSNAME, FUNCNAME) = {                    \
		.method = MODNAME##_##CLASSNAME##_##FUNCNAME, .utf8name = #FUNCNAME, .attributes = ATTRS}; \
                                                                                                   \
	/* this is an initializer function that add the property */                                    \
	/* definition to the module. */                                                                \
	NAPI_C_CTOR(_dsk_register_function_##MODNAME##_##CLASSNAME##_##FUNCNAME) {                     \
		dsk_export_def *exports = &_DSK_CLASS_DEFS(MODNAME, CLASSNAME);                            \
		dsk_export_def_register_member(exports, _DSK_METHOD_DEFS(MODNAME, CLASSNAME, FUNCNAME));   \
	}                                                                                              \
	/* begin of implementation of function. caller is required to provide the body */              \
	/* of the function after the DSK_DEFINE_FUNCTION macro call */                                 \
	DSK_JS_FUNC(MODNAME##_##CLASSNAME##_##FUNCNAME)

/**
 * @name DSK_DEFINE_METHOD
 * @group DSK_XP
 * @brief define a method of a class or object in a module
 * @descr
 * define a method of a class or object in a module
 * automatically register the member in the CLASSNAME class of MODNAME module exports.
 *
 */
#define DSK_DEFINE_METHOD(MODNAME, CLASSNAME, FUNCNAME)                                            \
	_DSK_DEFINE_METHOD(MODNAME, CLASSNAME, FUNCNAME, napi_default)

#define _DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, GETTER, SETTER, DATA, ATTRS)            \
	/* static dsk_export_def instance */                                                           \
	napi_property_descriptor _DSK_PROPERTY_DEFS(MODNAME, CLASSNAME,                                \
												PROPNAME) = {.getter = GETTER,                     \
															 .setter = SETTER,                     \
															 .data = DATA,                         \
															 .utf8name = #PROPNAME,                \
															 .attributes = ATTRS};                 \
                                                                                                   \
	/* this is an initializer function that add the property */                                    \
	/* definition to the module. */                                                                \
	NAPI_C_CTOR(_dsk_register_function_##MODNAME##_##CLASSNAME##_##PROPNAME) {                     \
		dsk_export_def *exports = &_DSK_CLASS_DEFS(MODNAME, CLASSNAME);                            \
		dsk_export_def_register_member(exports, _DSK_PROPERTY_DEFS(MODNAME, CLASSNAME, PROPNAME)); \
	}

/**
 * @name DSK_DEFINE_PROPERTY
 * @group DSK_XP
 * @brief define a property of a class or object in a module
 * @descr
 * define a property of a class or object in a module
 * the property will be readonly if setter is not provider.
 * calling modules will probably provides shortcuts by
 * implement further macros with default values for GETTER, SETTER
 * and other macros to build values for DATA
 */
#define DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, GETTER, SETTER, DATA)                    \
	_DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, GETTER, SETTER, DATA, napi_default)

/**
 * @name DSK_DEFINE_STATIC_METHOD
 * @group DSK_XP
 * @brief define a static method of a class or object in a module
 * @descr
 * define a static method of a class or object in a module
 * automatically register the member in the CLASSNAME class of MODNAME module exports.
 *
 */
#define DSK_DEFINE_STATIC_METHOD(MODNAME, CLASSNAME, FUNCNAME)                                     \
	_DSK_DEFINE_METHOD(MODNAME, CLASSNAME, FUNCNAME, napi_static)

/**
 * @name DSK_DEFINE_STATIC_PROPERTY
 * @group DSK_XP
 * @brief define a static property of a class or object in a module
 * @descr
 * define a static property of a class or object in a module
 * the property will be readonly if setter is not provider.
 * calling modules will probably provides shortcuts by
 * implement further macros with default values for GETTER, SETTER
 * and other macros to build values for DATA
 */
#define DSK_DEFINE_STATIC_PROPERTY(MODNAME, CLASSNAME, PROPNAME, GETTER, SETTER, DATA)             \
	_DSK_DEFINE_PROPERTY(MODNAME, CLASSNAME, PROPNAME, GETTER, SETTER, DATA, napi_static)

#define DSK_FAILURE(MSG)                                                                           \
	{                                                                                              \
		dsk_error_msg = (MSG);                                                                     \
		goto dsk_error;                                                                            \
	}

napi_value dsk_new_instance(napi_env env, napi_ref class, size_t arg_c, napi_value *arg_v);
void *dsk_unwrap(napi_env env, napi_value this);

typedef void dsk_SetterI32(void *instance, const int32_t value, void **datas);
typedef int32_t dsk_GetterI32(void *instance, void **datas);

typedef void dsk_SetterF32(void *instance, const float value, void **datas);
typedef float dsk_GetterF32(void *instance, void **datas);

typedef void dsk_SetterSTR(void *instance, char *value, void **datas);
typedef char *dsk_GetterSTR(void *instance, void **datas);

typedef void dsk_SetterBOOL(void *instance, bool value, void **datas);
typedef bool dsk_GetterBOOL(void *instance, void **datas);

DSK_JS_FUNC(dsk_setPropI32);
DSK_JS_FUNC(dsk_getPropI32);
DSK_JS_FUNC(dsk_setPropBOOL);
DSK_JS_FUNC(dsk_getPropBOOL);
DSK_JS_FUNC(dsk_setPropF32);
DSK_JS_FUNC(dsk_getPropF32);
DSK_JS_FUNC(dsk_setPropSTR);
DSK_JS_FUNC(dsk_getPropSTR);

#define DSK_PROP_STR(NAME, NATIVE_GETTER, NATIVE_SETTER)                                           \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, dsk_getPropSTR, dsk_setPropSTR,                   \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER}))

#define DSK_PROP_I32(NAME, NATIVE_GETTER, NATIVE_SETTER)                                           \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, dsk_getPropI32, dsk_setPropI32,                   \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER}))

#define DSK_PROP_F32(NAME, NATIVE_GETTER, NATIVE_SETTER)                                           \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, dsk_getPropF32, dsk_setPropF32,                   \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER}))

#define DSK_PROP_BOOL(NAME, NATIVE_GETTER, NATIVE_SETTER)                                          \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, dsk_getPropBOOL, dsk_setPropBOOL,                 \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER}))

napi_status dsk_call_cb_async(napi_env env, napi_value recv, napi_value cb, size_t argc,
							  const napi_value *argv);
// other utilities

#define DSK_ARRAY_FOREACH(ARR, BLOCK)                                                              \
	uint32_t dsk_iter_len;                                                                         \
	DSK_NAPI_CALL(napi_get_array_length(env, ARR, &dsk_iter_len));                                 \
	for (uint32_t dsk_iter_i = 0; dsk_iter_i < dsk_iter_len; dsk_iter_i++) {                       \
		napi_value dsk_iter_item;                                                                  \
		DSK_NAPI_CALL(napi_get_element(env, ARR, dsk_iter_i, &dsk_iter_item));                     \
		BLOCK;                                                                                     \
	}

napi_status dsk_array_push(napi_env env, napi_value arr, napi_value item);
napi_status dsk_get_utf8_cstr(napi_env env, napi_value str, char **result);
napi_status dsk_get_utf8_napistr(napi_env env, const char *str, napi_value *result);
napi_status dsk_get_utf16_cstr(napi_env env, napi_value str, char16_t **result);
napi_status dsk_get_utf16_napistr(napi_env env, const char16_t *str, napi_value *result);

// debug log

#define DSK_DEBUG_ON 1

#if DSK_DEBUG_ON

#define DSK_UNINITIALIZED 100042
static char dsk_enabled_debug_module[100];
static size_t dsk_env_var_size = DSK_UNINITIALIZED;

static inline bool debug_enabled_for_module(const char *module) {
	if (dsk_env_var_size == DSK_UNINITIALIZED) {
		uv_os_getenv("DEBUG", dsk_enabled_debug_module, &dsk_env_var_size);
	}

	return (strncmp(module, dsk_enabled_debug_module, dsk_env_var_size) == 0);
}

#define DSK_DEBUG(msg)                                                                             \
	{                                                                                              \
		if (debug_enabled_for_module("libdesktop")) {                                              \
			fprintf(stderr, msg "\n");                                                             \
		}                                                                                          \
	}
#define DSK_DEBUG_F(msg, ...)                                                                      \
	{                                                                                              \
		if (debug_enabled_for_module("libdesktop")) {                                              \
			fprintf(stderr, msg "\n", __VA_ARGS__);                                                \
		}                                                                                          \
	}
#else

#define DSK_DEBUG(msg) ;
#define DSK_DEBUG_F(msg, ...) ;

#endif

#endif