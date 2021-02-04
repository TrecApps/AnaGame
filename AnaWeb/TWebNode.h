#pragma once
#include <TObject.h>
#include <DrawingBoard.h>
#include <TPromptControl.h>
#include <TStringSlice.h>
#include <TDataMap.h>
#include "AnaWeb.h"

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

/**
 * Class: TextData
 * Purpose: Holds data about the text provided, enabling child nodes to inject text into parent nodes if they are inline
 */
class ANA_WEB_DLL TextData
{
public:
    TextData();
    TextData(const TextData&);

    TString text;
    USHORT fontSize;
    DWRITE_FLOW_DIRECTION flowDirection;
    DWRITE_FONT_STRETCH fontStretch;
    DWRITE_FONT_STYLE fontStyle;
    DWRITE_FONT_WEIGHT fontWeight;
    DWRITE_LINE_SPACING_METHOD lineSpacing;
    D2D1_COLOR_F textColor, backgroundColor;
    bool hasBackgroundColor;
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
        TWebNodeContainer(TrecSubPointer<TControl, TTextField> text);
        TWebNodeContainer(TrecPointer<TControl> control);
        TWebNodeContainer(TrecPointer<TWebNode> webNode);

        D2D1_RECT_F GetLocation();

        TrecPointer<TControl> control;
        TrecPointer<TWebNode> webNode;
        NodeContainerType type;
        UINT initTextSize;                  // Text From this Node
        TDataArray<TextData> textDataList;
    };


public:
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
    UINT CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window);

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
     * Method: TWebNode::OnLoseFocus
     * Purpose: Alert the WebNode that it has lost focus
     * Parameters: void
     * Returns: the script to run (empty if no script is found)
     */
    TString OnLoseFocus();


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


    // Basic Core atributes of the node
    
    /**
     * Whether to display at all
     */
    bool doDisplay;

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
     * Basic string attributes
     */
    TString tagName, id, nodeClass, name;

    /**
     * The attributes gained
     */
    TDataMap<TString> attributes;

    /**
     * Hold the Drawing board being held
     */
    TrecPointer<DrawingBoard> board;

    /**
     * Self reference
     */
    TrecPointerSoft<TWebNode> self, parent;

    TDataArray<FormattingDetails> formattingDetails;

    TextData thisTextData;

    HWND win;

    /// Table resources

    /**
     * Column and Row Data
     */
    TDataArray<UINT> columnSizes, rowSizes;

    /**
     * Column and Row Spans
     */
    UCHAR columnSpan, rowSpan;
};

