#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#import <Cocoa/Cocoa.h>
#import "yoga/Yoga.h"
#define MODULE "label"



@interface DskTextField : NSTextField 
	@property (nonatomic, readwrite) napi_value wrapper;
    @property (nonatomic, readwrite) YGNodeRef yoganode;
@end

@implementation DskTextField
@end


LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

    DskTextField *widget;
	widget = [[DskTextField alloc] initWithFrame:NSZeroRect];
	[widget setStringValue: @"ciao mac"];
	[widget setEditable:NO];
	[widget setSelectable:NO];
	[widget setDrawsBackground:NO];
    [widget setHidden:NO];
   	widget.translatesAutoresizingMaskIntoConstraints = true;
    widget.frame = NSMakeRect(0,0,100,40);

    
    dsk_wrap_widget(env, widget, this);

    return this;
}

	

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    
    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       //DSK_RWPROP_BOOL(visible,"visible"),
       //DSK_RWPROP_BOOL(visible,"enabled"),
       //DSK_CHILDPROP_I32(left,"x"),
       //DSK_CHILDPROP_I32(top,"y")
    }));
    return exports;
}