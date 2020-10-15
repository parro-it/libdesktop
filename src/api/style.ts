const {style} = require("../../build/Release/desktop.node")

/**
 * Style contains all the properties 
 * to customize a control layout behaviour.
 */
export interface Style {
    /**
     * Justify content describes how to align children within the main axis of their container. For 
     * example, you can use this property to center a child horizontally within a container with flex 
     * direction set to row or vertically within a container with flex direction set to column.
     * 
     *   * FLEX START (DEFAULT) Align children of a container to the start of the 
     *     container's main axis.
     *   * FLEX END Align children of a container to the end of the container's main axis.
     *   * CENTER Align children of a container in the center of the container's main axis.
     *   * SPACE BETWEEN Evenly space of children across the container's main axis, 
     *     distributing remaining space between the children.
     *   * SPACE AROUND Evenly space of children across the container's main axis, 
     *     distributing remaining space around the children. Compared to space between 
     *     using space around will result in space being distributed to the beginning of the 
     *     first child and end of the last child.
     *   * SPACE EVENLY Evenly distributed within the alignment container along the 
     *     main axis. The spacing between each pair of adjacent items, the main-start edge and 
     *     the first item, and the main-end edge and the last item, are all exactly the same.
     */
    justifyContent: YogaJustifyContent;

	/**
	 *  Set the text direction for the control, LTR or RTL.
     * 
     *  Property direction specifies the direction in which children and text in 
     *  a hierarchy should be laid out. Layout direction also effects what edge 
     *  start and end refer to. By default Yoga lays out with LTR layout direction. 
     *  In this mode start refers to left and end refers to right. When localizing your 
     *  apps for markets with RTL languages you should customize this by either by passing
     *  a direction to the CalculateLayout call or by setting the direction on the root node.
     * 
     *    * LTR (DEFAULT) Text and children and laid out from left to right. Margin and padding applied the start of an element are applied on the left side.
     *    * RTL Text and children and laid out from right to left. Margin and padding applied the start of an element are applied on the right side.
	 */
    direction: YogaDirection;
    
    /**
	 * Property flexDirection controls the direction in which children of a node are laid out. 
     * This is also referred to as the main axis. The main axis is the direction in which 
     * children are laid out. The cross axis the the axis perpendicular to the main axis, 
     * or the axis which wrapping lines are laid out in.
     * 
     *   * ROW (DEFAULT) Align children from left to right. If wrapping is enabled then the 
     *     next line will start under the first item on the left of the container.
     *   * COLUMN Align children from top to bottom. If wrapping is enabled then the next 
     *     line will start to the left first item on the top of the container.
     *   * ROW REVERSE Align children from right to left. If wrapping is enabled then the 
     *     ext line will start under the first item on the right of the container.
     *   * COLUMN REVERSE Align children from bottom to top. If wrapping is enabled then the 
     *     next line will start to the left first item on the bottom of the container.
	 */
	flexDirection: YogaFlexDirection;
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
const ALIGN_AUTO = 0;
const ALIGN_COUNT = 8;
const ALIGN_FLEX_START = 1;
const ALIGN_CENTER = 2;
const ALIGN_FLEX_END = 3;
const ALIGN_STRETCH = 4;
const ALIGN_BASELINE = 5;
const ALIGN_SPACE_BETWEEN = 6;
const ALIGN_SPACE_AROUND = 7;
const DIMENSION_COUNT = 2;
const DIMENSION_WIDTH = 0;
const DIMENSION_HEIGHT = 1;
const DIRECTION_COUNT = 3;
const DIRECTION_INHERIT = 0;
const DIRECTION_LTR = 1;
const DIRECTION_RTL = 2;
const DISPLAY_COUNT = 2;
const DISPLAY_FLEX = 0;
const DISPLAY_NONE = 1;
const EDGE_COUNT = 9;
const EDGE_LEFT = 0;
const EDGE_TOP = 1;
const EDGE_RIGHT = 2;
const EDGE_BOTTOM = 3;
const EDGE_START = 4;
const EDGE_END = 5;
const EDGE_HORIZONTAL = 6;
const EDGE_VERTICAL = 7;
const EDGE_ALL = 8;
const EXPERIMENTAL_FEATURE_COUNT = 1;
const EXPERIMENTAL_FEATURE_WEB_FLEX_BASIS = 0;
const FLEX_DIRECTION_COUNT = 4;
const FLEX_DIRECTION_COLUMN = 0;
const FLEX_DIRECTION_COLUMN_REVERSE = 1;
const FLEX_DIRECTION_ROW = 2;
const FLEX_DIRECTION_ROW_REVERSE = 3;
const JUSTIFY_COUNT = 6;
const JUSTIFY_FLEX_START = 0;
const JUSTIFY_CENTER = 1;
const JUSTIFY_FLEX_END = 2;
const JUSTIFY_SPACE_BETWEEN = 3;
const JUSTIFY_SPACE_AROUND = 4;
const JUSTIFY_SPACE_EVENLY = 5;
const LOG_LEVEL_COUNT = 6;
const LOG_LEVEL_ERROR = 0;
const LOG_LEVEL_WARN = 1;
const LOG_LEVEL_INFO = 2;
const LOG_LEVEL_DEBUG = 3;
const LOG_LEVEL_VERBOSE = 4;
const LOG_LEVEL_FATAL = 5;
const MEASURE_MODE_COUNT = 3;
const MEASURE_MODE_UNDEFINED = 0;
const MEASURE_MODE_EXACTLY = 1;
const MEASURE_MODE_AT_MOST = 2;
const NODE_TYPE_COUNT = 2;
const NODE_TYPE_DEFAULT = 0;
const NODE_TYPE_TEXT = 1;
const OVERFLOW_COUNT = 3;
const OVERFLOW_VISIBLE = 0;
const OVERFLOW_HIDDEN = 1;
const OVERFLOW_SCROLL = 2;
const POSITION_TYPE_COUNT = 2;
const POSITION_TYPE_RELATIVE = 0;
const POSITION_TYPE_ABSOLUTE = 1;
const PRINT_OPTIONS_COUNT = 3;
const PRINT_OPTIONS_LAYOUT = 1;
const PRINT_OPTIONS_STYLE = 2;
const PRINT_OPTIONS_CHILDREN = 4;
const UNIT_COUNT = 4;
const UNIT_UNDEFINED = 0;
const UNIT_POINT = 1;
const UNIT_PERCENT = 2;
const UNIT_AUTO = 3;
const WRAP_COUNT = 3;
const WRAP_NO_WRAP = 0;
const WRAP_WRAP = 1;
const WRAP_WRAP_REVERSE = 2;


export enum YogaJustifyContent {
    Center = JUSTIFY_CENTER,
    FlexEnd = JUSTIFY_FLEX_END,
    FlexStart = JUSTIFY_FLEX_START,
    SpaceAround = JUSTIFY_SPACE_AROUND,
    SpaceBetween = JUSTIFY_SPACE_BETWEEN,
    SpaceEvenly = JUSTIFY_SPACE_EVENLY,
}

export type YogaAlign =
    | typeof ALIGN_AUTO
    | typeof ALIGN_BASELINE
    | typeof ALIGN_CENTER
    | typeof ALIGN_FLEX_END
    | typeof ALIGN_FLEX_START
    | typeof ALIGN_SPACE_AROUND
    | typeof ALIGN_SPACE_BETWEEN
    | typeof ALIGN_STRETCH;

export enum YogaFlexDirection {
    Column = FLEX_DIRECTION_COLUMN,
    ColumnReverse = FLEX_DIRECTION_COLUMN_REVERSE,
    Count = FLEX_DIRECTION_COUNT,
    Row = FLEX_DIRECTION_ROW,
    RowReverse = FLEX_DIRECTION_ROW_REVERSE,
}

export enum YogaDirection {
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


