
/**
 * The UiWindow class is responsible for showing and managing native windows.
 * A UiWindow instance can contain only one child. If you need
 * more than one, you have to use [Containers](containers.md).
 */
export interface UiWindow {
	/**
	 * Create a new UiWindow object.
	 *
	 * @param title - text to show in window title bar.
	 * @param width - width of window in pixels.
	 * @param height -  height of window in pixels.
	 * @param hasMenubar - whether this window has a menu bar.
	 * See [Menu](#menubar).
	 */
	//constructor(title: string, width: number, height: number, hasMenubar: boolean);

	/**
	 * Show the window.
	 * LibUi always returns null
	 */
	show(): void;

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
	 * Set the control to show in this window content area.
	 * UiWindow instances can contain only one child. If you need
	 * more, you have to use [Containers](containers.md).
	 *
	 * @param control - the control to add as child.
	 * @param stretchy - whever the control should fill all the available space.
	 */
	setChild(control: any): void;

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
}