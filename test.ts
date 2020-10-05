import test from 'tape-async'
import {App} from './src/app'

test('run on multiple platforms', async (t:any): Promise<void> => {
    t.equal(typeof App.create, "function")
    const app = App.create()
    t.equal(app.arch, process.platform)
    t.equal(app.common, "createApp")
})