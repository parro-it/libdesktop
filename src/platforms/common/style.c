#include "libdesktop.h"
#include "napi_utils.h"
#include "widget.h"

#include <yoga/Yoga.h>
DSK_EXTEND_MODULE(libdesktop);
_DSK_USE_CLASS(libdesktop, EdgedProp);

typedef void SetterI32(const YGNodeRef node, const int32_t value);
typedef int32_t GetterI32(const YGNodeConstRef node);

typedef void SetterF32(const YGNodeRef node, const float value);
typedef float GetterF32(const YGNodeConstRef node);
typedef YGValue GetterYGVALUE(const YGNodeConstRef node);

typedef YGValue GetterEdgedYGVALUE(const YGNodeConstRef node, YGEdge edge);
typedef float GetterEdgedF32(const YGNodeConstRef node, YGEdge edge);
typedef void SetterEdgedF32(const YGNodeRef node, YGEdge edge, const float value);

struct EdgedPropData {
	GetterEdgedYGVALUE *getter;
	SetterEdgedF32 *setter;
	GetterEdgedF32 *getter_f;
	YGNodeRef node;
	YGUnit unit;
};

DSK_JS_FUNC(setPropI32) {
	DSK_JS_FUNC_INIT()
	ARG_INT32(value, 0);

	YGNodeRef node;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&node));

	void **fns;
	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&fns));

	SetterI32 *setter = fns[1];

	setter(node, value);
	return NULL;
}

DSK_JS_FUNC(getPropI32) {
	DSK_JS_FUNC_INIT()

	YGNodeRef node;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&node));

	void **fns;

	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&fns));

	GetterI32 *getter = fns[0];
	int32_t result = getter(node);

	return make_int32(env, result);
}

DSK_JS_FUNC(setPropF32) {
	DSK_JS_FUNC_INIT()
	ARG_DOUBLE(value, 0);

	YGNodeRef node;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&node));

	void **fns;
	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&fns));

	SetterF32 *setter = fns[1];
	setter(node, (float)value);
	return NULL;
}

DSK_JS_FUNC(getPropF32) {
	DSK_JS_FUNC_INIT()

	YGNodeRef node;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&node));

	void **fns;
	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&fns));

	GetterF32 *getter = fns[0];
	float result = getter(node);

	return make_double(env, (double)result);
}

DSK_JS_FUNC(getPropYGValue) {
	DSK_JS_FUNC_INIT()

	YGNodeRef node;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&node));

	void **fns;
	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&fns));

	GetterYGVALUE *getter = fns[0];
	char *unit = fns[2];
	YGValue result = getter(node);

	if (strcmp(unit, "AUTO") == 0) {
		return make_bool(env, result.unit == YGUnitAuto);
	}

	if (strcmp(unit, "POINT") == 0) {
		if (result.unit != YGUnitPoint) {
			return make_double(env, NAN);
		}
		return make_double(env, result.value);
	}

	if (strcmp(unit, "PERCENT") == 0) {
		if (result.unit != YGUnitPercent) {
			return make_double(env, NAN);
		}
		return make_double(env, result.value);
	}

	napi_throw_error(env, "EINVAL", "Unknown unit");
	return NULL;
}

#define PROP_I32(NAME, NATIVE_GETTER, NATIVE_SETTER)                                               \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, getPropI32, setPropI32,                           \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER}))

#define PROP_F32(NAME, NATIVE_GETTER, NATIVE_SETTER)                                               \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, getPropF32, setPropF32,                           \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER}))

#define PROP_YGVALUE_F32(NAME, UNIT, NATIVE_GETTER, NATIVE_SETTER)                                 \
	DSK_DEFINE_PROPERTY(libdesktop, Style, NAME, getPropYGValue, setPropF32,                       \
						((void *[]){NATIVE_GETTER, NATIVE_SETTER, #UNIT}))

napi_value mk_edged_prop(napi_env env, YGNodeRef node, YGUnit unit, GetterEdgedYGVALUE *getter,
						 SetterEdgedF32 *setter) {
	napi_value constructor;
	napi_status status;
	napi_value edgedProp;

	status = napi_get_reference_value(env, libdesktop_EdgedProp_ref, &constructor);
	CHECK_STATUS_THROW(status, napi_get_reference_value);

	status = napi_new_instance(env, constructor, 0, NULL, &edgedProp);
	CHECK_STATUS_THROW(status, napi_new_instance);

	struct EdgedPropData *data;
	status = napi_unwrap(env, edgedProp, (void **)&data);
	CHECK_STATUS_THROW(status, napi_unwrap);

	data->getter = getter;
	data->setter = setter;
	data->node = node;
	printf("YGUnit %d\n", unit);
	data->unit = unit;

	return edgedProp;
}

napi_value mk_edged_prop_f(napi_env env, YGNodeRef node, YGUnit unit, GetterEdgedF32 *getter,
						   SetterEdgedF32 *setter) {
	napi_value constructor;
	napi_status status;
	napi_value edgedProp;

	status = napi_get_reference_value(env, libdesktop_EdgedProp_ref, &constructor);
	CHECK_STATUS_THROW(status, napi_get_reference_value);

	status = napi_new_instance(env, constructor, 0, NULL, &edgedProp);
	CHECK_STATUS_THROW(status, napi_new_instance);

	struct EdgedPropData *data;
	status = napi_unwrap(env, edgedProp, (void **)&data);
	CHECK_STATUS_THROW(status, napi_unwrap);

	data->getter_f = getter;
	data->setter = setter;
	data->node = node;
	data->unit = unit;

	return edgedProp;
}

#define EDGED_PROP(NAME, UNIT, GETTER, SETTER)                                                     \
	napi_define_properties(                                                                        \
		env, this, 1,                                                                              \
		(napi_property_descriptor[]){{                                                             \
			.utf8name = #NAME,                                                                     \
			.value = mk_edged_prop(env, node, UNIT, GETTER, SETTER),                               \
			.attributes = napi_enumerable, /*!napi_configurable !napi_writable*/                   \
		}})

#define EDGED_PROP_F(NAME, GETTER, SETTER)                                                         \
	napi_define_properties(                                                                        \
		env, this, 1,                                                                              \
		(napi_property_descriptor[]){{                                                             \
			.utf8name = #NAME,                                                                     \
			.value = mk_edged_prop_f(env, node, 0xbabe, GETTER, SETTER),                           \
			.attributes = napi_enumerable, /*!napi_configurable !napi_writable*/                   \
		}})

static void set_margin_auto(YGNodeRef node, YGEdge edge, float margin) {
	if (margin == 0.0) {
		YGNodeStyleSetMargin(node, edge, 0.0);
	}
	YGNodeStyleSetMarginAuto(node, edge);
}

DSK_DEFINE_CLASS(libdesktop, Style) {
	DSK_JS_FUNC_INIT()
	DSK_EXACTLY_NARGS(2);
	napi_valuetype argType;

	YGNodeRef node;
	// printf("argc %zd\n",argc);
	if (argc < 2) {
		node = YGNodeNew();
	} else {
		DSK_NAPI_CALL(napi_typeof(env, argv[1], &argType));
		if (argType == napi_null) {
			node = YGNodeNew();
		} else {
			node = dsk_widget_get_node(env, argv[1]);
		}
	}

	DSK_NAPI_CALL(napi_wrap(env, this, node, NULL, NULL, NULL));

	EDGED_PROP(position, YGUnitPoint, YGNodeStyleGetPosition, YGNodeStyleSetPosition);
	EDGED_PROP(positionPercent, YGUnitPercent, YGNodeStyleGetPosition,
			   YGNodeStyleSetPositionPercent);

	EDGED_PROP_F(border, YGNodeStyleGetBorder, YGNodeStyleSetBorder);

	EDGED_PROP(padding, YGUnitPoint, YGNodeStyleGetPadding, YGNodeStyleSetPadding);
	EDGED_PROP(paddingPercent, YGUnitPercent, YGNodeStyleGetPadding, YGNodeStyleSetPaddingPercent);

	EDGED_PROP(margin, YGUnitPoint, YGNodeStyleGetMargin, YGNodeStyleSetMargin);
	EDGED_PROP(marginPercent, YGUnitPercent, YGNodeStyleGetMargin, YGNodeStyleSetMarginPercent);
	EDGED_PROP(marginAuto, YGUnitAuto, YGNodeStyleGetMargin, set_margin_auto);

	return this;
}

static void set_style_auto(const YGNodeRef node, float _) {
	YGNodeStyleSetFlexBasisAuto(node);
}

PROP_I32(direction, YGNodeStyleGetDirection, YGNodeStyleSetDirection);
PROP_I32(flexDirection, YGNodeStyleGetFlexDirection, YGNodeStyleSetFlexDirection);
PROP_I32(justifyContent, YGNodeStyleGetJustifyContent, YGNodeStyleSetJustifyContent);
PROP_I32(alignContent, YGNodeStyleGetAlignContent, YGNodeStyleSetAlignContent);
PROP_I32(alignItems, YGNodeStyleGetAlignItems, YGNodeStyleSetAlignItems);
PROP_I32(alignSelf, YGNodeStyleGetAlignSelf, YGNodeStyleSetAlignSelf);
PROP_I32(positionType, YGNodeStyleGetPositionType, YGNodeStyleSetPositionType);
PROP_I32(flexWrap, YGNodeStyleGetFlexWrap, YGNodeStyleSetFlexWrap);
PROP_I32(overflow, YGNodeStyleGetOverflow, YGNodeStyleSetOverflow);
PROP_I32(display, YGNodeStyleGetDisplay, YGNodeStyleSetDisplay);
PROP_F32(flex, YGNodeStyleGetFlex, YGNodeStyleSetFlex);
PROP_F32(flexGrow, YGNodeStyleGetFlexGrow, YGNodeStyleSetFlexGrow);
PROP_F32(flexShrink, YGNodeStyleGetFlexShrink, YGNodeStyleSetFlexShrink);
PROP_YGVALUE_F32(flexBasis, POINT, YGNodeStyleGetFlexBasis, YGNodeStyleSetFlexBasis);
PROP_YGVALUE_F32(flexBasisPercent, PERCENT, YGNodeStyleGetFlexBasis,
				 YGNodeStyleSetFlexBasisPercent);
PROP_YGVALUE_F32(flexBasisAuto, AUTO, YGNodeStyleGetFlexBasis, set_style_auto);
PROP_YGVALUE_F32(width, POINT, YGNodeStyleGetWidth, YGNodeStyleSetWidth);
PROP_YGVALUE_F32(widthPercent, PERCENT, YGNodeStyleGetWidth, YGNodeStyleSetWidthPercent);
PROP_YGVALUE_F32(height, POINT, YGNodeStyleGetHeight, YGNodeStyleSetHeight);
PROP_YGVALUE_F32(heightPercent, PERCENT, YGNodeStyleGetHeight, YGNodeStyleSetHeightPercent);
PROP_YGVALUE_F32(minWidth, POINT, YGNodeStyleGetMinWidth, YGNodeStyleSetMinWidth);
PROP_YGVALUE_F32(minWidthPercent, PERCENT, YGNodeStyleGetMinWidth, YGNodeStyleSetMinWidthPercent);
PROP_YGVALUE_F32(minHeight, POINT, YGNodeStyleGetMinHeight, YGNodeStyleSetMinHeight);
PROP_YGVALUE_F32(minHeightPercent, PERCENT, YGNodeStyleGetMinHeight,
				 YGNodeStyleSetMinHeightPercent);
PROP_YGVALUE_F32(maxWidth, POINT, YGNodeStyleGetMaxWidth, YGNodeStyleSetMaxWidth);
PROP_YGVALUE_F32(maxWidthPercent, PERCENT, YGNodeStyleGetMaxWidth, YGNodeStyleSetMaxWidthPercent);
PROP_YGVALUE_F32(maxHeight, POINT, YGNodeStyleGetMaxHeight, YGNodeStyleSetMaxHeight);
PROP_YGVALUE_F32(maxHeightPercent, PERCENT, YGNodeStyleGetMaxHeight,
				 YGNodeStyleSetMinHeightPercent);

static DSK_JS_FUNC(edgedFloatSetter) {
	DSK_JS_FUNC_INIT();
	DSK_AT_LEAST_NARGS(1)

	double value;
	DSK_NAPI_CALL(napi_get_value_double(env, argv[0], &value));

	struct EdgedPropData *data;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&data));

	YGEdge *edge;
	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&edge));
	data->setter(data->node, *edge, (float)value);

	return NULL;
}

/*

static DSK_JS_FUNC(edgedFloatGetter) {
	DSK_JS_FUNC_INIT();


	struct EdgedPropData* data;
	napi_status status = napi_unwrap(env, this, (void**)&data);
	CHECK_STATUS_THROW(status, napi_unwrap);

	YGEdge* edge;
	status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&edge);
	CHECK_STATUS_THROW(status, napi_get_cb_info);

	float result = data->getter_f(data->node, *edge);
	return make_double(env,result);
}
*/
static DSK_JS_FUNC(edgedYgValueGetter) {
	DSK_JS_FUNC_INIT();

	struct EdgedPropData *data;
	DSK_NAPI_CALL(napi_unwrap(env, this, (void **)&data));

	YGEdge *edge;
	DSK_NAPI_CALL(napi_get_cb_info(env, info, NULL, NULL, NULL, (void **)&edge));

	if (data->unit == 0xbabe) {
		float result = data->getter_f(data->node, *edge);
		napi_value ret;
		DSK_NAPI_CALL(napi_create_double(env, result, &ret));
		return ret;
	}

	YGValue result = data->getter(data->node, *edge);

	if (data->unit == YGUnitAuto) {
		return make_bool(env, result.unit == YGUnitAuto);
	}

	if (data->unit == YGUnitPoint) {
		napi_value ret;

		if (result.unit != YGUnitPoint) {
			DSK_NAPI_CALL(napi_create_double(env, NAN, &ret));
			return ret;
		}
		DSK_NAPI_CALL(napi_create_double(env, result.value, &ret));
		return ret;
	}

	if (data->unit == YGUnitPercent) {
		napi_value ret;
		if (result.unit != YGUnitPercent) {
			DSK_NAPI_CALL(napi_create_double(env, NAN, &ret));
			return ret;
		}
		DSK_NAPI_CALL(napi_create_double(env, result.value, &ret));
		return ret;
	}

	DSK_FAILURE("Unknown unit");
	return NULL;
}

#define EDGED_PROP_YGVALUE(NAME, EDGE)                                                             \
	DSK_DEFINE_PROPERTY(libdesktop, EdgedProp, NAME, edgedYgValueGetter, edgedFloatSetter,         \
						(YGEdge[]){EDGE})

static void finalize_edgedProp(napi_env env, void *finalize_data, void *finalize_hint) {
	free(finalize_data);
}

DSK_DEFINE_CLASS(libdesktop, EdgedProp) {
	DSK_JS_FUNC_INIT();

	struct EdgedPropData *data = malloc(sizeof(struct EdgedPropData));

	DSK_NAPI_CALL(napi_wrap(env, this, data, finalize_edgedProp, NULL, NULL));

	return this;
}

EDGED_PROP_YGVALUE(left, YGEdgeLeft)
EDGED_PROP_YGVALUE(top, YGEdgeTop)
EDGED_PROP_YGVALUE(right, YGEdgeRight)
EDGED_PROP_YGVALUE(bottom, YGEdgeBottom)
EDGED_PROP_YGVALUE(start, YGEdgeStart)
EDGED_PROP_YGVALUE(end, YGEdgeEnd)
EDGED_PROP_YGVALUE(horizontal, YGEdgeHorizontal)
EDGED_PROP_YGVALUE(vertical, YGEdgeVertical)
EDGED_PROP_YGVALUE(all, YGEdgeAll)

/*




// Yoga specific properties, not compatible with flexbox specification Aspect
// ratio control the size of the undefined dimension of a node. Aspect ratio is
// encoded as a floating point value width/height. e.g. A value of 2 leads to a
// node with a width twice the size of its height while a value of 0.5 gives the
// opposite effect.
//
// - On a node with a set width/height aspect ratio control the size of the
//   unset dimension
// - On a node with a set flex basis aspect ratio controls the size of the node
//   in the cross axis if unset
// - On a node with a measure function aspect ratio works as though the measure
//   function measures the flex basis
// - On a node with flex grow/shrink aspect ratio controls the size of the node
//   in the cross axis if unset
// - Aspect ratio takes min/max dimensions into account
WIN_EXPORT void YGNodeStyleSetAspectRatio(YGNodeRef node, float aspectRatio);
WIN_EXPORT float YGNodeStyleGetAspectRatio(YGNodeConstRef node);
*/