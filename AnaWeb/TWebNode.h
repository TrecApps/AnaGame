#pragma once
#include <TObject.h>
#include <DrawingBoard.h>
#include <TStringSlice.h>
#include <TDataMap.h>
#include "AnaWeb.h"
#include <TPage.h>
#include <TTextInput.h>

/**
 * Enum Class: WebSizeUnit
 * Purpose: Represents a unit a size value is given in
 */
typedef enum class WebSizeUnit
{
    wsu_px, // pixel
    wsu_in, // inch
    wsu_pt, // points
    wsu_cm, // centimeter
    wsu_mm, // millimeter
    wsu_em, // element relativity
    wsu_bl  // Blank
}WebSizeUnit;

/**
 * Enum Class: WebNodeDisplayOutside
 * Purpose: Easy means of managing how the Node is supposed to appear
 */
typedef enum class WebNodeDisplayOutside
{
    wndo_inline,
    wndo_block
}WebNodeDisplay;

/**
 * Enum Class: WebNodeDisplayInside
 * Purpose:
 */
typedef enum class WebNodeDisplayInside
{
    wndi_flow,
    wndi_flow_root,
    wndi_table,
    wndi_flex,
    wndi_grid,
    wndi_ruby
}WebNodeDisplayInside;

typedef enum class WebNodeDisplayInternal
{
    wndi_not_set,
    wndi_row_group,
    wndi_header_group,
    wndi_footer_group,
    wndi_row,
    wndi_cell,
    wndi_column_group,
    wndi_column,
    wndi_caption
};


/**
 * Class: EventPropagater
 * Purpose: Helps determine if a web node should report it's event first or let any child nodes report
 * 
 * Note: Information related to event propagation could be found here:
 *   https://stackoverflow.com/questions/4616694/what-is-event-bubbling-and-capturing#:~:text=Event%20bubbling%20and%20capturing%20are%20two%20ways%20of,in%20which%20order%20the%20elements%20receive%20the%20event.
 */
class ANA_WEB_DLL EventPropagater {
public:
    /**
     * Method: EventPropagater::EventPropagater
     * Purpose: Default Constructor
     * Parameters: void
     * Returns: new Event Propagater obj
     */
    EventPropagater();

    /**
     * Method: EventPropagater::EventPropagater
     * Purpose: Copy Constructor
     * Parameters: const EventPropagater& orig - the object to copy from
     * Returns: new Event Propagater obj
     */
    EventPropagater(const EventPropagater& orig);

    /**
     * Method: EventPropagater::EventPropagater
     * Purpose: Copy Constructor
     * Parameters: const TString& e - the string to check for
     *              bool useCapture - true, then use capture, false, use bubbling
     * Returns: new Event Propagater obj
     */
    EventPropagater(const TString& e, bool useCapture);

    /**
     * The name of the event
     */
    TString event;

    /**
     * the propagation mode
     */
    bool useCapture;
};



typedef enum class border_side
{
    bs_all,
    bs_top,
    bs_bottom,
    bs_left,
    bs_right
};

typedef enum class list_style
{
    ls_disc,
    ls_circle,
    ls_none,
    ls_square,

    ls_armenian,
    ls_cjk,
    ls_decimal,
    ls_decimal_0,
    ls_georgian,
    ls_hebrew,
    ls_hiragana,
    ls_hiragana_iroha,
    ls_katakana,
    ls_katakana_iroha,
    ls_lower_alph,
    ls_lower_greek,
    ls_lower_latin,
    ls_lower_roman,
    ls_upper_alpha,
    ls_upper_greek,
    ls_upper_latin,
    ls_upper_roman
};

/**
 * Class: TextData
 * Purpose: Holds data about the text provided, enabling child nodes to inject text into parent nodes if they are inline
 */
class ANA_WEB_DLL TextData
{
public:
    TextData();
    TextData(const TextData&);

    void Update(const TextData& copy);

    TString text;
    USHORT fontSize;
    DWRITE_FLOW_DIRECTION flowDirection;
    DWRITE_FONT_STRETCH fontStretch;
    DWRITE_FONT_STYLE fontStyle;
    DWRITE_FONT_WEIGHT fontWeight;
    DWRITE_LINE_SPACING_METHOD lineSpacing;
    TColor textColor, backgroundColor;
    bool hasBackgroundColor;

    bool fontSizeUpdated, flowDirectionUpdated, fontStretchUpdated, 
        fontStyleUpdated, fontWeightUpdated, lineSpacingUpdated, textColorUpdated;
};

class ANA_WEB_DLL TColorSet
{
public:
    TColorSet();
    TColorSet(const TColorSet& copy);
    TColorSet operator=(const TColorSet& color);
    TColorSet operator=(const TColor& color);

    TColor color;
    bool set;
};

typedef struct BorderCorners
{
    USHORT v, h;
}BorderCorners;

class ANA_WEB_DLL BorderData
{
public:
    BorderData();
    BorderData(const BorderData& copy);
    ~BorderData();

    void CompileAttributes(TString& atts, border_side side);

    void CompileBorder(TString& atts, border_side side);

    void CompileColor(TString& atts, border_side side);

    bool CompileStyle(TString& atts, border_side side);

    void CompileCorner(TString& atts, border_side side);

    USHORT GetBorderThickness(border_side side);

    bool GetBorderColor(TColor& color, border_side);

    stroke_style GetStrokeStyle(border_side side);

    stroke_style borderStyle, topStyle, bottomStyle, rightStyle, leftStyle;
    USHORT thick, topThick, bottomThick, rightThick, leftThick;
    TColor color;
    TColorSet topColor, bottomColor, rightColor, leftColor;
    TrecPointer<TBrush> brush;
    BorderCorners *topLeft, *topRight, *bottomLeft, *bottomRight;

private:
    void ClearCorner(border_side side);
};

/**
 * Class: TWebNode
 * Purpose: Represents HTML body Nodes, holds on to the Appropriate TControl (if necessary)
 * 
 * SuperClass: TObject
 */
class ANA_WEB_DLL TWebNode :
    public TObject
{
    friend class TWebNode;
protected:

    typedef enum class NodeContainerType
    {
        nct_text,
        nct_control,
        nct_web,
        ntc_null
    };

    class TWebNodeContainer : public TObject
    {
    public:
        TWebNodeContainer(TrecSubPointer<TPage, TTextInput> text);
        TWebNodeContainer(TrecPointer<TPage> control);
        TWebNodeContainer(TrecPointer<TWebNode> webNode);

        D2D1_RECT_F GetLocation();

        TrecPointer<TPage> control;
        TrecPointer<TWebNode> webNode;
        NodeContainerType type;
        UINT initTextSize;                  // Text From this Node
        TDataArray<TextData> textDataList;
    };


public:
    float ConvertMeasurement(TString& atts);

    /**
     * Method: TWebNode::TWebNode
     * Purpose: Constructor
     * Parameters: TrecPointer<DrawingBoard> board - the board to draw on
     * Returns: new Html Node
     */
    TWebNode(TrecPointer<DrawingBoard> board);

    /**
     * Method: TWebNode::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TStringSliceManager& html the string to process
     *              UINT start - the index to start at
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessHtml(TStringSliceManager& html, UINT& start, HWND win);

    /**
     * Method: TWebNode::ProcessInnerHtml
     * Purpose: Compiles the html the node has been given
     * Parameters: void
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win);

    /**
     * Method: TWebNode::SetInnerHtml
     * Purpose: Allows the Node to have it's contents changed dynamically
     * Parameters: const TString& html - the html to set the insides of this node to
     * Returns: UINT - error code (0 for success)
     */
    UINT SetInnerHtml(TStringSliceManager& html);

    /**
     * Method: TWebNode::GetElementsByName
     * Purpose: Retrieves a list of elements with the given name
     * Parameters: const TString& name - the name of the nodes to collect
     *              TDataArray<TrecPointer<TWebNode>>& nodes - the structure to hold the nodes in
     * Returns: void
     */
    void GetElementsByName(const TString& name, TDataArray<TrecPointer<TWebNode>>& nodes);

    /**
     * Method: TWebNode::GetElementsByTag
     * Purpose: Retrieves a list of elements with the given tag type
     * Parameters: const TString& tag - the tag type of the nodes to collect
     *              TDataArray<TrecPointer<TWebNode>>& nodes - the structure to hold the nodes in
     * Returns: void
     */
    void GetElementsByTag(const TString& tag, TDataArray<TrecPointer<TWebNode>>& nodes);

    /**
     * Method: TWebNode::GetElementsByClass
     * Purpose: Retrieves a list of elements with the given class type
     * Parameters: const TString& nodeClass - the class type of the nodes to collect
     *              TDataArray<TrecPointer<TWebNode>>& nodes - the structure to hold the nodes in
     * Returns: void
     */
    void GetElementsByClass(const TString& nodeClass, TDataArray<TrecPointer<TWebNode>>& nodes);

    /**
     * Method: TWebNode::GetElementById
     * Purpose: Retrieves the 
     * Parameters: const TString& id - the id to seek out
     * Returns: TrecPointer<TWebNode> - the first node with the given id, or noll if the node was not found
     */
    TrecPointer<TWebNode> GetElementById(const TString& id);

    /**
     * Method: TWebNode::SetSelf
     * Purpose: Supports the Self Trait of this class
     * Parameters: TrecPointer<TWebNode> s - the pointer to use to set self to
     * Returns: void
     */
    void SetSelf(TrecPointer<TWebNode> s);

    /**
     * Method: TWebNode::GetType
     * Purpose: Reports the Type of this object
     * Parameters: void
     * Returns: TString - the string representation of this objects type
     */
    virtual TString GetType() override;

    /**
     * Method: TWebNode::PreCreate
     * Purpose: Allows Node to predict ahead of time what space it will need and how it will be shown
     * Parameters: TrecPointerSoft<TWebNode> parent - the Web Node that called the method
     *              TrecPointer<TArray<styleTable>> styles - list of CSS styles that the node should adhere to
     * Returns: void
     */
    void PreCreate(TrecPointerSoft<TWebNode> parent, TrecPointer<TArray<styleTable>>& styles);

    /**
     * Method: TWebNode::SetDisplay
     * Purpose: Sets up the display attributes of the web node according to the string provided
     * Parameters: const TString& display - the string holding display info
     * Return: void
     */
    void SetDisplay(const TString& display);


    /**
     * Method: TWebNode::CreateWebNode
     * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
     * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
     *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
     *              HWND window - handle to the window the node is operating in
     *              TrecPointerSoft<TWebNode> parent - the node that called this method
     * Returns: UINT - Error Code
     */
    virtual UINT CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window);

    /**
     * Method: TWebNode::GetLocation
     * Purpose: Retrieves the current Location of the WebNode
     * Parameters: void
     * Returns: D2D1_RECT_F - the location of the node
     */
    D2D1_RECT_F GetLocation();

    /**
     * Method: TWebNode::OnLButtonDown
     * Purpose: Allows the Node to react to the Button down event
     * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
     *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
     *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
     *              const TPoint& point - the point in the window where the mouse was clicked
     * Returns: void
     */
    void OnLButtonDown(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, TDataArray<TrecPointer<TWebNode>>& nodeCollection, const TPoint& point);

    /**
     * Method: TWebNode::OnLButtonUp
     * Purpose: Allows the Node to react to the Button Up event
     * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
     *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
     *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
     *              TrecPointer<TWebNode>& focusNode - the node that has current user focus 
     *              const TPoint& point - the point in the window where the mouse was clicked
     * Returns: void
     */
    void OnLButtonUp(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, TDataArray<TrecPointer<TWebNode>>& nodeCollection, TrecPointer<TWebNode>& focusNode, const TPoint& point);

    /**
     * Method: TWebNode::OnLButtonDblClck
     * Purpose: Allows the Node to react to the Button down event
     * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
     *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
     *              const TPoint& point - the point in the window where the mouse was clicked
     * Returns: void
     */
    void OnLButtonDblClck(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, const TPoint& point);

    /**
     * Method: TWebNode::OnLButtonDown
     * Purpose: Allows the Node to react to the Button down event
     * Parameters: TDataArray<TString>& script - the script to append to if the node has a relevent event attribute and deems it necessary to append it
     *              TDataArray<TrecObjectPointer>& thisCollection - the collection of objects to use as 'this' when handler is run
     *              TDataArray<TrecPointer<TWebNode>>& nodeCollection - the collection of nodes that could have this handler apply
     *              const TPoint& point - the point in the window where the mouse was clicked
     * Returns: void
     */
    void OnMouseMove(TDataArray<TString>& script, TDataArray<TrecObjectPointer>& thisCollection, TDataArray<TrecPointer<TWebNode>>& nodeCollection, const TPoint& point);

    /**
     * Method: TWebNode::OnSubmit
     * Purpose: Allows the Node to handle OnSumit events
     * Parameters: void
     * Returns: void
     */
    virtual void OnSubmit();

    /** 
     * Method: TWebNode::OnLoseFocus
     * Purpose: Alert the WebNode that it has lost focus
     * Parameters: void
     * Returns: the script to run (empty if no script is found)
     */
    TString OnLoseFocus();

    /**
     * Method: TWebNode::OnDraw
     * Purpose: Draws the Node
     * Parameters: void
     * Returns: void
     */
    void OnDraw();

protected:
    /// Display Properties

    /**
     * Method: TWebNode::CompileProperties
     * Purpose: Takes CSS Attributes this node has and generates them into styling data
     * Parameters: const TrecPointer<TArray<styleTable>>& styles - list of CSS styles to look at
     * Returns: void
     */
    void CompileProperties(TrecPointer<TArray<styleTable>>& styles);

    /**
     * Method: TWebNode::CompileProperties_
     * Purpose: Finalize propertie form the generated lists (virtual so that special nodes can look for special attributes)
     * Parameters: const TrecPointer<TArray<styleTable>>& atts - list of CSS
     * Returns: void
     */
    virtual void CompileProperties(TDataMap<TString>& atts);

    /**
     * Method: TWebNode::IsText
     * Purpose: Reports whether this Node chiefly holds Text that can be inserted into parent text
     * Parameters: void
     * Returns: bool - whether the Node can be treated as text that can be inserted into parent text
     */
    bool IsText();

    /**
     * Method: TWebNode::RetrieveText
     * Purpose: Retrieves Text Data for the Parent Node, allowing the root Text node to compile text
     * Parameters: TDataArray<TextData>& textDataList - List of Data to create
     * Returns: void
     */
    void RetrieveText(TDataArray<TextData>& textDataList);

    /**
     * Method: TWebNode::CompileText
     * Purpose: Compiles Text Nodes according to the text Data it contains
     * Parameters: TrecPointer<TWebNode::TWebNodeContainer> textNode - the text node to compile
     *              D2D1_RECT_F loc - the space the text has been allocated
     * Returns: void
     */
    void CompileText(TrecPointer<TWebNode::TWebNodeContainer> textNode, D2D1_RECT_F loc);

    // Methods for Inter-Node Communication

    /**
     * Method: TWebNode::AddColumn
     * Purpose: Adds a Column to the table, if there is one
     * Parameters: UCHAR count - number of columns to add
     * Returns: void
     * 
     * Note: 
     */
    void AddColumn(UCHAR count = 1);

    /**
     * Method: TWebNode::AddRow
     * Purpose: Adds a Row to the table, if there is one
     * Parameters: void
     * Returns: void
     */
    void AddRow();

    /**
     * Method: TWebNode::SetColumnsAndRows
     * Purpose: Sets the attributes for the Columns and rows
     * Parameters: TDataArray<UINT>& cols - set of columns
     *              TDataArray<UINT>& rows - set of rows
     * Returns: void
     */
    void SetColumnsAndRows(TDataArray<UINT>& cols, TDataArray<UINT>& rows);

    /**
     * Method: TWebNode::PreEstablishTable
     * Purpose: Allows Table Elements to establish their elements and determine the dimensions they have
     * Parameters: void
     * Returns: void
     */
    void PreEstablishTable();

    /**
     * Method: TWebNode::ShrinkHeight
     * Purpose: Shrinks the Node to the minimum neccesary to display
     * Parameters: void
     * Returns: void
     */
    void ShrinkHeight();

    /**
     * Method: TWebNode::ShrinkWidth
     * Purpose: Shrinks the Node to the minimum neccessary width to display
     * Parameters: UINT minWidth - The absolute minimum width to allow the Node to shrink
     * Returns: void
     */
    void ShrinkWidth(UINT minWidth);
    /**
     * Method: TWebNode::ShrinkWidth
     * Purpose: Shrinks the Node to the minimum neccessary width to display
     * Parameters: UINT column - The Column in Question (if Node is not a row, this parameter is ignored
     * Returns: float - the width needed
     */
    float NeedsWidth(UINT column);

    /**
     * Method: TWebNode::GetListPrePend
     * Purpose: Called by list-items, provides the list item with the Prepend String
     * Parameters: void
     * Returns: TString - the current prepender
     */
    TString GetListPrepend();

    /**
     * Method: TWebNode::HandleRowSpan
     * Purpose: Called by a node identifying as a table to allow row nodes to update cells that extend the rowspan
     * Parameters: void
     * Returns: void
     */
    void HandleRowSpan();

    /**
     * Method: TWebNode::HandleColGroup
     * Purpose: Called by table to handle attributes provided by colgroups and col nodes
     * Parameters: TDataArray<TDataMap<String>>& groups - collection of attribute to add
     * Returns: void
     */
    void HandleColGroup(TDataArray<TDataMap<TString>>& groups);

    /**
     * Method: TWebNode::ProbeColAtts
     * Purpose: Called By table to probe for column attributes 
     * Parameters: TDataArray<TDataMap<String>>& groups - collection of attribute to add
     * Returns: void
     */
    void ProbeColAtts(TDataArray<TDataMap<TString >> &groups);

    // Other Helper Methods

    /**
     * Method: TWebNode::DrawBorder
     * Purpose: Holds the logic for actually drawing a border around the content
     * Parameters: void
     * Returns: void
     */
    void DrawBorder();

    /**
     * Method: TWebNode::CompileMargin
     * Purpose: Converts String data into a margin
     * Parameters: const TSTring& marginString
     *              border_side side - the border side to focus on
     *              bool inner - inner border
     * Returns: void
     */
    void CompileMargin(const TString& marginString, border_side side, bool inner);

    // Basic Core atributes of the node
    
    /**
     * Whether to display at all
     */
    bool doDisplay, shouldDraw;

    /**
     * The Display outside Property
     */
    WebNodeDisplayOutside outsideDisplay;
    /**
     * The Display Inside Property
     */
    WebNodeDisplayInside insideDisplay;
    /**
     * Whether it is a List item
     */
    bool isListItem;
    /**
     * The internal dispay to use (applies to table and (eventually) ruby)
     */
    WebNodeDisplayInternal internalDisplay;


    /**
     * Represents the handlers that are meant to Run 
     */
    TDataMap<EventPropagater> handlers;

    /**
     * represents the Element Node's innerHTML attribute
     */
    TrecPointer<TStringSlice> innerHtml;

    /**
     * Holds on to all child Elements underneath it
     */
    TDataArray<TrecPointer<TWebNodeContainer>> childNodes;

    /**
     * Represents the actual control, if need be
     */
    TrecPointer<TControl> control;

    /**
     * Location of the Web Node
     */
    D2D1_RECT_F location;

    /**
     * Margins of the App (inner between text/content and border, outer between border and boundaries)
     */
    D2D1_RECT_F innerMargin, outerMargin;

    /**
     * Basic string attributes
     */
    TString tagName, id, nodeClass, name, value;

    /**
     * The attributes gained
     */
    TDataMap<TString> attributes, exAttributes;

    /**
     * Hold the Drawing board being held
     */
    TrecPointer<DrawingBoard> board;

    /**
     * Self reference
     */
    TrecPointerSoft<TWebNode> self, parent;

    TDataArray<TextFormattingDetails> formattingDetails;

    TextData thisTextData;

    BorderData borderData;

    /**
     * For list objects, what to send to the next list item that asks, for List items, what to prepend to the text
     */
    TString listInfo;

    /**
     * whether to send the list forward, or backwards
     */
    bool listForward;

    /**
     * list Style Mode
     */
    list_style listStyle;

    HWND win;

    TrecPointer<TWindowEngine> d3dEngine;

    /// Table resources

    /**
     * Column and Row Data
     */
    TDataArray<UINT> columnSizes, rowSizes;

    /**
     * Column and Row Spans
     */
    UCHAR columnSpan, rowSpan;

    /**
     * Whether the node should attempt to shrink height
     */
    bool doShrink;
};

