import {Control} from './control'
const {label} = require("../../build/Release/desktop.node")

/**
 * A static text label.
 */
export interface Label extends Control {
	/**
	 *  The text of the label.
	 */
	text: string;
}


export const Label:{
	/**
	 * Create a new Label object.
	 *
	 * @param props - property of the label to create.
	 * @param children - ignored
	 */
	new(props: any, children: Array<Control>): Label
} = label.Label;
  