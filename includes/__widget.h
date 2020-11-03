#ifndef DSG_WIDGET_H__
#define DSG_WIDGET_H__ 1
#include "napi_utils.h"
#include "yoga/Yoga.h"

napi_value _dsk_define_class(napi_env env, napi_value exports, const char *name,
							 napi_callback constructor, const napi_property_descriptor properties[],
							 size_t propertiesCount, napi_ref *ref);

#define dsk_define_class(env, exports, name, constructor, properties)                              \
	_dsk_define_class(env, exports, name, constructor, (properties),                               \
					  sizeof((properties)) / sizeof(napi_property_descriptor), NULL)

#define dsk_define_class_ref(env, exports, name, constructor, properties, ref)                     \
	_dsk_define_class(env, exports, name, constructor, (properties),                               \
					  sizeof((properties)) / sizeof(napi_property_descriptor), ref)

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget);

#endif