import test from 'tape-async'
import {App} from './index'

test('run on multiple platforms', async (t:any): Promise<void> => {
    t.equal(typeof App.create, "function")
    const app = App.create()
    t.equal(app.arch, process.platform)
    t.equal(app.ver, "0.0.1")
})

test('start stop', async (t:any): Promise<void> => {
    const app = App.create()
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