import test from 'tape-async'
import {Style,YogaDirection} from '../index'

test('Style object creation', async (t:any): Promise<void> => {
    t.equal(typeof Style, "function")
    const s = new Style({})
    t.equal(typeof s.direction, "number")
    t.equal(typeof (s as any).otherFiled, "undefined")
})


test('Properties can be set', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.direction, YogaDirection.Inherit)
    s.direction = YogaDirection.LTR
    t.equal(s.direction, YogaDirection.LTR)
})