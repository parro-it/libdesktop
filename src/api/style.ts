const {style} = require("../../build/Release/desktop.node")

/**
 * Style contains all the properties 
 * to customize a control layout behaviour.
 */
export interface Style {
	/**
	 *  Set the text direction for the control, LTR or RTL.
	 */
	direction: YogaDirection;
}


export const Style:{
	/**
	 * Create a new Style object.
	 *
	 * @param props - property of the label to create.
	 * @param children - ignored
	 */
	new(props: any): Style
} = style.Style;
  

/*
getAlignContent(): YogaAlign;
getAlignItems(): YogaAlign;
getAlignSelf(): YogaAlign;
getAspectRatio(): number;
getBorder(edge: YogaEdge): number;
getChild(index: number): YogaNode;
getChildCount(): number;
getComputedBorder(edge: YogaEdge): number;
getComputedBottom(): number;
getComputedHeight(): number;
getComputedLayout(): Layout;
getComputedLeft(): number;
getComputedMargin(edge: YogaEdge): number;
getComputedPadding(edge: YogaEdge): number;
getComputedRight(): number;
getComputedTop(): number;
getComputedWidth(): number;
getDisplay(): YogaDisplay;
getFlexBasis(): number;
getFlexDirection(): YogaFlexDirection;
getFlexGrow(): number;
getFlexShrink(): number;
getFlexWrap(): YogaFlexWrap;
getHeight(): Value;
getJustifyContent(): YogaJustifyContent;
getMargin(edge: YogaEdge): Value;
getMaxHeight(): Value;
getMaxWidth(): Value;
getMinHeight(): Value;
getMinWidth(): Value;
getOverflow(): YogaOverflow;
getPadding(edge: YogaEdge): Value;
getParent(): YogaNode | null;
getPosition(edge: YogaEdge): Value;
getPositionType(): YogaPositionType;
getWidth(): Value;
insertChild(child: YogaNode, index: number): void;
isDirty(): boolean;
markDirty(): void;
removeChild(child: YogaNode): void;
reset(): void;
setAlignContent(alignContent: YogaAlign): void;
setAlignItems(alignItems: YogaAlign): void;
setAlignSelf(alignSelf: YogaAlign): void;
setAspectRatio(aspectRatio: number): void;
setBorder(edge: YogaEdge, borderWidth: number): void;
setDisplay(display: YogaDisplay): void;
setFlex(flex: number): void;
setFlexBasis(flexBasis: number | string): void;
setFlexBasisPercent(flexBasis: number): void;
setFlexDirection(flexDirection: YogaFlexDirection): void;
setFlexGrow(flexGrow: number): void;
setFlexShrink(flexShrink: number): void;
setFlexWrap(flexWrap: YogaFlexWrap): void;
setHeight(height: number | string): void;
setHeightAuto(): void;
setHeightPercent(height: number): void;
setJustifyContent(justifyContent: YogaJustifyContent): void;
setMargin(edge: YogaEdge, margin: number | string): void;
setMarginAuto(edge: YogaEdge): void;
setMarginPercent(edge: YogaEdge, margin: number): void;
setMaxHeight(maxHeight: number | string): void;
setMaxHeightPercent(maxHeight: number): void;
setMaxWidth(maxWidth: number | string): void;
setMaxWidthPercent(maxWidth: number): void;
setMeasureFunc(measureFunc: (width: number, widthMeasureMode: YogaMeasureMode, height: number, heightMeasureMode: YogaMeasureMode) => { width?: number; height?: number } | null): void;
setMinHeight(minHeight: number | string): void;
setMinHeightPercent(minHeight: number): void;
setMinWidth(minWidth: number | string): void;
setMinWidthPercent(minWidth: number): void;
setOverflow(overflow: YogaOverflow): void;
setPadding(edge: YogaEdge, padding: number | string): void;
setPaddingPercent(edge: YogaEdge, padding: number): void;
setPosition(edge: YogaEdge, position: number | string): void;
setPositionPercent(edge: YogaEdge, position: number): void;
setPositionType(positionType: YogaPositionType): void;
setWidth(width: number | string): void;
setWidthAuto(): void;
setWidthPercent(width: number): void;
unsetMeasureFunc(): void;
*/
declare const ALIGN_AUTO: 0;
declare const ALIGN_COUNT: 8;
declare const ALIGN_FLEX_START: 1;
declare const ALIGN_CENTER: 2;
declare const ALIGN_FLEX_END: 3;
declare const ALIGN_STRETCH: 4;
declare const ALIGN_BASELINE: 5;
declare const ALIGN_SPACE_BETWEEN: 6;
declare const ALIGN_SPACE_AROUND: 7;
declare const DIMENSION_COUNT: 2;
declare const DIMENSION_WIDTH: 0;
declare const DIMENSION_HEIGHT: 1;
declare const DIRECTION_COUNT: 3;
declare const DIRECTION_INHERIT: 0;
declare const DIRECTION_LTR: 1;
declare const DIRECTION_RTL: 2;
declare const DISPLAY_COUNT: 2;
declare const DISPLAY_FLEX: 0;
declare const DISPLAY_NONE: 1;
declare const EDGE_COUNT: 9;
declare const EDGE_LEFT: 0;
declare const EDGE_TOP: 1;
declare const EDGE_RIGHT: 2;
declare const EDGE_BOTTOM: 3;
declare const EDGE_START: 4;
declare const EDGE_END: 5;
declare const EDGE_HORIZONTAL: 6;
declare const EDGE_VERTICAL: 7;
declare const EDGE_ALL: 8;
declare const EXPERIMENTAL_FEATURE_COUNT: 1;
declare const EXPERIMENTAL_FEATURE_WEB_FLEX_BASIS: 0;
declare const FLEX_DIRECTION_COUNT: 4;
declare const FLEX_DIRECTION_COLUMN: 0;
declare const FLEX_DIRECTION_COLUMN_REVERSE: 1;
declare const FLEX_DIRECTION_ROW: 2;
declare const FLEX_DIRECTION_ROW_REVERSE: 3;
declare const JUSTIFY_COUNT: 6;
declare const JUSTIFY_FLEX_START: 0;
declare const JUSTIFY_CENTER: 1;
declare const JUSTIFY_FLEX_END: 2;
declare const JUSTIFY_SPACE_BETWEEN: 3;
declare const JUSTIFY_SPACE_AROUND: 4;
declare const JUSTIFY_SPACE_EVENLY: 5;
declare const LOG_LEVEL_COUNT: 6;
declare const LOG_LEVEL_ERROR: 0;
declare const LOG_LEVEL_WARN: 1;
declare const LOG_LEVEL_INFO: 2;
declare const LOG_LEVEL_DEBUG: 3;
declare const LOG_LEVEL_VERBOSE: 4;
declare const LOG_LEVEL_FATAL: 5;
declare const MEASURE_MODE_COUNT: 3;
declare const MEASURE_MODE_UNDEFINED: 0;
declare const MEASURE_MODE_EXACTLY: 1;
declare const MEASURE_MODE_AT_MOST: 2;
declare const NODE_TYPE_COUNT: 2;
declare const NODE_TYPE_DEFAULT: 0;
declare const NODE_TYPE_TEXT: 1;
declare const OVERFLOW_COUNT: 3;
declare const OVERFLOW_VISIBLE: 0;
declare const OVERFLOW_HIDDEN: 1;
declare const OVERFLOW_SCROLL: 2;
declare const POSITION_TYPE_COUNT: 2;
declare const POSITION_TYPE_RELATIVE: 0;
declare const POSITION_TYPE_ABSOLUTE: 1;
declare const PRINT_OPTIONS_COUNT: 3;
declare const PRINT_OPTIONS_LAYOUT: 1;
declare const PRINT_OPTIONS_STYLE: 2;
declare const PRINT_OPTIONS_CHILDREN: 4;
declare const UNIT_COUNT: 4;
declare const UNIT_UNDEFINED: 0;
declare const UNIT_POINT: 1;
declare const UNIT_PERCENT: 2;
declare const UNIT_AUTO: 3;
declare const WRAP_COUNT: 3;
declare const WRAP_NO_WRAP: 0;
declare const WRAP_WRAP: 1;
declare const WRAP_WRAP_REVERSE: 2;


export type YogaJustifyContent =
    | typeof JUSTIFY_CENTER
    | typeof JUSTIFY_FLEX_END
    | typeof JUSTIFY_FLEX_START
    | typeof JUSTIFY_SPACE_AROUND
    | typeof JUSTIFY_SPACE_BETWEEN
    | typeof JUSTIFY_SPACE_EVENLY;

export type YogaAlign =
    | typeof ALIGN_AUTO
    | typeof ALIGN_BASELINE
    | typeof ALIGN_CENTER
    | typeof ALIGN_FLEX_END
    | typeof ALIGN_FLEX_START
    | typeof ALIGN_SPACE_AROUND
    | typeof ALIGN_SPACE_BETWEEN
    | typeof ALIGN_STRETCH;

export type YogaFlexDirection =
    | typeof FLEX_DIRECTION_COLUMN
    | typeof FLEX_DIRECTION_COLUMN_REVERSE
    | typeof FLEX_DIRECTION_COUNT
    | typeof FLEX_DIRECTION_ROW
    | typeof FLEX_DIRECTION_ROW_REVERSE;

export enum YogaDirection{
    Inherit = DIRECTION_INHERIT,
    LTR = DIRECTION_LTR,
    RTL = DIRECTION_RTL,
}

export type YogaFlexWrap =
    | typeof WRAP_NO_WRAP
    | typeof WRAP_WRAP
    | typeof WRAP_WRAP_REVERSE;

export type YogaEdge =
    | typeof EDGE_LEFT
    | typeof EDGE_TOP
    | typeof EDGE_RIGHT
    | typeof EDGE_BOTTOM
    | typeof EDGE_START
    | typeof EDGE_END
    | typeof EDGE_HORIZONTAL
    | typeof EDGE_VERTICAL
    | typeof EDGE_ALL;

export type YogaDisplay =
    | typeof DISPLAY_FLEX
    | typeof DISPLAY_NONE;

export type YogaUnit =
    | typeof UNIT_AUTO
    | typeof UNIT_PERCENT
    | typeof UNIT_POINT
    | typeof UNIT_UNDEFINED;

export type YogaOverflow =
    | typeof OVERFLOW_HIDDEN
    | typeof OVERFLOW_SCROLL
    | typeof OVERFLOW_VISIBLE;

export type YogaPositionType =
    | typeof POSITION_TYPE_ABSOLUTE
    | typeof POSITION_TYPE_RELATIVE;

export type YogaExperimentalFeature = typeof EXPERIMENTAL_FEATURE_WEB_FLEX_BASIS;

export type YogaMeasureMode =
    | typeof MEASURE_MODE_COUNT
    | typeof MEASURE_MODE_UNDEFINED
    | typeof MEASURE_MODE_EXACTLY
    | typeof MEASURE_MODE_AT_MOST;


