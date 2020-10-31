# NAPI functions and macros helpers

This header file includes functions prototypes and macros that
helps working with Node.js NAPI framework.

## Members


**Function and macro to call NAPI and callback functions.**

* [DSK_NAPI_CALL](#DSK_NAPI_CALL) - wraps its FN argument with code to check that a NAPI call succeded.

**Function and macro to declare error handlers.**

* [DSK_ONERROR_THROW_RET](#DSK_ONERROR_THROW_RET) - defines an error handler that throw a JavaScript error.
* [DSK_ONERROR_FATAL_RET](#DSK_ONERROR_FATAL_RET) - defines an error handler that bort the process
* [DSK_ONERROR_UNCAUGHT_RET](#DSK_ONERROR_UNCAUGHT_RET) - defines an error handler that throw an uncaught JavaScript error.

**Function and macro to declare callback functions and theis arguments.**

* [DSK_JS_FUNC](#DSK_JS_FUNC) - declares a js callback function prototype
* [DSK_JS_FUNC_INIT](#DSK_JS_FUNC_INIT) - initialize a scope with the following variables:
* [DSK_JS_FUNC_INIT_WITH_ARGS](#DSK_JS_FUNC_INIT_WITH_ARGS) - initialize a function scope with a set of standard variables
* [DSK_AT_LEAST_NARGS](#DSK_AT_LEAST_NARGS) - Throw a "EINVAL" errors if the callback is not called with at least N arguments.
* [DSK_EXACTLY_NARGS](#DSK_EXACTLY_NARGS) - Throw a "EINVAL" errors if the callback is not called with exactly N arguments.

**Function and macro to exports function, objects, or classes.**

* [dsk_def_type](#dsk_def_type) - An enum used to specify the type of a dsk_export_def.
* [dsk_export_def](#dsk_export_def) - contains all infos needed to create a class or function
* [dsk_modexports_def](#dsk_modexports_def) - contains all infos needed to create all exported members of a module
* [dsk_modexports_def_register_member](#dsk_modexports_def_register_member) - register a new member in the exports of a module.
* [dsk_export_def_register_member](#dsk_export_def_register_member) - register a new member of a class or object.
* [dsk_modexports_def_free](#dsk_modexports_def_free) - free the exports data structures after module registration.
* [dsk_init_module_def](#dsk_init_module_def) - default initialization function for modules.
* [DSK_USE_MODULE_INITIALIZER](#DSK_USE_MODULE_INITIALIZER) - declare the prototype of an external module initializer function
* [DSK_MODULE_INITIALIZER](#DSK_MODULE_INITIALIZER) - declare a custom module initializer to use instead of `dsk_init_module_def`
* [DSK_DEFINE_MODULE](#DSK_DEFINE_MODULE) - define a module initializer
* [DSK_EXTEND_MODULE](#DSK_EXTEND_MODULE) - extend the definition of a module
* [DSK_DEFINE_CLASS](#DSK_DEFINE_CLASS) - define a new class constructor
* [DSK_EXTEND_CLASS](#DSK_EXTEND_CLASS) - extend the definition of a class or object
* [DSK_DEFINE_FUNCTION](#DSK_DEFINE_FUNCTION) - define a function callback
* [DSK_DEFINE_METHOD](#DSK_DEFINE_METHOD) - define a method of a class or object in a module
* [DSK_DEFINE_PROPERTY](#DSK_DEFINE_PROPERTY) - define a property of a class or object in a module
* [DSK_DEFINE_STATIC_METHOD](#DSK_DEFINE_STATIC_METHOD) - define a static method of a class or object in a module
* [DSK_DEFINE_STATIC_PROPERTY](#DSK_DEFINE_STATIC_PROPERTY) - define a static property of a class or object in a module

### Function and macro to call NAPI and callback functions.
#### DSK_NAPI_CALL

> wraps its FN argument with code to check that a NAPI call succeded.

DSK_NAPI_CALL macro wraps its FN argument
with code to check that a napi call succeded.
FN argument must be an expression that evaluates
to an object of `napi_status` type.

The entire macro is wrapped in a do {} while, so it
doesn't pollute the calling stack with variables.

The macro does these assumptions on the calling scope:

 * an `dsk_env` variable is defined of type `napi_env` that contains the current
   NAPI environment to use.
 * a `dsk_error_msg` variable is declared of type char*.
   This variable will contain the error message in case an error
   occurs during the call.
 * a `dsk_error` label is defined. Execution will jump to that label
   after appropriately setting the dsk_error_msg variable.

If you are using the DSK_NAPI_CALL macro in a function that calls
the DSK_JS_FUNC_INIT* macro, that macro already appropriately defines
in the scope all variables needed by DSK_NAPI_CALL. Otherwise, it is
caller responsibility to appropriately prepare the scope before
the call to DSK_NAPI_CALL.


---

### Function and macro to declare error handlers.
#### DSK_ONERROR_THROW_RET

> defines an error handler that throw a JavaScript error.

This macro defines following variables in current scope:

 * a `dsk_error_msg` variable of type char* that is compiled by DSK_NAPI_CALL in
   case of errors.
 * a `dsk_error` label to which execution will jump in case of error. It contains
   code to throw a JS error with the message contained in `dsk_error_msg` variable,
   and return `WHAT` argument afterward;


#### DSK_ONERROR_FATAL_RET

> defines an error handler that bort the process

This macro defines following variables in current scope:

 * a `dsk_error_msg` variable of type char* that is compiled by DSK_NAPI_CALL in
   case of errors.
 * a `dsk_error` label to which execution will jump in case of error. It contains
   code to call `napi_fatal_error` with the message contained in `dsk_error_msg` variable,
   and return `WHAT` argument afterward. The call to `napi_fatal_error` causes the node process
	  to immediately exit.


#### DSK_ONERROR_UNCAUGHT_RET

> defines an error handler that throw an uncaught JavaScript error.

This macro defines following variables in current scope:

 * a `dsk_error_msg` variable of type char* that is compiled by DSK_NAPI_CALL in
   case of errors.
 * a `dsk_error` label to which execution will jump in case of error. It contains
   code to throw an uncaught Javascript exception with the message contained in `dsk_error_msg`
   variable, and return `WHAT` argument afterward.


---

### Function and macro to declare callback functions and theis arguments.
#### DSK_JS_FUNC

> declares a js callback function prototype

DSK_JS_FUNC declares a js callback function prototype given the name.
It can be used to declare a function in a header file:

```c

 DSK_JS_FUNC(external_function);

```

or otherwise to define a function in a source file:

```c

DSK_JS_FUNC(external_function) {

 // do something interesting here...

 // and return something useful.
 return NULL;
}

```


#### DSK_JS_FUNC_INIT

> initialize a scope with the following variables:

The macro does these assumptions on the calling scope:
 * an `env` variable is defined of type `napi_env` that contains the current
   NAPI environment to use.
 * an `info` variable is declared of type napi_callback_info.
   This variable will contain the error message in case an error
   occurs during the call.
This two requirement are already satisfied by the protype required by callback
functions.

The macro uses `env` and `info` to extract arguments of the call, and defines
in current scope the following additional variables:

 * a `dsk_error_msg` variable of type char* that is compiled by NAPI_CALL in case of errors.
 * a `dsk_error` label to which execution will jump in case of error. It contains code to
   throw a JS error with the message contained in `dsk_error_msg` variable, and return NULL
afterward;
 * an `argc` variable of type size_t that contains the actual number of argument
   passed to the function by the caller.
 * an `argv` array of type napi_value[] that contains the actual number of argument
   passed to the function by the caller. If the function is given more than 10 arguments, the
remaining one are ignored. If it's given less than 10 arguments, than argv elements from `argc`
to 10 are filled witth `napi_undefined`. If you have to use accepts more than 10 arguments, use
`DSK_JS_FUNC_INIT_WITH_ARGS` macro to specify a greater number.
 * a `this` variable, of type napi_value, that contains the receiver of the call (the JS `this`
value);
 * an `argc` variable of type size_t containing actual number of arguments received in the call.


#### DSK_JS_FUNC_INIT_WITH_ARGS

> initialize a function scope with a set of standard variables

You can use this function when you have to accepts more than 10 arguments in your
callback. Use `DSK_JS_FUNC_INIT` when <= 10 arguments are required.


#### DSK_AT_LEAST_NARGS

> Throw a "EINVAL" errors if the callback is not called with at least N arguments.

The calling scope must contains `env`, `argc` and `dsk_error_msg` variables, and a `dsk_error`
label.


#### DSK_EXACTLY_NARGS

> Throw a "EINVAL" errors if the callback is not called with exactly N arguments.

The calling scope must contains `env`, `argc` and `dsk_error_msg` variables, and a `dsk_error`
label.


---

### Function and macro to exports function, objects, or classes.
#### dsk_def_type

> An enum used to specify the type of a dsk_export_def.

specifies if an export def is a class, object or method.


#### dsk_export_def

> contains all infos needed to create a class or function

dsk_export_def contains all napi_property_descriptor that defines
an export of the module.

type member contains the kind of export to define.

* when equal to dsk_def_type_function, the properties member
  is array of exactly 1 member, that contain the napi_property_descriptor for the
  function

* when equal to dsk_def_type_object, the properties member contains the definition
	 of all properties of the object.

* when equal to dsk_def_type_class, the first property contains:

		1) the constructor in method field.
		2) an optional addres to a napi_ref where to save a reference to the class, in data field.
  	3) the name of the class in utf8name field.

  All other properties specify the properties to define in the class prototype.


#### dsk_modexports_def

> contains all infos needed to create all exported members of a module




#### dsk_modexports_def_register_member

> register a new member in the exports of a module.

register a new member in the exports of a module.


#### dsk_export_def_register_member

> register a new member of a class or object.

register a new member of a class or object.


#### dsk_modexports_def_free

> free the exports data structures after module registration.

free the exports data structures after module registration.


#### dsk_init_module_def

> default initialization function for modules.

default initialization function for modules.


#### DSK_USE_MODULE_INITIALIZER

> declare the prototype of an external module initializer function

this macro could be usefule to register multiple module in
the same NAPI exports


#### DSK_MODULE_INITIALIZER

> declare a custom module initializer to use instead of `dsk_init_module_def`

declare a custom module initializer to use instead of `dsk_init_module_def`


#### DSK_DEFINE_MODULE

> define a module initializer

define a module initializer function that setup
the whole module exports using the static structures
filled during C files initialization stage.
Should be used in a single C file per module


#### DSK_EXTEND_MODULE

> extend the definition of a module

used in a C file to extend the definition of a module defined in anoth file.
the extern module can be extended using DSK_DEFINE_FUNCTION or DSK_DEFINE_CLASS macro,
or otherwise by directly using dsk_modexports_def_register_member function.


#### DSK_DEFINE_CLASS

> define a new class constructor

define a new class constructor for a class named CLASSNAME.
automatically register the class in the MODNAME module exports.
the extern class or object definition can be extended using any of the DSK_DEFINE_*,
or otherwise by directly using dsk_export_def_register_member function.


#### DSK_EXTEND_CLASS

> extend the definition of a class or object

used in a C file to extend the definition of a class or object defined in another file.
the extern class or object definition can be extended using any of the DSK_DEFINE_*,
or otherwise by directly using dsk_export_def_register_member function.


#### DSK_DEFINE_FUNCTION

> define a function callback

define an exported function named FUNCNAME.
automatically register function in the MODNAME module exports.


#### DSK_DEFINE_METHOD

> define a method of a class or object in a module

define a method of a class or object in a module
automatically register the member in the CLASSNAME class of MODNAME module exports.


#### DSK_DEFINE_PROPERTY

> define a property of a class or object in a module

define a property of a class or object in a module
the property will be readonly if setter is not provider.
calling modules will probably provides shortcuts by
implement further macros with default values for GETTER, SETTER
and other macros to build values for DATA


#### DSK_DEFINE_STATIC_METHOD

> define a static method of a class or object in a module

define a static method of a class or object in a module
automatically register the member in the CLASSNAME class of MODNAME module exports.


#### DSK_DEFINE_STATIC_PROPERTY

> define a static property of a class or object in a module

define a static property of a class or object in a module
the property will be readonly if setter is not provider.
calling modules will probably provides shortcuts by
implement further macros with default values for GETTER, SETTER
and other macros to build values for DATA


---


