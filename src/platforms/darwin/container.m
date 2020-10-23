#include "napi_utils.h"
#include "widget.h"
#include <yoga/Yoga.h>
#import <Cocoa/Cocoa.h>

#include "control.h"

#define MODULE "container"


@interface DskContainer : NSView 
    @property napi_value wrapper;
    @property YGNodeRef yoganode;
    @property (readonly) bool isFlipped;

@end

@implementation DskContainer
    -(bool)isFlipped
    {
        return true;
    }
@end

void dsk_widget_move(napi_env env, UIHandle container, UIHandle widget, float xcoord, float ycoord) {
    NSView* view = (NSView*) widget;
    // printf("MOVE %f %f\n",xcoord,ycoord);
    [view setFrame: NSMakeRect(xcoord,ycoord, 150, 60)];
}

void dsk_platform_container_add_child(UIHandle parent, UIHandle child) {
    NSView* cnt = parent;
    // printf("\n\nCHILD: %p\n\n");

    [cnt addSubview:(NSView*)child];
}

LIBUI_FUNCTION(containerNew) {
    INIT_ARGS(2);

    DskContainer* widget = [[DskContainer alloc] init];
   	//widget.translatesAutoresizingMaskIntoConstraints = true;
    widget.frame = NSMakeRect(0,0,800,600);
    //[widget setHidden:NO];
    dsk_wrap_widget(env, widget, this);
        
    dsk_append_all_children(env, widget, argv[1]);

    
    return this;
}

napi_ref ContainerRef;

napi_value container_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    dsk_define_class_ref(env,module,"Container",containerNew,((napi_property_descriptor[]){
       //DSK_RWPROP_BOOL(visible,"visible"),
       //DSK_RWPROP_BOOL(enabled,"enabled"),
    }), &ContainerRef);

    return exports;
}