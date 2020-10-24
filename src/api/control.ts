import {Style} from "./style";
/**
 * Base class of all controls except UiWindow
 */
export interface Control {
	/**
	 * Set or return whether the control is enabled.
	 */
	enabled: boolean;
	/**
	 * Set or return whether the control is visible.
	 */
	visible: boolean;
	/**
	 * Set or return whether the control is a top level one.
	 */
	toplevel: boolean;
	

	left: number;
	top: number;
	
	/**
	 * Create a new UiControl object.
	 */
	readonly style:Style;
}
