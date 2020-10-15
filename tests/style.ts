import test from 'tape-async'
import {Style} from '../index'

test('run on multiple platforms', async (t:any): Promise<void> => {
    t.equal(typeof Style, "function")
})