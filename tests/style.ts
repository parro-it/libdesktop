import test from 'tape-async'
import {Style,YogaDirection,YogaFlexDirection,YogaJustifyContent,YogaAlign} from '../index'
import { EdgedProp, YogaDisplay, YogaFlexWrap, YogaOverflow, YogaPositionType } from '../src/api/style'

test('Style object creation', async (t:any): Promise<void> => {
    t.equal(typeof Style, "function")
    const s = new Style({},null)
    t.equal(typeof (s as any).otherFiled, "undefined")
})

test('direction', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.direction, YogaDirection.Inherit)
    s.direction = YogaDirection.LTR
    t.equal(s.direction, YogaDirection.LTR)
    t.throws(()=>{(s as any).direction=""},Error,/A number was expected/)
})

test('flexDirection', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.flexDirection, YogaFlexDirection.Column)
    s.flexDirection = YogaFlexDirection.Row
    t.equal(s.flexDirection, YogaFlexDirection.Row)
    t.throws(()=>{(s as any).flexDirection=""},Error,/A number was expected/)
    
})

test('justifyContent', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.justifyContent, YogaJustifyContent.FlexStart)
    s.justifyContent = YogaJustifyContent.SpaceAround
    t.equal(s.justifyContent, YogaJustifyContent.SpaceAround)
    t.throws(()=>{(s as any).justifyContent=""},Error,/A number was expected/)
})

test('alignContent', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.alignContent, YogaAlign.FlexStart)
    s.alignContent = YogaAlign.Stretch
    t.equal(s.alignContent, YogaAlign.Stretch)
    t.throws(()=>{(s as any).alignContent=""},Error,/A number was expected/)
})

test('alignItems', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.alignItems, YogaAlign.Stretch)
    s.alignItems = YogaAlign.FlexStart
    t.equal(s.alignItems, YogaAlign.FlexStart)
    t.throws(()=>{(s as any).alignItems=""},Error,/A number was expected/)
})

test('alignSelf', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.alignSelf, YogaAlign.Auto)
    s.alignSelf = YogaAlign.FlexStart
    t.equal(s.alignSelf, YogaAlign.FlexStart)
    t.throws(()=>{(s as any).alignSelf=""},Error,/A number was expected/)
})

test('positionType', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.positionType, YogaPositionType.PositionTypeRelative)
    s.positionType = YogaPositionType.PositionTypeAbsolute
    t.equal(s.positionType, YogaPositionType.PositionTypeAbsolute)
    t.throws(()=>{(s as any).positionType=""},Error,/A number was expected/)
})

test('flexWrap', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.flexWrap, YogaFlexWrap.NoWrap)
    s.flexWrap = YogaFlexWrap.WrapReverse
    t.equal(s.flexWrap, YogaFlexWrap.WrapReverse)
    t.throws(()=>{(s as any).flexWrap=""},Error,/A number was expected/)
})


test('overflow', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.overflow, YogaOverflow.Visible)
    s.overflow = YogaOverflow.Scroll
    t.equal(s.overflow, YogaOverflow.Scroll)
    t.throws(()=>{(s as any).overflow=""},Error,/A number was expected/)
})


test('display', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.display, YogaDisplay.Flex)
    s.display = YogaDisplay.None
    t.equal(s.display, YogaDisplay.None)
    t.throws(()=>{(s as any).display=""},Error,/A number was expected/)
})

test('flex', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.true(isNaN(s.flex))
    s.flex = 42
    t.equal(s.flex, 42)
    t.throws(()=>{(s as any).flex=""},Error,/A number was expected/)
})

test('flexGrow', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.flexGrow, 0)
    s.flexGrow = 41
    t.equal(s.flexGrow, 41)
    t.throws(()=>{(s as any).flexGrow=""},Error,/A number was expected/)
})

test('flexShrink', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(s.flexShrink, 0)
    s.flexShrink = 41
    t.equal(s.flexShrink, 41)
    t.throws(()=>{(s as any).flexShrink=""},Error,/A number was expected/)
})

function checkEdge(t:any, prop: any, edge: string) {
    t.equal(typeof prop[edge], "number")
    t.true(isNaN(prop[edge]))
    
    prop[edge]=42
    t.equal(prop[edge], 42)
    
    t.throws(()=>{debugger;(prop as any)[edge]=""},/A number was expected/)
    t.equal(prop[edge], 42)

    prop[edge]=41
    t.equal(prop[edge], 41)
}

function checkEdgeAuto(t:any, prop: any, edge: string) {
    t.equal(typeof prop[edge], "boolean")
    t.false(prop[edge])
    
    prop[edge]=1
    t.equal(prop[edge], true)
    
    t.throws(()=>{debugger;(prop as any)[edge]=""},/A number was expected/)
    t.equal(prop[edge], true)

    prop[edge]=0
    t.equal(prop[edge], true)
    

}

test('position', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.position, "object")
    t.throws(()=>{(s as any).position=""},/Cannot assign to read only property \'position\' of object \'#<Style>\'/)

    checkEdge(t,s.position,"all");
    checkEdge(t,s.position,"left");
    checkEdge(t,s.position,"top");
    checkEdge(t,s.position,"right");
    checkEdge(t,s.position,"bottom");
    checkEdge(t,s.position,"start");
    checkEdge(t,s.position,"end");
    checkEdge(t,s.position,"horizontal");
    checkEdge(t,s.position,"vertical");
})


test('border', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.border, "object")
    t.throws(()=>{(s as any).border=""},/Cannot assign to read only property \'border\' of object \'#<Style>\'/)

    checkEdge(t,s.border,"all");
    checkEdge(t,s.border,"left");
    checkEdge(t,s.border,"top");
    checkEdge(t,s.border,"right");
    checkEdge(t,s.border,"bottom");
    checkEdge(t,s.border,"start");
    checkEdge(t,s.border,"end");
    checkEdge(t,s.border,"horizontal");
    checkEdge(t,s.border,"vertical");
})

test('padding', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.padding, "object")
    t.throws(()=>{(s as any).padding=""},/Cannot assign to read only property \'padding\' of object \'#<Style>\'/)

    checkEdge(t,s.padding,"all");
    checkEdge(t,s.padding,"left");
    checkEdge(t,s.padding,"top");
    checkEdge(t,s.padding,"right");
    checkEdge(t,s.padding,"bottom");
    checkEdge(t,s.padding,"start");
    checkEdge(t,s.padding,"end");
    checkEdge(t,s.padding,"horizontal");
    checkEdge(t,s.padding,"vertical");
})

test('paddingPercent', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.paddingPercent, "object")
    t.throws(()=>{(s as any).paddingPercent=""},/Cannot assign to read only property \'paddingPercent\' of object \'#<Style>\'/)

    checkEdge(t,s.paddingPercent,"all");
    checkEdge(t,s.paddingPercent,"left");
    checkEdge(t,s.paddingPercent,"top");
    checkEdge(t,s.paddingPercent,"right");
    checkEdge(t,s.paddingPercent,"bottom");
    checkEdge(t,s.paddingPercent,"start");
    checkEdge(t,s.paddingPercent,"end");
    checkEdge(t,s.paddingPercent,"horizontal");
    checkEdge(t,s.paddingPercent,"vertical");
})

test('margin', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.margin, "object")
    t.throws(()=>{(s as any).margin=""},/Cannot assign to read only property \'margin\' of object \'#<Style>\'/)

    checkEdge(t,s.margin,"all");
    checkEdge(t,s.margin,"left");
    checkEdge(t,s.margin,"top");
    checkEdge(t,s.margin,"right");
    checkEdge(t,s.margin,"bottom");
    checkEdge(t,s.margin,"start");
    checkEdge(t,s.margin,"end");
    checkEdge(t,s.margin,"horizontal");
    checkEdge(t,s.margin,"vertical");
})
test('marginPercent', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.marginPercent, "object")
    t.throws(()=>{(s as any).marginPercent=""},/Cannot assign to read only property \'marginPercent\' of object \'#<Style>\'/)

    checkEdge(t,s.marginPercent,"all");
    checkEdge(t,s.marginPercent,"left");
    checkEdge(t,s.marginPercent,"top");
    checkEdge(t,s.marginPercent,"right");
    checkEdge(t,s.marginPercent,"bottom");
    checkEdge(t,s.marginPercent,"start");
    checkEdge(t,s.marginPercent,"end");
    checkEdge(t,s.marginPercent,"horizontal");
    checkEdge(t,s.marginPercent,"vertical");
})

test('marginAuto', async (t:any): Promise<void> => {
    const s = new Style({},null)
    t.equal(typeof s.marginAuto, "object")
    t.throws(()=>{(s as any).marginAuto=""},/Cannot assign to read only property \'marginAuto\' of object \'#<Style>\'/)

    checkEdgeAuto(t,s.marginAuto,"all");
    checkEdgeAuto(t,s.marginAuto,"left");
    checkEdgeAuto(t,s.marginAuto,"top");
    checkEdgeAuto(t,s.marginAuto,"right");
    checkEdgeAuto(t,s.marginAuto,"bottom");
    checkEdgeAuto(t,s.marginAuto,"start");
    checkEdgeAuto(t,s.marginAuto,"end");
    checkEdgeAuto(t,s.marginAuto,"horizontal");
    checkEdgeAuto(t,s.marginAuto,"vertical");
})
