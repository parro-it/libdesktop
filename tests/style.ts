import test from 'tape-async'
import {Style} from '../index'

test('Style object creation', async (t:any): Promise<void> => {
    t.equal(typeof Style, "function")
    const s = new Style({})
    t.equal(typeof s.direction, "number")
    t.equal(typeof (s as any).otherFiled, "undefined")
})