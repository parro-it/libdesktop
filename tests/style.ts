import test from 'tape-async'
import {Style,YogaDirection,YogaFlexDirection,YogaJustifyContent} from '../index'

test('Style object creation', async (t:any): Promise<void> => {
    t.equal(typeof Style, "function")
    const s = new Style({})
    t.equal(typeof (s as any).otherFiled, "undefined")
})

test('direction', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.direction, YogaDirection.Inherit)
    s.direction = YogaDirection.LTR
    t.equal(s.direction, YogaDirection.LTR)
    t.throws(()=>{(s as any).direction=""},TypeError,/Argument value: A number was expected/)
    
})
test('flexDirection', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.flexDirection, YogaFlexDirection.Column)
    s.flexDirection = YogaFlexDirection.Row
    t.equal(s.flexDirection, YogaFlexDirection.Row)
    t.throws(()=>{(s as any).flexDirection=""},TypeError,/Argument value: A number was expected/)
    
})

test('justifyContent', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.justifyContent, YogaJustifyContent.FlexStart)
    s.justifyContent = YogaJustifyContent.SpaceAround
    t.equal(s.justifyContent, YogaJustifyContent.SpaceAround)
    t.throws(()=>{(s as any).justifyContent=""},TypeError,/Argument value: A number was expected/)
    
})