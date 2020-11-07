import {Control} from './control'
const {Textfield: NativeTextfield} = require("../../build/Release/desktop.node")
import {Event} from "./event";

/**
 * A static text label.
 */
export interface Textfield extends Control {
	/**
	 *  The text of the label.
	 */
	text: string;

	events: {
		click: Event;
	};
}


export const Textfield:{
	/**
	 * Create a new Textfield object.
	 *
	 * @param props - property of the text field to create.
	 * @param children - ignored
	 */
	new(props: any, children: Array<Control>): Textfield
} = NativeTextfield;
  