import { Control } from "./control";

const {Window: NativeWindow} = require("../../build/Release/desktop.node")

/**
 * The Window class is responsible for showing and managing native windows.
 * A Window instance can contain only one child. If you need
 * more than one, you have to use [Containers](containers.md).
 */
export interface Window {
	
	/**
	 * Add a listener to the `closing` event. This event is emitted when the OS
	 * window close button is clicked. You are responsible for closing the
	 * window in response to this event.
	 *
	 * @param callback - callback to execute when the event is fired.
	 */
	onClosing(callback: () => any): void;

	/**
	 * Add a listener to the `contentSizeChanged` event. This event is emitted
	 * when window content size change.
	 *
	 * @param callback - callback to execute when the event is fired.
	 */
	onContentSizeChanged(callback: Function): void;

	/**
	 * Close the window.
	 */
	close(): void;

	/**
	 * Saves the content of a window to a PNG file
	 * @param filename path to save to
	 */
	saveAsPNGImage(filename: string): void;

	/**
	 * Set or return the text to show in window title bar.
	 */
	title: string;

	/**
	 * Set or return the size in pixel of the content area of the window
	 * (excluding the size of the window decoration). This mean that if you set window
	 * size to 0,0 you still see title bar and OS window buttons.
	 */
	contentSize: {width: number; height: number};


	/**
	 * When true, an internal margin is added to the window.
	 */
	margined: boolean;

	/**
	 * When true, the window is displayed without a border.
	 */
	borderless: boolean;

	/**
	 * When true, the window is displayed in full screen mode.
	 */
	fullscreen: boolean;

	/**
	 * Set or return whether the window is visible.
	 */
	visible: boolean;

	/**
	 * Set or return the width of the window.
	 */
	width: number;

	/**
	 * Set or return the height of the window.
	 */
	height: number;


}


export const Window: {
	/**
	 * Create a new Window object.
	 *
	 * @param props - property of the windows to create.
	 * @param children - children widgets of the window
	 */
	new(props: any, children: Array<Control>): Window
} = NativeWindow;
