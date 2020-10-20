#include "napi_utils.h"
#include "widget.h"

#include <yoga/Yoga.h>

#define MODULE "style"



typedef void SetterI32(const YGNodeRef node, const int32_t value);
typedef int32_t GetterI32(const YGNodeConstRef node);

typedef void SetterF32(const YGNodeRef node, const float value);
typedef float GetterF32(const YGNodeConstRef node);
typedef YGValue GetterYGVALUE(const YGNodeConstRef node);

typedef YGValue GetterEdgedYGVALUE(const YGNodeConstRef node, YGEdge edge);
typedef float GetterEdgedF32(const YGNodeConstRef node, YGEdge edge);
typedef void SetterEdgedF32(const YGNodeRef node, YGEdge edge, const float value);

struct prop_fns {
    SetterI32* setterI32;
    GetterI32* getterI32;
    SetterF32* setterF32;
    GetterF32* getterF32;
    GetterYGVALUE* getterYGValue;
    char* unit;
    
};

struct EdgedPropData {
    GetterEdgedYGVALUE* getter;
    SetterEdgedF32* setter;
    GetterEdgedF32* getter_f;
    YGNodeRef node;
    YGUnit unit;
};


static struct prop_fns mk_prop_fns_i32(GetterI32* getterI32, SetterI32* setterI32) {
   struct prop_fns fns= {.getterI32=getterI32,.setterI32=setterI32};
   return fns;
}

static struct prop_fns mk_prop_fns_f32(GetterF32* getterF32, SetterF32* setterF32) {
   struct prop_fns fns= {.getterF32=getterF32,.setterF32=setterF32};
   return fns;
}

static struct prop_fns mk_prop_fns_ygvalue(char* unit, GetterYGVALUE* getter, SetterF32* setter) {
   struct prop_fns fns= {.getterYGValue=getter,.setterF32=setter,.unit=unit};
   return fns;
}

//static LIBUI_FUNCTION( edgedFloatGetter);
static LIBUI_FUNCTION( edgedFloatSetter);
static LIBUI_FUNCTION( edgedYgValueGetter);




LIBUI_FUNCTION(setPropI32) {                                                           
    INIT_ARGS(1);                                                                      
    ARG_INT32(value, 0);                                                               
    
    YGNodeRef node;                                                                 
    napi_status status = napi_unwrap(env, this, (void**)&node);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    struct prop_fns* fns; 
    
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&fns); 
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                         
    
    fns->setterI32(node, value);                                                                                     
    return NULL;                                                                       
}                                                                                      
                                                                                       
LIBUI_FUNCTION(getPropI32) {                                                           
    INIT_EMPTY_ARGS();                                                                 
    
    YGNodeRef node;                                                                 
    napi_status status = napi_unwrap(env, this, (void**)&node);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    struct prop_fns* fns; 
    
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&fns); 
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                         
                                                                   
                                                                                       
    int32_t result = fns->getterI32(node);
                                                                                       
    return make_int32(env, result);                                                    
}                                                                                      

LIBUI_FUNCTION(setPropF32) {                                                           
    INIT_ARGS(1);                                                                      
    ARG_DOUBLE(value, 0);                                                               
    
    YGNodeRef node;                                                                 
    napi_status status = napi_unwrap(env, this, (void**)&node);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    struct prop_fns* fns; 
    
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&fns); 
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                         
    
    fns->setterF32(node, (float)value);                                                                                     
    return NULL;                                                                       
}                                                                                      
                                                                                       
LIBUI_FUNCTION(getPropF32) {                                                           
    INIT_EMPTY_ARGS();                                                                 
    
    YGNodeRef node;                                                                 
    napi_status status = napi_unwrap(env, this, (void**)&node);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    struct prop_fns* fns; 
    
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&fns); 
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                         
                                                                   
                                                                                       
    float result = fns->getterF32(node);
                                                                                       
    return make_double(env, (double)result);                                                    
}  

LIBUI_FUNCTION(getPropYGValue) {                                                           
    INIT_EMPTY_ARGS();                                                                 
    
    YGNodeRef node;                                                                 
    napi_status status = napi_unwrap(env, this, (void**)&node);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    struct prop_fns* fns; 
    
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&fns); 
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                         
                                                                   
                                                                                       
    YGValue result = fns->getterYGValue(node);
    
    if (fns->unit=="AUTO") {
        return make_bool(env, result.unit==YGUnitAuto);
    }                                                                                      

    if (fns->unit=="POINT") {
        if (result.unit!=YGUnitPoint) {
            return make_double(env,NAN);
        }
        return make_double(env, result.value);
    }  

    if (fns->unit=="PERCENT") {
        if (result.unit!=YGUnitPercent) {
            return make_double(env,NAN);
        }
        return make_double(env, result.value);
    }                                                                                      

    napi_throw_error(env, "EINVAL", "Unknown unit");
    return NULL;
}  



static struct prop_fns direction_fns;
static struct prop_fns flex_direction_fns;
static struct prop_fns justify_content_fns;
static struct prop_fns position_type_fns;
static struct prop_fns align_content_fns;
static struct prop_fns align_items_fns;
static struct prop_fns align_self_fns;
static struct prop_fns flex_wrap_fns;
static struct prop_fns overflow_fns;
static struct prop_fns display_fns;
static struct prop_fns flex_fns;
static struct prop_fns flex_grow_fns;
static struct prop_fns flex_shrink_fns;
static struct prop_fns flex_basis_fns;
static struct prop_fns flex_basis_percent_fns;
static struct prop_fns flex_basis_auto_fns;
static struct prop_fns width_fns;
static struct prop_fns width_percent_fns;
static struct prop_fns height_fns;
static struct prop_fns height_percent_fns;
static struct prop_fns min_width_fns;
static struct prop_fns min_width_percent_fns;
static struct prop_fns min_height_fns;
static struct prop_fns min_height_percent_fns;
static struct prop_fns max_width_fns;
static struct prop_fns max_width_percent_fns;
static struct prop_fns max_height_fns;
static struct prop_fns max_height_percent_fns;

//static struct prop_fns position_fns;



#define PROP_I32(NAME,FNS) (napi_property_descriptor) {.utf8name = #NAME, .getter = getPropI32, .setter = setPropI32, .data = FNS}
#define PROP_F32(NAME,FNS) (napi_property_descriptor) {.utf8name = #NAME, .getter = getPropF32, .setter = setPropF32, .data = FNS}                        
#define PROP_YGVALUE_F32(NAME,FNS) (napi_property_descriptor) {.utf8name = #NAME, .getter = getPropYGValue, .setter = setPropF32, .data = FNS}


napi_ref EdgedPropRef;
napi_ref StyleRef;

napi_value mk_edged_prop(napi_env env, YGNodeRef node, YGUnit unit, GetterEdgedYGVALUE* getter, SetterEdgedF32* setter) {
    napi_value constructor;
    napi_status status;
    napi_value edgedProp;

    status = napi_get_reference_value(env, EdgedPropRef, &constructor);
    CHECK_STATUS_THROW(status, napi_get_reference_value);                                          

    status = napi_new_instance(env,constructor,0,NULL,&edgedProp);
    CHECK_STATUS_THROW(status,napi_new_instance);

    struct EdgedPropData* data; 
    status = napi_unwrap(env, edgedProp, (void**)&data);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    data->getter = getter;
    data->setter = setter;
    data->node = node;
    data->unit = unit;

    return edgedProp;
}

napi_value mk_edged_prop_f(napi_env env, YGNodeRef node, YGUnit unit, GetterEdgedF32* getter, SetterEdgedF32* setter) {
    napi_value constructor;
    napi_status status;
    napi_value edgedProp;

    status = napi_get_reference_value(env, EdgedPropRef, &constructor);
    CHECK_STATUS_THROW(status, napi_get_reference_value);                                          

    status = napi_new_instance(env,constructor,0,NULL,&edgedProp);
    CHECK_STATUS_THROW(status,napi_new_instance);

    struct EdgedPropData* data; 
    status = napi_unwrap(env, edgedProp, (void**)&data);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    data->getter_f = getter;
    data->setter = setter;
    data->node = node;
    data->unit = unit;

    return edgedProp;
}

#define EDGED_PROP(NAME,UNIT,GETTER,SETTER) \
    napi_define_properties(env,this,1, (napi_property_descriptor[]) {{\
        .utf8name=#NAME,\
        .value=mk_edged_prop(env, node, UNIT, GETTER, SETTER),\
        .attributes=napi_enumerable,  /*!napi_configurable !napi_writable*/ \
    }})

#define EDGED_PROP_F(NAME,GETTER,SETTER) \
    napi_define_properties(env,this,1, (napi_property_descriptor[]) {{\
        .utf8name=#NAME,\
        .value=mk_edged_prop_f(env, node, 0xbabe, GETTER, SETTER),\
        .attributes=napi_enumerable,  /*!napi_configurable !napi_writable*/ \
    }})

static void set_margin_auto(YGNodeRef node, YGEdge edge, float margin) {
    if (margin == 0.0) {
        YGNodeStyleSetMargin(node,edge,0.0);    
    }
    YGNodeStyleSetMarginAuto(node,edge);
}

static napi_value link_style_to_node(napi_env env, YGNodeRef node, napi_value this) {
    napi_status status = napi_wrap(env, this, node, NULL, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          
    
    EDGED_PROP(position, YGUnitPoint, YGNodeStyleGetPosition, YGNodeStyleSetPosition);
    EDGED_PROP(positionPercent, YGUnitPercent, YGNodeStyleGetPosition, YGNodeStyleSetPositionPercent);
   
    EDGED_PROP_F(border, YGNodeStyleGetBorder, YGNodeStyleSetBorder);

    EDGED_PROP(padding, YGUnitPoint, YGNodeStyleGetPadding, YGNodeStyleSetPadding);
    EDGED_PROP(paddingPercent, YGUnitPercent, YGNodeStyleGetPadding, YGNodeStyleSetPaddingPercent);
   
    EDGED_PROP(margin,        YGUnitPoint,     YGNodeStyleGetMargin, YGNodeStyleSetMargin);
    EDGED_PROP(marginPercent, YGUnitPercent,   YGNodeStyleGetMargin, YGNodeStyleSetMarginPercent);
    EDGED_PROP(marginAuto,    YGUnitAuto,      YGNodeStyleGetMargin, set_margin_auto);

    return NULL;
}


LIBUI_FUNCTION(styleNew) {
    INIT_ARGS(2);
    napi_valuetype argType;

    YGNodeRef node;
    printf("argc %d\n",argc);
    if (argc < 2) {
        node = YGNodeNew();
    } else {
        napi_typeof(env, argv[1], &argType);
        if (argType == napi_null) {
            printf("napi_null\n");
            node = YGNodeNew();
        } else {
            printf("get node\n");
            node = dsg_widget_get_node(env, argv[1]);
        }
    }


    
    link_style_to_node(env, node, this);
    return this;
}



static void finalize_edgedProp(napi_env env, void *finalize_data, void *finalize_hint) {
    free(finalize_data);
}

LIBUI_FUNCTION(edgedPropNew) {
    INIT_EMPTY_ARGS();

    struct EdgedPropData* data = malloc(sizeof (struct EdgedPropData));  

    napi_status status = napi_wrap(env, this, data, finalize_edgedProp, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    return this;
}


typedef void SetterF32(const YGNodeRef node, const float value);
typedef float GetterF32(const YGNodeConstRef node);
typedef YGValue GetterYGVALUE(const YGNodeConstRef node);



static LIBUI_FUNCTION(edgedFloatSetter) {                    
    INIT_ARGS(1);                                                                      
    ARG_DOUBLE(value, 0);                                                               
    
    struct EdgedPropData* data; 
    napi_status status = napi_unwrap(env, this, (void**)&data);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    YGEdge* edge; 
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&edge); 
    CHECK_STATUS_THROW(status, napi_get_cb_info); 

    data->setter(data->node, *edge, (float)value);
    
    return NULL;
}

/*
static LIBUI_FUNCTION(edgedFloatGetter) {                    
    INIT_EMPTY_ARGS();                                                          
    
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
static LIBUI_FUNCTION(edgedYgValueGetter) {
    INIT_EMPTY_ARGS();
    struct EdgedPropData* data; 
    napi_status status = napi_unwrap(env, this, (void**)&data);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    YGEdge* edge; 
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&edge); 
    CHECK_STATUS_THROW(status, napi_get_cb_info); 

    if (data->unit==0xbabe) {
       float result = data->getter_f(data->node, *edge);
        return make_double(env,result);
    }                                                                                      

    YGValue result = data->getter(data->node, *edge);

    if (data->unit==YGUnitAuto) {
        return make_bool(env, result.unit==YGUnitAuto);
    }                                                                                      

    if (data->unit==YGUnitPoint) {
        if (result.unit!=YGUnitPoint) {
            return make_double(env,NAN);
        }
        return make_double(env, result.value);
    }  

    if (data->unit==YGUnitPercent) {
        if (result.unit!=YGUnitPercent) {
            return make_double(env,NAN);
        }
        return make_double(env, result.value);
    }                                                                                      

    napi_throw_error(env, "EINVAL", "Unknown unit");
    return NULL;
}


static YGEdge edgeAll = YGEdgeAll;
static YGEdge edgeLeft = YGEdgeLeft;
static YGEdge edgeTop = YGEdgeTop;
static YGEdge edgeRight = YGEdgeRight;
static YGEdge edgeBottom = YGEdgeBottom;
static YGEdge edgeStart = YGEdgeStart;
static YGEdge edgeEnd = YGEdgeEnd;
static YGEdge edgeHorizontal = YGEdgeHorizontal;
static YGEdge edgeVertical = YGEdgeVertical;


#define EDGED_PROP_YGVALUE(NAME, EDGE) (napi_property_descriptor) {.utf8name = #NAME, .getter = edgedYgValueGetter, .setter = edgedFloatSetter, .data = &EDGE}

static void set_style_auto(const YGNodeRef node, float _) {
    YGNodeStyleSetFlexBasisAuto(node);
}

napi_value style_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    direction_fns = mk_prop_fns_i32((GetterI32*)YGNodeStyleGetDirection, (SetterI32*)YGNodeStyleSetDirection);
    flex_direction_fns = mk_prop_fns_i32((GetterI32*)YGNodeStyleGetFlexDirection, (SetterI32*)YGNodeStyleSetFlexDirection);
    justify_content_fns = mk_prop_fns_i32((GetterI32*)YGNodeStyleGetJustifyContent, (SetterI32*)YGNodeStyleSetJustifyContent);
    align_content_fns = mk_prop_fns_i32((GetterI32*)YGNodeStyleGetAlignContent, (SetterI32*)YGNodeStyleSetAlignContent);
    align_items_fns = mk_prop_fns_i32((GetterI32*)YGNodeStyleGetAlignItems, (SetterI32*)YGNodeStyleSetAlignItems);
    align_self_fns = mk_prop_fns_i32((GetterI32*)YGNodeStyleGetAlignSelf, (SetterI32*)YGNodeStyleSetAlignSelf);
    position_type_fns= mk_prop_fns_i32((GetterI32*)YGNodeStyleGetPositionType, (SetterI32*)YGNodeStyleSetPositionType);
    flex_wrap_fns= mk_prop_fns_i32((GetterI32*)YGNodeStyleGetFlexWrap, (SetterI32*)YGNodeStyleSetFlexWrap);
    overflow_fns= mk_prop_fns_i32((GetterI32*)YGNodeStyleGetOverflow, (SetterI32*)YGNodeStyleSetOverflow);
    display_fns= mk_prop_fns_i32((GetterI32*)YGNodeStyleGetDisplay, (SetterI32*)YGNodeStyleSetDisplay);
    flex_fns= mk_prop_fns_f32((GetterF32*)YGNodeStyleGetFlex, (SetterF32*)YGNodeStyleSetFlex);
    flex_grow_fns= mk_prop_fns_f32((GetterF32*)YGNodeStyleGetFlexGrow, (SetterF32*)YGNodeStyleSetFlexGrow);
    flex_shrink_fns= mk_prop_fns_f32((GetterF32*)YGNodeStyleGetFlexShrink, (SetterF32*)YGNodeStyleSetFlexShrink);
    flex_basis_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetFlexBasis, (SetterF32*)   YGNodeStyleSetFlexBasis         );
    flex_basis_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetFlexBasis, (SetterF32*)   YGNodeStyleSetFlexBasisPercent  );
    flex_basis_auto_fns         = mk_prop_fns_ygvalue("AUTO",(GetterYGVALUE*)       YGNodeStyleGetFlexBasis, (SetterF32*)   set_style_auto     );
    width_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetWidth, (SetterF32*)   YGNodeStyleSetWidth         );
    width_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetWidth, (SetterF32*)   YGNodeStyleSetWidthPercent  );
    height_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetHeight, (SetterF32*)   YGNodeStyleSetHeight         );
    height_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetHeight, (SetterF32*)   YGNodeStyleSetHeightPercent  );
    min_width_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetMinWidth, (SetterF32*)   YGNodeStyleSetMinWidth         );
    min_width_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetMinWidth, (SetterF32*)   YGNodeStyleSetMinWidthPercent  );
    min_height_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetMinHeight, (SetterF32*)   YGNodeStyleSetMinHeight         );
    min_height_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetMinHeight, (SetterF32*)   YGNodeStyleSetMinHeightPercent  );
    max_width_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetMaxWidth, (SetterF32*)   YGNodeStyleSetMaxWidth         );
    max_width_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetMaxWidth, (SetterF32*)   YGNodeStyleSetMaxWidthPercent  );
    max_height_fns              = mk_prop_fns_ygvalue("POINT",(GetterYGVALUE*)      YGNodeStyleGetMaxHeight, (SetterF32*)   YGNodeStyleSetMaxHeight         );
    max_height_percent_fns      = mk_prop_fns_ygvalue("PERCENT",(GetterYGVALUE*)    YGNodeStyleGetMaxHeight, (SetterF32*)   YGNodeStyleSetMaxHeightPercent  );



    dsk_define_class_ref(env,module,"EdgedProp",edgedPropNew,((napi_property_descriptor[]){
      EDGED_PROP_YGVALUE(left, edgeLeft),
      EDGED_PROP_YGVALUE(top, edgeTop),
      EDGED_PROP_YGVALUE(right, edgeRight),
      EDGED_PROP_YGVALUE(bottom, edgeBottom),
      EDGED_PROP_YGVALUE(start, edgeStart),
      EDGED_PROP_YGVALUE(end, edgeEnd),
      EDGED_PROP_YGVALUE(horizontal, edgeHorizontal),
      EDGED_PROP_YGVALUE(vertical, edgeVertical),
      EDGED_PROP_YGVALUE(all, edgeAll),
    }), &EdgedPropRef);

    
    dsk_define_class_ref(env,module,"Style",styleNew,((napi_property_descriptor[]){
      PROP_I32(direction,&direction_fns),
      PROP_I32(flexDirection,&flex_direction_fns),
      PROP_I32(justifyContent,&justify_content_fns),
      PROP_I32(alignContent,&align_content_fns),
      PROP_I32(alignItems,&align_items_fns),
      PROP_I32(alignSelf,&align_self_fns),
      PROP_I32(positionType,&position_type_fns),
      PROP_I32(flexWrap,&flex_wrap_fns),
      PROP_I32(overflow,&overflow_fns),
      PROP_I32(display,&display_fns),
      PROP_F32(flex,&flex_fns),
      PROP_F32(flexGrow,&flex_grow_fns),
      PROP_F32(flexShrink,&flex_shrink_fns),

      PROP_YGVALUE_F32(flexBasis  ,&flex_basis_fns),
      PROP_YGVALUE_F32(flexBasisPercent,  &flex_basis_percent_fns),
      PROP_YGVALUE_F32(flexBasisAuto,  &flex_basis_auto_fns),
      PROP_YGVALUE_F32(width, &width_fns),
      PROP_YGVALUE_F32(widthPercent, &width_percent_fns),
      PROP_YGVALUE_F32(height, &height_fns),
      PROP_YGVALUE_F32(heightPercent, &height_percent_fns),
      PROP_YGVALUE_F32(minWidth, &min_width_fns),
      PROP_YGVALUE_F32(minWidthPercent, &min_width_percent_fns),
      PROP_YGVALUE_F32(minHeight, &min_height_fns),
      PROP_YGVALUE_F32(minHeightPercent, &min_height_percent_fns),
      PROP_YGVALUE_F32(maxWidth, &max_width_fns),
      PROP_YGVALUE_F32(maxWidthPercent, &max_width_percent_fns),
      PROP_YGVALUE_F32(maxHeight, &max_height_fns),
      PROP_YGVALUE_F32(maxHeightPercent, &max_height_percent_fns),
    }), &StyleRef);

    return exports;
}


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