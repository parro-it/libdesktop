#include "dsknapi.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void dsk_modexports_def_register_member(dsk_modexports_def *exports, dsk_export_def *member) {
	exports->members_count++;
	exports->members = realloc(exports->members, exports->members_count * sizeof(dsk_export_def *));
	exports->members[exports->members_count - 1] = member;
}

void dsk_export_def_register_member(dsk_export_def *group, napi_property_descriptor member) {
	group->properties_count++;
	if (group->malloced) {
		group->properties =
			realloc(group->properties, group->properties_count * sizeof(napi_property_descriptor));
		group->properties[group->properties_count - 1] = member;
		return;
	}

	// static existing data
	napi_property_descriptor *props =
		calloc(group->properties_count, sizeof(napi_property_descriptor));

	for (uint32_t i = 0; i < group->properties_count - 1; i++) {
		props[i] = group->properties[i];
	}
	props[group->properties_count - 1] = member;

	group->properties = props;
	group->malloced = true;
}

void dsk_modexports_def_free(dsk_modexports_def *exports) {
	for (uint32_t i = 0; i < exports->members_count; i++) {
		dsk_export_def *def = exports->members[i];
		// free the properties of the export, allocated in dsk_export_def_register_member
		printf("def: %p props:%p\n", def, def->properties);
		if (def->malloced) {
			free(def->properties);
		}
		def->properties = NULL;

		// the  dsk_export_def itself is a static instance, so it cannot be freed.
	}
	printf("exports: %p members:%p\n", exports, exports->members);
	// free the members of the module, allocated in dsk_modexports_def_register_member
	// free(exports->members);
	exports->members = NULL;
	// the dsk_modexports_def itself is a static instance, so it cannot be freed.
}

napi_value dsk_init_module_def(napi_env env, napi_value exports, dsk_modexports_def *exports_def) {
	// Since this function is normally run during the process init,
	// the safes thing to do is to crash the process.
	DSK_ONERROR_FATAL_RET(NULL);
	napi_property_descriptor *properties =
		malloc(sizeof(napi_property_descriptor) * exports_def->members_count);
	printf("exports_def->members_count:%zu\n", exports_def->members_count);
	for (uint32_t i = 0; i < exports_def->members_count; i++) {
		printf("i:%d\n", i);
		dsk_export_def *def = exports_def->members[i];
		if (def->type == dsk_def_type_function) {
			assert(def->properties_count == 1);
			assert(def->properties != NULL);
			// first property is the function itself
			properties[i] = def->properties[0];
			continue;
		}

		if (def->type == dsk_def_type_class) {
			assert(def->properties_count >= 1);
			assert(def->properties != NULL);

			// first property contains name and constructor of the function
			napi_property_descriptor classDef = def->properties[0];

			printf("properties_count for class %zu\n", def->properties_count);
			// other ones contains members of the class (both static and instance members)
			napi_property_descriptor *classProperties =
				malloc(sizeof(napi_property_descriptor) * (def->properties_count - 1));

			for (uint32_t i = 0; i < (def->properties_count - 1); i++) {
				printf("i:%d\n", i);

				classProperties[i] = def->properties[i + 1];
			}

			printf("properties_count for class DNNE %zu %s\n", def->properties_count,
				   classDef.utf8name);

			napi_value Class;

			DSK_NAPI_CALL(napi_define_class(env, classDef.utf8name, NAPI_AUTO_LENGTH,
											classDef.method, NULL, def->properties_count - 1,
											classProperties, &Class));
			printf("napi_define_class %p\n", &Class);

			free(classProperties);

			napi_ref ignored_ref;
			napi_ref *ClassRef;
			if (classDef.data == NULL) {
				ClassRef = &ignored_ref;
			} else {
				ClassRef = classDef.data;
			}
			printf("napi_create_reference %p\n", ClassRef);

			DSK_NAPI_CALL(napi_create_reference(env, Class, 1, ClassRef));

			properties[i] = (napi_property_descriptor){
				.value = Class,
				.utf8name = classDef.utf8name,
			};
			continue;
		}

		if (def->type == dsk_def_type_object) {
			// data contains property of a singleton, that could contains
			// function, other objects or classes.
			assert("dsk_def_type_object NOT IMPLEMENTED" == NULL);
		}

		if (def->type == dsk_def_type_exec) {
			// data is a function to run at init time
			// that returns an array of props-defs to register on exports
			assert("dsk_def_type_exec NOT IMPLEMENTED" == NULL);
		}
	}

	DSK_NAPI_CALL(napi_define_properties(env, exports, exports_def->members_count, properties));

	// free the napi_properties data structures that are no more needed.
	dsk_modexports_def_free(exports_def);
	free(properties);
	return exports;
}