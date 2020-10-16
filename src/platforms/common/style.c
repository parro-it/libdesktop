#include "napi_utils.h"
#include "widget.h"

#include <yoga/Yoga.h>

#define MODULE "style"

LIBUI_FUNCTION(styleNew) {
    INIT_ARGS(1);

    YGNodeRef node = YGNodeNew();
    napi_status status = napi_wrap(env, this, node, NULL, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    return this;
}


typedef void SetterI32(const YGNodeRef node, const int32_t value);
typedef int32_t GetterI32(const YGNodeConstRef node);

typedef void SetterF32(const YGNodeRef node, const float value);
typedef float GetterF32(const YGNodeConstRef node);


struct prop_fns {
    SetterI32* setterI32;
    GetterI32* getterI32;
    SetterF32* setterF32;
    GetterF32* getterF32;
};

static struct prop_fns mk_prop_fns_i32(GetterI32* getterI32, SetterI32* setterI32) {
   struct prop_fns fns= {.getterI32=getterI32,.setterI32=setterI32};
   return fns;
}

static struct prop_fns mk_prop_fns_f32(GetterF32* getterF32, SetterF32* setterF32) {
   struct prop_fns fns= {.getterF32=getterF32,.setterF32=setterF32};
   return fns;
}


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
    ARG_DOUBLE(value64, 0);                                                               
    
    YGNodeRef node;                                                                 
    napi_status status = napi_unwrap(env, this, (void**)&node);
    CHECK_STATUS_THROW(status, napi_unwrap);   

    struct prop_fns* fns; 
    
    status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&fns); 
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                         
    
    fns->setterF32(node, (float)value64);                                                                                     
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

#define PROP_I32(NAME,FNS) (napi_property_descriptor) {.utf8name = #NAME, .getter = getPropI32, .setter = setPropI32, .data = FNS}
#define PROP_F32(NAME,FNS) (napi_property_descriptor) {.utf8name = #NAME, .getter = getPropF32, .setter = setPropF32, .data = FNS}                        


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

    dsk_define_class(env,module,"Style",styleNew,((napi_property_descriptor[]){
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
      
      PROP_F32(flexGrow,&flex_grow_fns),
      PROP_F32(flex,&flex_fns),
      
    }));

    return exports;
}


/*





WIN_EXPORT void YGNodeStyleSetFlex(YGNodeRef node, float flex);
WIN_EXPORT float YGNodeStyleGetFlex(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetFlexGrow(YGNodeRef node, float flexGrow);
WIN_EXPORT float YGNodeStyleGetFlexGrow(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetFlexShrink(YGNodeRef node, float flexShrink);
WIN_EXPORT float YGNodeStyleGetFlexShrink(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetFlexBasis(YGNodeRef node, float flexBasis);
WIN_EXPORT void YGNodeStyleSetFlexBasisPercent(YGNodeRef node, float flexBasis);
WIN_EXPORT void YGNodeStyleSetFlexBasisAuto(YGNodeRef node);
WIN_EXPORT YGValue YGNodeStyleGetFlexBasis(YGNodeConstRef node);


WIN_EXPORT void YGNodeStyleSetPosition(
    YGNodeRef node,
    YGEdge edge,
    float position);
WIN_EXPORT void YGNodeStyleSetPositionPercent(
    YGNodeRef node,
    YGEdge edge,
    float position);
WIN_EXPORT YGValue YGNodeStyleGetPosition(YGNodeConstRef node, YGEdge edge);


WIN_EXPORT void YGNodeStyleSetMargin(YGNodeRef node, YGEdge edge, float margin);
WIN_EXPORT void YGNodeStyleSetMarginPercent(
    YGNodeRef node,
    YGEdge edge,
    float margin);
WIN_EXPORT void YGNodeStyleSetMarginAuto(YGNodeRef node, YGEdge edge);
WIN_EXPORT YGValue YGNodeStyleGetMargin(YGNodeConstRef node, YGEdge edge);

WIN_EXPORT void YGNodeStyleSetPadding(
    YGNodeRef node,
    YGEdge edge,
    float padding);
WIN_EXPORT void YGNodeStyleSetPaddingPercent(
    YGNodeRef node,
    YGEdge edge,
    float padding);
WIN_EXPORT YGValue YGNodeStyleGetPadding(YGNodeConstRef node, YGEdge edge);


WIN_EXPORT void YGNodeStyleSetBorder(YGNodeRef node, YGEdge edge, float border);
WIN_EXPORT float YGNodeStyleGetBorder(YGNodeConstRef node, YGEdge edge);

WIN_EXPORT void YGNodeStyleSetWidth(YGNodeRef node, float width);
WIN_EXPORT void YGNodeStyleSetWidthPercent(YGNodeRef node, float width);
WIN_EXPORT void YGNodeStyleSetWidthAuto(YGNodeRef node);
WIN_EXPORT YGValue YGNodeStyleGetWidth(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetHeight(YGNodeRef node, float height);
WIN_EXPORT void YGNodeStyleSetHeightPercent(YGNodeRef node, float height);
WIN_EXPORT void YGNodeStyleSetHeightAuto(YGNodeRef node);
WIN_EXPORT YGValue YGNodeStyleGetHeight(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetMinWidth(YGNodeRef node, float minWidth);
WIN_EXPORT void YGNodeStyleSetMinWidthPercent(YGNodeRef node, float minWidth);
WIN_EXPORT YGValue YGNodeStyleGetMinWidth(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetMinHeight(YGNodeRef node, float minHeight);
WIN_EXPORT void YGNodeStyleSetMinHeightPercent(YGNodeRef node, float minHeight);
WIN_EXPORT YGValue YGNodeStyleGetMinHeight(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetMaxWidth(YGNodeRef node, float maxWidth);
WIN_EXPORT void YGNodeStyleSetMaxWidthPercent(YGNodeRef node, float maxWidth);
WIN_EXPORT YGValue YGNodeStyleGetMaxWidth(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetMaxHeight(YGNodeRef node, float maxHeight);
WIN_EXPORT void YGNodeStyleSetMaxHeightPercent(YGNodeRef node, float maxHeight);
WIN_EXPORT YGValue YGNodeStyleGetMaxHeight(YGNodeConstRef node);

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