import './tests/style.ts'
import './tests/event.ts'
import './tests/dsknapi.ts'
import './tests/dskctrl.ts'


import test from 'tape-async'
import {App,Window,Label,Textfield,Container,YogaFlexDirection} from './index'
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
*/
test('Window', async (t:any): Promise<void> => {
    t.equal(typeof Window, "function")
    const app = App.create()
    
    const lbl = new Label({ style: { margin: { left: 0, top: 0 }, minWidth: 120 }, text: "Given Name" }, []);
    const fld = new Textfield({ style: { flexGrow: 1, margin: { left: 12, top: 0 } }, text: "Andrea" }, [])

    const win = new Window({ title: "Running on " + process.platform, style: { padding: { left: 26, right: 26, top: 12, bottom: 12 } } }, [
        new Container({ style: { flexDirection: YogaFlexDirection.Row } }, [
            lbl,
            fld,
        ]),
        new Container({ style: { flexGrow: 1, flexDirection: YogaFlexDirection.Row } }, [
            new Label({ style: { margin: { left: 0, top: 12 }, minWidth: 120 }, text: "Family Name" }, []),
            new Textfield({ style: { flexGrow: 1, margin: { left: 12, top: 12 } }, text: "Parodi" }, []),
        ]),

        new Container({ style: { flexDirection: YogaFlexDirection.Row } }, [
            new Label({ style: { margin: { left: 0, top: 12 }, minWidth: 120 }, text: "City" }, []),
            new Textfield({ style: { flexGrow: 1, margin: { left: 12, top: 12 } }, text: "Genoa" }, []),
        ]),
    ]);

    app.start();
    setTimeout(()=>{
        win.saveAsPNGImage("win.png");
        console.log("saved");
        win.close();
        app.stop();

    },600);
    

    //(l1 as any).print();
})
