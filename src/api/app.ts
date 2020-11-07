const {App: NativeApp, wakeupBackgroundThread,startEventLoop, stopEventLoop } = require("../../build/Release/desktop.node")
import {createHook,AsyncHook} from 'async_hooks';

let wakingup = false;
let asyncHook: AsyncHook;

export interface App {
    readonly arch: string;
    readonly ver: string;
    start(): Promise<void>;
    stop(): Promise<void>;
}

export const App: {
    new(): App
    create(): App
} = NativeApp;

App.prototype.wakeupBackgroundThread = wakeupBackgroundThread

App.prototype.start = () => {
	asyncHook = createHook({init: initAsyncResource});

	// Allow callbacks of this AsyncHook instance to call. This is not an
	// implicit action after running the constructor, and must be explicitly run
	// to begin executing callbacks.
	asyncHook.enable();

	return new Promise((resolve, reject) => startEventLoop(resolve));
};

App.prototype.stop = () => {
	// Allow callbacks of this AsyncHook instance to call. This is not an
	// implicit action after running the constructor, and must be explicitly run
	// to begin executing callbacks.
	asyncHook.disable();

	return new Promise((resolve, reject) => stopEventLoop(resolve));
};


// This is called when a new async handle
// is created. It's used to signal the background
// thread to stop awaiting calls and upgrade it's list of handles
// it's awaiting for.
function initAsyncResource() {
	if (wakingup) {
		return;
	}
	wakingup = true;
	setImmediate(() => {
		wakeupBackgroundThread();
		wakingup = false;
	});
}
