import {Control} from './control'
const {textfield} = require("../../build/Release/desktop.node")

/**
 * A static text label.
 */
export interface Textfield extends Control {
	/**
	 *  The text of the label.
	 */
	text: string;
}


export const Textfield:{
	/**
	 * Create a new Textfield object.
	 *
	 * @param props - property of the text field to create.
	 * @param children - ignored
	 */
	new(props: any, children: Array<Control>): Textfield
} = textfield.Textfield;
  