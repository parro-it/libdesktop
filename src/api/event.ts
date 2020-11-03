const {Event: NativeEvent} = require("../../build/Release/desktop.node")


export interface EventHandler {
    stop(): void;
}

export interface Event {
    
    listen(listener: Function): EventHandler;
    invoke(arg: any): void;
}

export const Event:{
	new(name:string,sender:any): Event
} = NativeEvent;
  