#include "control.h"
#include "napi_utils.h"

static void widget_reposition(napi_env env, UIHandle container, YGNodeRef root) {
    uint32_t childrenCount = YGNodeGetChildCount(root);
    for (uint32_t i=0; i < childrenCount; i++) {
        YGNodeRef childNode = YGNodeGetChild(root,i);
        UIHandle childHandle = YGNodeGetContext(childNode);
        dsk_widget_reposition(
            env, container, childHandle, 
            YGNodeLayoutGetLeft(childNode), 
            YGNodeLayoutGetTop(childNode),
            YGNodeLayoutGetWidth(childNode), 
            YGNodeLayoutGetHeight(childNode)
        );
        widget_reposition(env,childHandle, childNode);
    }
}

void dsk_calculate_layout(napi_env env, UIHandle container, YGNodeRef root,float availableWidth,float availableHeight) {
    // YGNodePrint(root,YGPrintOptionsChildren | YGPrintOptionsStyle  );
    YGNodeCalculateLayout(root,availableWidth,availableHeight,YGDirectionInherit);
    widget_reposition(env,container,root);
    // YGNodePrint(root,YGPrintOptionsChildren | YGPrintOptionsLayout);
}

void dsk_add_child(napi_env env, UIHandle parentHandle, UIHandle childHandle) {
    napi_value parent = dsk_widget_wrapper(env, parentHandle);
    napi_value child = dsk_widget_wrapper(env, childHandle);
    // printf("handles: %p->%p   values: %p->%p\n",childHandle,parentHandle,child,parent);
    // printf("dsk_platform_container_add_child 1\n");
    dsk_platform_container_add_child(parentHandle, childHandle);
    

    YGNodeRef node = dsk_widget_get_node(env, parent);
    // printf("node %p\n",node);
    YGNodeRef childNode = dsk_widget_get_node(env, child);
    // printf("childNode %p\n",childNode);
    uint32_t childrenCount = YGNodeGetChildCount(node);
    YGNodeInsertChild(node, childNode, childrenCount);
}

void dsk_append_all_children(napi_env env, UIHandle widget,napi_value children) {
    uint32_t len;
    napi_get_array_length(env, children, &len);
    // printf("len %d\n", len);
    for (uint32_t i=0; i < len; i++) {
        // printf("i %d\n", i);
        napi_value idx;
        napi_create_uint32(env,i,&idx);
        napi_value child;
        napi_get_property(env,children,idx,&child);
        
        UIHandle childHandle;
        napi_unwrap(env,child,(void**)&childHandle);
        // printf("childHandle %d: %p\n", i, childHandle);

        dsk_add_child(env, widget, childHandle);
    }
}