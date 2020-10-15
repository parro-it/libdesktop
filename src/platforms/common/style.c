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


typedef void Setter(const YGNodeRef node,const int32_t value);
typedef int32_t Getter(const YGNodeConstRef node);

struct prop_fns {
    Setter* setter;
    Getter* getter;
};

static struct prop_fns mk_prop_fns(Getter* getter, Setter* setter) {
   struct prop_fns fns= {.getter=getter,.setter=setter};
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
    
    fns->setter(node, value);                                                                                     
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
                                                                   
                                                                                       
    int32_t result = fns->getter(node);
                                                                                       
    return make_int32(env, result);                                                    
}                                                                                      

static struct prop_fns direction_fns;
static struct prop_fns flex_direction_fns;
static struct prop_fns justify_content_fns;

#define PROP_I32(NAME,FNS) (napi_property_descriptor) {.utf8name = #NAME, .getter = getPropI32, .setter = setPropI32, .data = FNS}                        


napi_value style_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    direction_fns = mk_prop_fns((Getter*)YGNodeStyleGetDirection, (Setter*)YGNodeStyleSetDirection);
    flex_direction_fns = mk_prop_fns((Getter*)YGNodeStyleGetFlexDirection, (Setter*)YGNodeStyleSetFlexDirection);
    justify_content_fns = mk_prop_fns((Getter*)YGNodeStyleGetJustifyContent, (Setter*)YGNodeStyleSetJustifyContent);
    
    dsk_define_class(env,module,"Style",styleNew,((napi_property_descriptor[]){
      PROP_I32(direction,&direction_fns),
      PROP_I32(flexDirection,&flex_direction_fns),
      PROP_I32(justifyContent,&justify_content_fns),
    }));

    return exports;
}


/*

WIN_EXPORT void (
    YGNodeRef node,
    YGJustify justifyContent);
WIN_EXPORT YGJustify YGNodeStyleGetJustifyContent(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetAlignContent(
    YGNodeRef node,
    YGAlign alignContent);
WIN_EXPORT YGAlign YGNodeStyleGetAlignContent(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetAlignItems(YGNodeRef node, YGAlign alignItems);
WIN_EXPORT YGAlign YGNodeStyleGetAlignItems(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetAlignSelf(YGNodeRef node, YGAlign alignSelf);
WIN_EXPORT YGAlign YGNodeStyleGetAlignSelf(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetPositionType(
    YGNodeRef node,
    YGPositionType positionType);
WIN_EXPORT YGPositionType YGNodeStyleGetPositionType(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetFlexWrap(YGNodeRef node, YGWrap flexWrap);
WIN_EXPORT YGWrap YGNodeStyleGetFlexWrap(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetOverflow(YGNodeRef node, YGOverflow overflow);
WIN_EXPORT YGOverflow YGNodeStyleGetOverflow(YGNodeConstRef node);

WIN_EXPORT void YGNodeStyleSetDisplay(YGNodeRef node, YGDisplay display);
WIN_EXPORT YGDisplay YGNodeStyleGetDisplay(YGNodeConstRef node);

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