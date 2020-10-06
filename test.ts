import 'module-alias/register'

import test from 'tape-async'
import {App} from './index'

test('run on multiple platforms', async (t:any): Promise<void> => {
    t.equal(typeof App.create, "function")
    const app = App.create()
    t.equal(app.arch, process.platform)
    t.equal(app.ver, "0.0.1")
})