#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>

#define MODULE "container"

static void container_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    GtkWidget* widget = finalize_data;
    YGNodeRef node = g_object_get_data(G_OBJECT( widget),"yoganode");
    YGNodeFree(node);
}

void add_child_gtk(GtkWidget* parent, GtkWidget* child) {
    gtk_container_add(GTK_CONTAINER(parent), child);
    YGNodeRef node = g_object_get_data(G_OBJECT(parent),"yoganode");
    YGNodeRef childNode = g_object_get_data(G_OBJECT(child),"yoganode");
    uint32_t childrenCount = YGNodeGetChildCount(node);
    YGNodeInsertChild(node, childNode, childrenCount);
}

LIBUI_FUNCTION(containerAppend) {
    INIT_ARGS(1);

   	GtkWidget* widget;
    DSK_UNWRAP_WIDGET();
    
    return this;
}

void calculate_layout_gtk(GtkContainer* container) {
    YGNodeRef node = g_object_get_data(G_OBJECT(container),"yoganode");
    
    YGNodeCalculateLayout(node,800,600,YGDirectionInherit);
    uint32_t childrenCount = YGNodeGetChildCount(node);
    
    for (uint32_t i=0; i < childrenCount; i++) {
        YGNodeRef childNode = YGNodeGetChild(node,i);
        GtkWidget* child_gtk = YGNodeGetContext(childNode);
        GValue x = G_VALUE_INIT;
        g_value_init(&x,G_TYPE_INT);
        g_value_set_int(&x,YGNodeLayoutGetLeft(childNode));
        GValue y = G_VALUE_INIT;
        g_value_init(&y,G_TYPE_INT);
        g_value_set_int(&y,YGNodeLayoutGetTop(childNode));
        gtk_container_child_set_property(container, child_gtk, "x", &x);
        gtk_container_child_set_property(container, child_gtk, "y", &y);
        
        printf("X:Y = %f:%f\n",YGNodeLayoutGetLeft(childNode),YGNodeLayoutGetTop(childNode));
    }
}

GtkWidget* container_new_gtk() {
    GtkWidget* widget = gtk_fixed_new();
    YGNodeRef node = YGNodeNew();
    YGNodeStyleSetFlexDirection(node,YGFlexDirectionRow);
    g_object_set_data(G_OBJECT(widget),"yoganode", node);
    YGNodeSetContext(node, widget);
    return widget;
}

void append_all_children_gtk(napi_env env,GtkWidget* widget,napi_value children) {
    uint32_t len;
    napi_get_array_length(env, children, &len);
    printf("container children count %d\n", len);
    for (uint32_t i=0; i < len; i++) {
        printf("add %d\n",i);
        napi_value idx;
        napi_create_uint32(env,i,&idx);
        napi_value child;
        napi_get_property(env,children,idx,&child);
        GtkWidget* child_gtk;
        napi_unwrap(env,child,(void**)&child_gtk);
        add_child_gtk(widget, child_gtk);
    }
}

LIBUI_FUNCTION(containerNew) {
    INIT_ARGS(2);

   	GtkWidget* widget = container_new_gtk();
    
    napi_status status = napi_wrap(env, this, widget, container_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          
        
    append_all_children_gtk(env,widget,argv[1]);

    return this;
}

napi_value container_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    dsk_define_class(env,module,"Container",containerNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
    }));

    return exports;
}

