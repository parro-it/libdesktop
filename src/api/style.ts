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
    
    /**
     * alignContent defines the distribution of lines along the cross-axis. This only has effect when items are 
     * wrapped to multiple lines using flex wrap.
     * 
     *  * FLEX START (DEFAULT) Align wrapped lines to the start of the container's cross axis.
     *  * FLEX END Align wrapped lines to the end of the container's cross axis.
     *  * STRETCH Stretch wrapped lines to match the height of the container's cross axis.
     *  * CENTER Align wrapped lines in the center of the container's cross axis.
     *  * SPACE BETWEEN Evenly space wrapped lines across the container's main axis, distributing remaining 
     *    space between the lines.
     *  * SPACE AROUND Evenly space wrapped lines across the container's main axis, distributing remaining 
     *    space around the lines. Compared to space between using space around will result in space being 
     *    distributed to the begining of the first lines and end of the last line.
     */
    alignContent: YogaAlign;

    /**
     * alignItems describes how to align children along the cross axis of their container. alignItems is very
     * similar to justifyContent but instead of applying to the main axis, alignItems applies to the cross axis.
     * 
     *  * STRETCH (DEFAULT) Stretch children of a container to match the height of the container's cross axis.
     *  * FLEX START Align children of a container to the start of the container's cross axis.
     *  * FLEX END Align children of a container to the end of the container's cross axis.
     *  * CENTER Align children of a container in the center of the container's cross axis.
     *  * BASELINE Align children of a container along a common baseline. Individual children can be set to be 
     *    the reference baseline for their parents.
     */
    alignItems: YogaAlign;

    /**
     * Align self has the same options and effect as align items but instead of affecting 
     * the children within a container,  you can apply this property to a single child to 
     * change its alignment within its parent. align self overrides any option set by the 
     * parent with align items.
     */
    alignSelf: YogaAlign;

    /**
     * The position type of an element defines how it is positioned within its parent.
     * 
     *   * RELATIVE (DEFAULT) By default an element is positioned relatively. This means an 
     *     element is positioned according to the normal flow of the layout, and then offset
     *     relative to that position based on the values of top, right, bottom, and left. The 
     *     offset does not affect the position of any sibling or parent elements.
     * 
     *   * ABSOLUTE When positioned absolutely an element doesn't take part in the normal layout 
     *     flow. It is instead laid out independent of its siblings. The position is determined based 
     *     on the top, right, bottom, and left values.
     * 
     * The position values top, right, bottom, and left behave differently depending on the position
     * type of the element. For a relative element they offset the position of the element in the 
     * direction specified. For absolute element though these properties specify the offset of the 
     * element's side from the same side on the parent.
     */
    positionType: YogaPositionType;



    /**
     * The  flex wrap property  is set on containers and controls  what happens when children 
     * overflow the size of the container along the main axis. By default children are forced 
     * into a single line (which can shrink elements).
     * If wrapping is allowed items are wrapped into multiple lines along the main axis if needed. 
     * wrap reverse behaves the same, but the order of the lines is reversed.
     * When wrapping lines align content can be used to specify  how the lines are  placed in 
     * the container.
     */
    flexWrap: YogaFlexWrap;

    display: YogaDisplay;

    overflow: YogaOverflow;

    flex: number;

    /**
     * FLEX GROW describes how any space within a container should be distributed among its children
     * along the main axis. After laying out its children, a container will distribute any remaining 
     * space according to the flex grow values specified by its children.
     * Flex grow accepts any floating point value >= 0, with 0 being the default value. A container 
     * will distribute any remaining space among its children weighted by the child’s flex grow value.
     */
    flexGrow: number;

    /**
     * FLEX SHRINK describes how to shrink children along the main axis in the case that the total 
     * size of the children overflow the size of the container on the main axis. flex shrink is very 
     * similar to flex grow and can be thought of in the same way if any overflowing size is considered 
     * to be negative remaining space. These two properties also work well together by allowing children 
     * to grow and shrink as needed.
     * flex shrink accepts any floating point value >= 0, with 1 being the default value. A container will 
     * shrink its children weighted by the child’s flex shrink value.
     */
    flexShrink: number;

    /*
        FLEX BASIS is an axis-independent way of providing the default size of an item along the main axis. 
        Setting the flex basis of a child is similar to setting the width of that child if its parent is a 
        container with flex direction: row or setting the height of a child if its parent is a container with 
        flex direction: column. The flex basis of an item is the default size of that item, the size of the 
        item before any flex grow and flex shrink calculations are performed.
    */
    flexBasis: number
    
    /*
        Same as flexBasis, but in percentage.
    */
    flexBasisPercent: number
    
    /*
        Same as flexBasis, but set the value to Auto.
    */
    flexBasisAuto: boolean

    width: number
    widthPercent: number

    height: number
    heightPercent: number

    minWidth: number
    minWidthPercent: number

    minHeight: number
    minHeightPercent: number

    maxWidth: number
    maxWidthPercent: number

    maxHeight: number
    maxHeightPercent: number

    readonly position: EdgedProp


    readonly border: EdgedProp
    readonly padding: EdgedProp
    readonly paddingPercent: EdgedProp
    readonly margin: EdgedProp
    readonly marginPercent: EdgedProp
    readonly marginAuto: EdgedProp
   


}

export interface EdgedProp {
    left: number
    top: number
    right: number
    bottom: number
    start: number
    end: number
    horizontal: number
    vertical: number
    all: number
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

export enum YogaAlign {
    Auto = ALIGN_AUTO,
    Baseline = ALIGN_BASELINE,
    Center = ALIGN_CENTER,
    FlexEnd = ALIGN_FLEX_END,
    FlexStart = ALIGN_FLEX_START,
    SpaceAround = ALIGN_SPACE_AROUND,
    SpaceBetween = ALIGN_SPACE_BETWEEN,
    Stretch = ALIGN_STRETCH,
}

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

export enum YogaFlexWrap {
    NoWrap = WRAP_NO_WRAP,
    Wrap = WRAP_WRAP,
    WrapReverse = WRAP_WRAP_REVERSE,
}

export enum YogaEdge {
    Left = EDGE_LEFT,
    Top = EDGE_TOP,
    Right = EDGE_RIGHT,
    Bottom = EDGE_BOTTOM,
    Start = EDGE_START,
    End = EDGE_END,
    Horizontal = EDGE_HORIZONTAL,
    Vertical = EDGE_VERTICAL,
    All = EDGE_ALL,
}
export enum YogaDisplay {
    Flex = DISPLAY_FLEX,
    None = DISPLAY_NONE,
}

export enum YogaUnit {
    Auto = UNIT_AUTO,
    Percent = UNIT_PERCENT,
    Point = UNIT_POINT,
    Undefined = UNIT_UNDEFINED,
}
export enum YogaOverflow {
     Hidden=OVERFLOW_HIDDEN,
     Scroll=OVERFLOW_SCROLL,
     Visible=OVERFLOW_VISIBLE,
}

export enum YogaPositionType {
     PositionTypeAbsolute = POSITION_TYPE_ABSOLUTE,
     PositionTypeRelative = POSITION_TYPE_RELATIVE
}
/*
export type YogaExperimentalFeature = typeof EXPERIMENTAL_FEATURE_WEB_FLEX_BASIS;

export type YogaMeasureMode =
    | typeof MEASURE_MODE_COUNT
    | typeof MEASURE_MODE_UNDEFINED
    | typeof MEASURE_MODE_EXACTLY
    | typeof MEASURE_MODE_AT_MOST;
*/

