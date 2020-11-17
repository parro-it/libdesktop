#include "dsktest.h"
#include "libdesktop.h"

#import <Cocoa/Cocoa.h>



@interface DskNSRootContainer : NSView
	@property(nonatomic, readwrite) DskCtrlI *DskCtrlI;
	@property(readonly) bool isFlipped;
@end

@implementation DskNSRootContainer
- (bool)isFlipped {
	return true;
}

- (BOOL)acceptsFirstResponder {
	return YES;
}
@end



UIHandle dsk_new_container_platform_ui_control() {
	return  [[DskNSRootContainer alloc] init];
} 