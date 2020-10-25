const {event} = require("../../build/Release/desktop.node")


export interface EventHandler {
    stop(): void;
}

export interface Event {
    
    listen(listener: Function): EventHandler;
    invoke(): void;
}


export const Event:{
	/**
	 * Create a new Event object.
	 
	 */
	new(name:string,sender:any): Event
} = event.Event;
  