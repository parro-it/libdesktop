import test from 'tape-async'
import {Style,YogaDirection,YogaFlexDirection,YogaJustifyContent,YogaAlign} from '../index'
import { YogaFlexWrap, YogaPositionType } from '../src/api/style'

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

test('alignContent', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.alignContent, YogaAlign.FlexStart)
    s.alignContent = YogaAlign.Stretch
    t.equal(s.alignContent, YogaAlign.Stretch)
    t.throws(()=>{(s as any).alignContent=""},TypeError,/Argument value: A number was expected/)
})

test('alignItems', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.alignItems, YogaAlign.Stretch)
    s.alignItems = YogaAlign.FlexStart
    t.equal(s.alignItems, YogaAlign.FlexStart)
    t.throws(()=>{(s as any).alignItems=""},TypeError,/Argument value: A number was expected/)
})

test('alignSelf', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.alignSelf, YogaAlign.Auto)
    s.alignSelf = YogaAlign.FlexStart
    t.equal(s.alignSelf, YogaAlign.FlexStart)
    t.throws(()=>{(s as any).alignSelf=""},TypeError,/Argument value: A number was expected/)
})

test('positionType', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.positionType, YogaPositionType.PositionTypeRelative)
    s.positionType = YogaPositionType.PositionTypeAbsolute
    t.equal(s.positionType, YogaPositionType.PositionTypeAbsolute)
    t.throws(()=>{(s as any).positionType=""},TypeError,/Argument value: A number was expected/)
})

test('flexWrap', async (t:any): Promise<void> => {
    const s = new Style({})
    t.equal(s.flexWrap, YogaFlexWrap.NoWrap)
    s.flexWrap = YogaFlexWrap.WrapReverse
    t.equal(s.flexWrap, YogaFlexWrap.WrapReverse)
    t.throws(()=>{(s as any).flexWrap=""},TypeError,/Argument value: A number was expected/)
})