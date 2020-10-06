const {app} = require("./build/Release/desktop.node")

export interface App {
    readonly arch: string;
    readonly ver: string;
}

export const App: {
    new(): App
    create(): App
} = app.App;