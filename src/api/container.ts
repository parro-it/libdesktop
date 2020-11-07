import {Control} from './control'
const {Container: NativeContainer} = require("../../build/Release/desktop.node")

export interface Container extends Control {
	
	/**
	 * Append a new child control as the last child.
	 * @param control - the control to add as a child.
	 */
	append(control: Control): void;

	/**
	 * Remove a child control.
	 * @param index - the index of the control to remove
	 */
	deleteAt(index: number): void;
}

export const Container:{
	/**
	 * Create a new Container object.
	 *
	 * @param props - property of the widget to create.
	 * @param children - children to add to this container.
	 */
	new(props: any, children: Array<Control>): Container
} = NativeContainer;
  