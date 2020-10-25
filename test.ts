//import './tests/style.ts'
import test from 'tape-async'
import {App,Window,Label,Container,YogaFlexDirection} from './index'
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
    const [l1,l2,l3] = [
        new Label({style: {
            margin:{left:6,top:6}
        },
        label:"plutone",visible:false},[]),
        new Label({
            label:"lunabadula lunabadulalunabadula",
            enabled:false,
            style: {
                margin:{left:6,top:6},
                padding: {left: 50},
                minHeight: 350,
                height: 350,
            }
        },[]),
        new Label({
            style: {
                margin:{left:6,top:6}
            },
            label:"terra mondo tondo"
        },[]),
        
    ];
    //(l3 as any).setLabel("SALVE");
    const win = new Window({},[   
        new Container({style: {flexDirection: YogaFlexDirection.Row}},[l1,l2,l3]),
        new Container({style: {flexDirection: YogaFlexDirection.Row}},[
            new Label({style: {margin:{left:6,top:6}},label: "A1"},[]),
            new Label({style: {margin:{left:6,top:6}},label: "A2"},[]),
            new Label({style: {margin:{left:6,top:6}},label: "A3"},[])
        ]),
        new Container({style: {flexDirection: YogaFlexDirection.Row}},[
            new Label({style: {margin:{left:6,top:6}},label: "B1"},[]),
            new Label({style: {margin:{left:6,top:6}},label: "B2"},[]),
            new Label({style: {margin:{left:6,top:6}},label: "B3"},[])
        ]),
    ]);

    app.start();

    //(l1 as any).print();
})