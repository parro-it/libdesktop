import './tests/style.ts'
import test from 'tape-async'
import {App,Window,Label,Container} from './index'
const app = App.create()
test('run on multiple platforms', async (t:any): Promise<void> => {
    t.equal(app.arch, process.platform)
    t.equal(app.ver, "0.0.1")
})
/*

test('start stop', async (t:any): Promise<void> => {
    t.equal(typeof app.start, "function")
    t.equal(typeof app.stop, "function")

    await Promise.all([
        (async (): Promise<void> =>{
            console.log("call start")
            await app.start()
            console.log("start resolved")
        })(),
        new Promise(resolve=>setTimeout(
            async (): Promise<void> =>{
                console.log("call stop")
                await app.stop()
                console.log("stop resolved")
                resolve()
            },
            100
        )),
    ])
})
test('Window', async (t:any): Promise<void> => {
    t.equal(typeof Window, "function")
    const app = App.create()
    const [l1,l2,l3] = [
        new Label({},[]),
        new Label({},[]),
        new Label({},[])
    ]
    
    const win = new Window({},[
        new Container({},[l1,l2,l3]),
        new Container({},[
            new Label({},[]),
            new Label({},[]),
            new Label({},[])
        ]),
        new Container({},[
            new Label({},[]),
            new Label({},[]),
            new Label({},[])
        ]),
    ])

    console.log({win})
    console.log(win.title)
    win.title = "Prova";
    win.width=640
    win.height=480
    win.visible = true;
    
    console.log(win.title)
    app.start()
})
*/