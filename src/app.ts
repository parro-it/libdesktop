const {app} = require("../build/Release/desktop.node")



export class App {
    static create ():App {
        //console.log(process.cwd())
        //console.log(app.createApp())
        return new App()
    }

    get common():string {
        return app.createApp()
    }

    get arch():string {
        return app.appArch()
    }
}