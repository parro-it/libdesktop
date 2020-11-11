#include "yoga.h"
#import <Cocoa/Cocoa.h>

#include "libdesktop-host.h"
#include "libdesktop.h"
#import <CoreFoundation/CoreFoundation.h>
#import <dlfcn.h> // see future.m
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#include <uv.h>

static BOOL (^isRunning)(void);
static BOOL stepsIsRunning;

NAPI_C_CTOR(_dsk_init_proc) {
	ProcessSerialNumber psn = {0, kCurrentProcess};
	TransformProcessType(&psn, kProcessTransformToForegroundApplication);
	printf("Process initialized\n");
}

/*
static BOOL canQuit = NO;
static NSAutoreleasePool *globalPool;
static uiprivApplicationClass *app;
static uiprivAppDelegate *delegate;


@implementation uiprivApplicationClass

- (void)sendEvent:(NSEvent *)e
{
	if (uiprivSendAreaEvents(e) != 0)
		return;
	[super sendEvent:e];
}

// NSColorPanel always sends changeColor: to the first responder regardless of whether there's a
target set on it
// we can override it here (see colorbutton.m)
// thanks to mikeash in irc.freenode.net/#macdev for informing me this is how the first responder
chain is initiated
// it turns out NSFontManager also sends changeFont: through this; let's inhibit that here too (see
fontbutton.m)
- (BOOL)sendAction:(SEL)sel to:(id)to from:(id)from
{
	if (uiprivColorButtonInhibitSendAction(sel, from, to))
		return NO;
	if (uiprivFontButtonInhibitSendAction(sel, from, to))
		return NO;
	return [super sendAction:sel to:to from:from];
}

// likewise, NSFontManager also sends NSFontPanelValidation messages to the first responder, however
it does NOT use sendAction:from:to:!
// instead, it uses this one (thanks swillits in irc.freenode.net/#macdev)
// we also need to override it (see fontbutton.m)
- (id)targetForAction:(SEL)sel to:(id)to from:(id)from
{
	id override;

	if (uiprivFontButtonOverrideTargetForAction(sel, from, to, &override))
		return override;
	return [super targetForAction:sel to:to from:from];
}

// hey look! we're overriding terminate:!
// we're going to make sure we can go back to main() whether Cocoa likes it or not!
// and just how are we going to do that, hm?
// (note: this is called after applicationShouldTerminate:)
- (void)terminate:(id)sender
{
	// yes that's right folks: DO ABSOLUTELY NOTHING.
	// the magic is [NSApp run] will just... stop.

	// well let's not do nothing; let's actually quit our graceful way
	NSEvent *e;

	if (!canQuit)
		uiprivImplBug("call to [NSApp terminate:] when not ready to terminate; definitely contact
andlabs");

	[[NSApplication sharedApplication] stop:[NSApplication sharedApplication]];
	// stop: won't register until another event has passed; let's synthesize one
	e = [NSEvent otherEventWithType:NSApplicationDefined
		location:NSZeroPoint
		modifierFlags:0
		timestamp:[[NSProcessInfo processInfo] systemUptime]
		windowNumber:0
		context:[NSGraphicsContext currentContext]
		subtype:0
		data1:0
		data2:0];
	[[NSApplication sharedApplication] postEvent:e atStart:NO];		// let pending events take
priority (this is what PostQuitMessage() on Windows does so we have to do it here too for parity;
thanks to mikeash in irc.freenode.net/#macdev for confirming that this parameter should indeed be
NO)

	// and in case uiMainSteps() was called
	stepsIsRunning = NO;
}

@end

@implementation uiprivAppDelegate

- (void)dealloc
{
	// Apple docs: "Don't Use Accessor Methods in Initializer Methods and dealloc"
	[_menuManager release];
	[super dealloc];
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)app
{
	// for debugging
	NSLog(@"in applicationShouldTerminate:");
	if (uiprivShouldQuit()) {
		canQuit = YES;
		// this will call terminate:, which is the same as dsk_quit()
		return NSTerminateNow;
	}
	return NSTerminateCancel;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app
{
	return NO;
}

@end

dsk_initOptions uiprivOptions;

void uiUninit(void)
{
	if (!globalPool)
		uiprivUserBug("You must call dsk_init() first!");
	[globalPool release];

	@autoreleasepool {
		uiprivUninitUnderlineColors();
		[delegate release];
		[[NSApplication sharedApplication] setDelegate:nil];
		[app release];
		uiprivUninitAlloc();
	}
}

void uiFreeInitError(const char *err)
{
}

void uiMain(void)
{
	isRunning = ^{
		return [[NSApplication sharedApplication] isRunning];
	};
	[[NSApplication sharedApplication] run];
}



// thanks to mikeash in irc.freenode.net/#macdev for suggesting the use of Grand Central Dispatch
for this
// LONGTERM will dispatch_get_main_queue() break after _CFRunLoopSetCurrent()?
void uiQueueMain(void (*f)(void *data), void *data)
{
	// dispatch_get_main_queue() is a serial queue so it will not execute multiple uiQueueMain()
functions concurrently
	// the signature of f matches dispatch_function_t
	dispatch_async_f(dispatch_get_main_queue(), data, f);
}


@end
*/

// TODO figure out the best way to clean the above up in uiUninit(), if it's even necessary
// TODO that means figure out if timers can still fire without the main loop

;
typedef struct uiprivNextEventArgs {
	NSEventMask mask;
	NSDate *duration;
	// LONGTERM no NSRunLoopMode?
	NSString *mode;
	BOOL dequeue;
} uiprivNextEventArgs;

// see also:
// - http://www.cocoawithlove.com/2009/01/demystifying-nsapplication-by.html
// - https://github.com/gnustep/gui/blob/master/Source/NSApplication.m
int uiprivMainStep(uiprivNextEventArgs *nea, BOOL (^interceptEvent)(NSEvent *e)) {
	// NSDate *expire;
	NSEvent *e;
	NSEventType type;

	@autoreleasepool {
		if (!isRunning())
			return 0;

		e = [[NSApplication sharedApplication] nextEventMatchingMask:nea->mask
														   untilDate:nea->duration
															  inMode:nea->mode
															 dequeue:nea->dequeue];
		if (e == nil)
			return 1;

		type = [e type];
		if (!interceptEvent(e))
			[[NSApplication sharedApplication] sendEvent:e];
		[[NSApplication sharedApplication] updateWindows];

		// GNUstep does this
		// it also updates the Services menu but there doesn't seem to be a public API for that so
		/*if (type != NSPeriodic && type != NSMouseMoved)
			[[[NSApplication sharedApplication] mainMenu] update];
*/
		return 1;
	}
}

void dsk_quit(void) {
	// canQuit = YES;
	[[NSApplication sharedApplication] terminate:[NSApplication sharedApplication]];
}

int dsk_process_ui_event(int wait) {
	uiprivNextEventArgs nea;

	nea.mask = NSEventMaskAny;

	// ProPuke did this in his original PR requesting this
	// I'm not sure if this will work, but I assume it will...
	nea.duration = [NSDate distantPast];
	if (wait) // but this is normal so it will work
		nea.duration = [NSDate distantFuture];

	nea.mode = NSDefaultRunLoopMode;
	nea.dequeue = YES;

	return uiprivMainStep(&nea, ^(NSEvent *e) {
	  return NO;
	});
}

void uiMainSteps(void) {
	// SDL does this and it seems to be necessary for the menubar to work (see #182)
	[[NSApplication sharedApplication] finishLaunching];
	isRunning = ^{
	  return stepsIsRunning;
	};
	stepsIsRunning = YES;
}

const char *dsk_init() {
	/*@autoreleasepool {
		app = [[uiprivApplicationClass sharedApplication] retain];
		// don't check for a NO return; something (launch services?) causes running from application
	bundles to always return NO when asking to change activation policy, even if the change is to
	the same activation policy!
		// see https://github.com/andlabs/ui/issues/6
		[[NSApplication sharedApplication]
	setActivationPolicy:NSApplicationActivationPolicyRegular]; delegate = [uiprivAppDelegate new];
		[[NSApplication sharedApplication] setDelegate:delegate];

		uiprivInitAlloc();
		uiprivLoadFutures();
		uiprivLoadUndocumented();

		// always do this so we always have an application menu
		uiprivAppDelegate().menuManager = [[uiprivMenuManager new] autorelease];
		[[NSApplication sharedApplication] setMainMenu:[uiprivAppDelegate().menuManager
	makeMenubar]];

		uiprivSetupFontPanel();

		uiprivInitUnderlineColors();
	}


	globalPool = [[NSAutoreleasePool alloc] init];
*/

	return NULL;
}

int dsk_ui_events_pending() {

	NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
										untilDate:[NSDate distantPast]
										   inMode:NSDefaultRunLoopMode
										  dequeue:NO];
	return nil != event;
}

int dsk_wait_node_events(uv_loop_t *loop, int timeout) {
	int nodeBackendFd = uv_backend_fd(loop);
	if (nodeBackendFd == -1) {
		fprintf(stderr, "Invalid node backend fd.\n");
		return 0;
	}

	struct kevent event;
	struct timespec ts;
	struct timespec *tsp = NULL;

	if (timeout != -1) {
		tsp = &ts;
		ts.tv_sec = timeout / 1000;
		ts.tv_nsec = (timeout % 1000) * 1000000;
	}

	return kevent(nodeBackendFd, NULL, 0, &event, 1, tsp);
}

int dsk_wakeup_ui_loop() {
	[[NSApplication sharedApplication]
		postEvent:[NSEvent otherEventWithType:NSEventTypeApplicationDefined
									 location:NSZeroPoint
								modifierFlags:0
									timestamp:0.0
								 windowNumber:0
									  context:nil
									  subtype:0
										data1:0
										data2:0]
		  atStart:NO];

	return 0;
}

@interface DskControl : NSView
@property napi_ref wrapper;
@property YGNodeRef yoganode;
@end

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget) {
	// printf("unwrap %p\n", widget);
	DskControl *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		// printf("non ok \n");
		const napi_extended_error_info *result;
		napi_get_last_error_info(env, &result);
		// printf("napi_unwrap failed with code %d: %s\n", result->engine_error_code,
		// result->error_message);
		return NULL;
	}
	// printf("widgetG %p\n",widgetG);

	return widgetG.yoganode;
}

void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node) {
	DskControl *widgetG;
	napi_status status = napi_unwrap(env, widget, (void **)&widgetG);
	if (status != napi_ok) {
		return;
	}

	// printf("node %p\n",node);

	widgetG.yoganode = node;

	napi_ref ref;
	napi_create_reference(env, widget, 1, &ref);

	// printf("ref %p\n",ref);
	widgetG.wrapper = ref;
}

napi_value dsk_widget_wrapper(napi_env env, UIHandle widget) {
	napi_ref ref = ((DskControl *)widget).wrapper;
	if (ref == NULL) {
		return NULL;
	}
	// printf("ref is %p\n",ref);
	napi_value node;
	napi_get_reference_value(env, ref, &node);
	return node;
}

void dsk_get_preferred_sizes(UIHandle widget, int *width, int *height) {
	NSView *view = widget;
	NSSize sz = [view fittingSize];

	*width = sz.width;
	if (*width < 130) {
		*width = 130;
	}
	*height = sz.height;
}

void dsk_connect_event(UIHandle widget, char *eventname, struct dsk_event_args *args) {}

void dsk_ui_set_prop_s(void *instance, char *value, void **datas) {
	NSObject *widget = instance;
	char *propname = datas[2];
	[widget setValue:[NSString stringWithUTF8String:value]
			  forKey:[NSString stringWithUTF8String:propname]];
}

char *dsk_ui_get_prop_s(void *instance, void **datas) {
	NSObject *widget = instance;
	char *propname = datas[2];
	NSString *ns_result = [widget valueForKey:[NSString stringWithUTF8String:propname]];
	char *result = (char *)[ns_result UTF8String];
	return result;
}

void dsk_ui_set_prop_i32(void *instance, int32_t value, void **datas) {
	/*NSObject *widget = instance;
	char *propname = datas[2];

	if (strcmp(propname, "left") == 0 || strcmp(propname, "top")) {
		GtkContainer *container = GTK_CONTAINER(gtk_widget_get_parent(widget));
		if (container == NULL) {
			printf("ERROR: THIS WIDGET HAS NO CONTAINER\n");
			return;
		}
		GValue val = G_VALUE_INIT;
		g_value_init(&val, G_TYPE_INT);
		g_value_set_int(&val, value);
		gtk_container_child_set_property(container, widget, propname, &val);
		return;
	}

	g_object_set(widget, propname, value, NULL);*/
}

int32_t dsk_ui_get_prop_i32(void *instance, void **datas) {
	/*NSObject *widget = instance;
	char *propname = datas[2];

	if (strcmp(propname, "left") == 0 || strcmp(propname, "top")) {
		GtkContainer *container = GTK_CONTAINER(gtk_widget_get_parent(widget));
		GValue value;
		gtk_container_child_get_property(container, widget, propname, &value);
		return g_value_get_int(&value);
	}

	int32_t result;
	g_object_get(widget, propname, &result, NULL);

	return result;*/
	return 0;
}

void dsk_ui_set_prop_bool(void *instance, bool value, void **datas) {
	/*NSObject *widget = instance;
	char *propname = datas[2];
	g_object_set(widget, propname, value, NULL);*/
}
bool dsk_ui_get_prop_bool(void *instance, void **datas) {
	/*NSObject *widget = instance;
	char *propname = datas[2];
	bool result;
	g_object_get(widget, propname, &result, NULL);
	return result;*/
	return true;
}
