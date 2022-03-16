#pragma once
#include <TPage.h>
#include "AnaWeb.h"
#include <TStringSlice.h>
#include <TDataMap.h>
#include <TComplexTextElement.h>
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
typedef enum class TcWebNodeDisplayOutside
{
    wndo_inline,
    wndo_block
}TcWebNodeDisplay;

/**
 * Enum Class: WebNodeDisplayInside
 * Purpose:
 */
typedef enum class TcWebNodeDisplayInside
{
    wndi_flow,
    wndi_flow_root,
    wndi_table,
    wndi_flex,
    wndi_grid,
    wndi_ruby
}TcWebNodeDisplayInside;

typedef enum class TcWebNodeDisplayInternal
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
}TcWebNodeDisplayInternal;

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

typedef enum class tc_border_side
{
    bs_all,
    bs_top,
    bs_bottom,
    bs_left,
    bs_right
}tc_border_side;

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
class ANA_WEB_DLL TcTextData
{
public:
    TcTextData();
    TcTextData(const TcTextData&);

    void Update(const TcTextData& copy);

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

typedef struct BorderCorners
{
    USHORT v, h;
}BorderCorners;

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

class ANA_WEB_DLL TcBorderData
{
public:
    TcBorderData();
    TcBorderData(const TcBorderData& copy);
    ~TcBorderData();

    void CompileAttributes(TString& atts, tc_border_side side);

    void CompileBorder(TString& atts, tc_border_side side);

    void CompileColor(TString& atts, tc_border_side side);

    bool CompileStyle(TString& atts, tc_border_side side);

    void CompileCorner(TString& atts, tc_border_side side);

    USHORT GetBorderThickness(tc_border_side side);

    bool GetBorderColor(TColor& color, tc_border_side);

    stroke_style GetStrokeStyle(tc_border_side side);

    stroke_style borderStyle, topStyle, bottomStyle, rightStyle, leftStyle;
    USHORT thick, topThick, bottomThick, rightThick, leftThick;
    TColor color;
    TColorSet topColor, bottomColor, rightColor, leftColor;
    TrecPointer<TBrush> brush;
    BorderCorners* topLeft, * topRight, * bottomLeft, * bottomRight;

private:
    void ClearCorner(tc_border_side side);
};

class ANA_WEB_DLL TcWebNode :
    public TPage
{
protected:
    typedef enum class NodeContainerType
    {
        nct_raw_text,   // The Inner HTML was text
        nct_reg_node,   // The Inner HTML was a Regular blocked Node
        nct_tex_node,   // The Inner HTML was an inline (texted node)
        ntc_null
    };

    class TcNodeElement : public TVObject
    {
    public:
        virtual NodeContainerType GetElementType() = 0;

        virtual D2D1_RECT_F GetLocation() = 0;
    };

    class TcTextNodeElement : public TcNodeElement
    {
    protected:
        bool isFirst;
        TrecPointer<TTextElement> textElement;
    public :
        TcTextNodeElement(bool isFirst = true);

        virtual NodeContainerType GetElementType() override;
        virtual D2D1_RECT_F GetLocation() override;
        bool IsFirst();

        /**
         * Method: TVObject::GetVariable
         * Purpose: Revtrieves a value in Variable form
         * Parameters: const TString& name - the name of the variable
         *              TrecPointer<TVariable>& var - holds the variable retrieved if valid
         * Returns: bool - whether the variable was valid or not
         *
         * Attributes: abstract
         */
        virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) override;

        /**
         * Method: TVObject::SetVariable
         * Purpose: Sets a value in Variable form
         * Parameters: const TString& name - the name of the variable
         *              TrecPointer<TVariable> var - the value to set
         * Returns: bool - whether the variable was valid or not
         *
         * Attributes: abstract
         */
        virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var) override;

        TcTextData data;
    };

    class TcWebNodeElement : public TcNodeElement
    {
    protected:
        NodeContainerType nodeType;
        TrecSubPointer<TPage, TcWebNode> node;
    public:
        TcWebNodeElement(TrecSubPointer<TPage, TcWebNode> node);
        TrecSubPointer<TPage, TcWebNode> GetWebNode();

        virtual NodeContainerType GetElementType() override;
        virtual D2D1_RECT_F GetLocation() override;

        /**
         * Method: TVObject::GetVariable
         * Purpose: Revtrieves a value in Variable form
         * Parameters: const TString& name - the name of the variable
         *              TrecPointer<TVariable>& var - holds the variable retrieved if valid
         * Returns: bool - whether the variable was valid or not
         *
         * Attributes: abstract
         */
        virtual bool GetVariable(const TString& name, TrecPointer<TVariable>& var) override;

        /**
         * Method: TVObject::SetVariable
         * Purpose: Sets a value in Variable form
         * Parameters: const TString& name - the name of the variable
         *              TrecPointer<TVariable> var - the value to set
         * Returns: bool - whether the variable was valid or not
         *
         * Attributes: abstract
         */
        virtual bool SetVariable(const TString& name, TrecPointer<TVariable> var) override;
    };

    class TcTextStructure
    {
    public:
        TrecSubPointer<TTextElement, TComplexTextElement> text;
        TDataArray<DWRITE_TEXT_RANGE> ranges;
    };

    /**
     * represents the Element Node's innerHTML attribute
     */
    TrecPointer<TStringSlice> innerHtml;

    /**
     * Holds on to all child Elements underneath it
     */
    TDataArray<TrecPointer<TcNodeElement>> childNodes;

    /**
     * The attributes gained (attributes from the HTML element declaration, )
     */
    TDataMap<TString> attributes, exAttributes;

    /**
     * Basic string attributes
     */
    TString tagName, id, nodeClass, name, value;
    
    /**
     * 3D Engine
     */
    TrecPointer<TWindowEngine> d3dEngine;
    /**
     * Window Handle
     */
    HWND win;
    /**
    * Whether to display at all
    */
    bool doDisplay, shouldDraw;

    /**
     * The Display outside Property
     */
    TcWebNodeDisplayOutside outsideDisplay;
    /**
     * The Display Inside Property
     */
    TcWebNodeDisplayInside insideDisplay;
    /**
     * The internal dispay to use (applies to table and (eventually) ruby)
     */
    TcWebNodeDisplayInternal internalDisplay;
    /**
     * Whether it is a List item
     */
    bool isListItem;
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

    /**
     * Basic Border Attributes
     */
    TcBorderData borderData;
    /**
     * Basic Text Attributes
     */
    TcTextData textData;

    /**
     * Margins of the App (inner between text/content and border, outer between border and boundaries)
     */
    D2D1_RECT_F innerMargin, outerMargin;

    /**
     * List of Text Elements to Draw
     */
    TDataArray<TrecPointer<TcTextStructure>> textElements;

    /**
     * Represents the handlers that are meant to Run
     */
    TDataMap<EventPropagater> handlers;

    /// Table resources

    /**
     * Column and Row Data
     */
    TDataArray<UINT> columnSizes, rowSizes;

    /**
     * Column and Row Spans
     */
    UCHAR columnSpan, rowSpan;

public:

    /**
     * Method: TWebNode::TWebNode
     * Purpose: Constructor
     * Parameters: TrecPointer<DrawingBoard> board - the board to draw on
     * Returns: new Html Node
     */
    TcWebNode(TrecPointer<DrawingBoard> board);

    ///
    /// TPage Methods
    /// 

    /**
     * Method: TcWebNode::HandlesEvents
     * Purpose: Whether the object is of a Page type that Handles Events (i.e. has what would be called an Event Handler, an object that
     *      runs it's own methods in response to receiving an "event"
     * Parameters: void
     * Returns: bool - whether the page had an "Event Handler" (Top level Page types, such as 'TAnafacePage' or 'TWebPage' should report true while sub pages such as
     *              'TControl' or 'TWebNode' should report false)
     *
     * Attributes: override
     */
    virtual bool HandlesEvents() override;

    /**
     * Method: TcWebNode::Draw
     * Purpose: Draws the Page to the Window
     * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
     * Returns: void
     *
     * Attributes: override
     */
    virtual void Draw(TrecPointer<TVariable> object) override;

    /**
     * Method: TcWebNode::OnRButtonUp
     * Purpose: Responds to the Right Button Up Message
     * Parameters: UINT nFlags - flags associated with the message
     *				const TPoint& point - the point included in the message
     *				message_output& mOut -  the result of the message
     *				TDataArray<EventID_Cred>& - list of events to be handled
     * Returns: void
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


    /**
     * Method: TcWebNode::OnRButtonDown
     * Purpose: Responds to the Right Button Down Message
     * Parameters: UINT nFlags - flags associated with the message
     *				const TPoint& point - the point included in the message
     *				message_output& mOut -  the result of the message
     *				TDataArray<EventID_Cred>& - list of events to be handled
     * Returns: void
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


    /**
     * Method: TcWebNode::OnLButtonUp
     * Purpose: Responds to the Left Button Up Message
     * Parameters: UINT nFlags - flags associated with the message
     *				const TPoint& point - the point included in the message
     *				message_output& mOut -  the result of the message
     *				TDataArray<EventID_Cred>& - list of events to be handled
     * Returns: void
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;


    /**
     * Method: TcWebNode::OnLButtonDown
     * Purpose: Responds to the Left Button Down Message
     * Parameters: UINT nFlags - flags associated with the message
     *				const TPoint& point - the point included in the message
     *				message_output& mOut -  the result of the message
     *				TDataArray<EventID_Cred>& - list of events to be handled
     * Returns: void
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&) override;

    /**
     * Method: TcWebNode::OnMouseMove
     * Purpose: Responds to the Mouse Move Message
     * Parameters: UINT nFlags - flags associated with the message
     *				TPoint point - the point included in the message
     *				message_output& mOut -  the result of the message
     * Returns: void
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&) override;

    /**
     * Method: TcWebNode::OnLButtonDblClk
     * Purpose: Responds to the Left Button Double CLick Message
     * Parameters: UINT nFlags - flags associated with the message
     *				TPoint point - the point included in the message
     *				message_output& mOut -  the result of the message
     * Returns: void
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>& eventAr) override;

    /**
     * Method: TcWebNode::OnResize
     * Purpose: Resizes the Page
     * Parameters: D2D1_RECT_F& newLoc - the new regoin of the Page
     *				UINT nFlags - flags associated with the move
     *				TrecPointer<TWindowEngine> - the 3D Engine to work with
     *				TDataArray<EventID_Cred>& eventAr - list of events
     * Returns: void
     *
     * Note: May be Deprecated soon once the MiniHandler is removed from the library
     *
     * Attributes: message; override
     */
    ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr) override;

    /**
     * Method: Page::OnDestroy
     * Purpose: Reports whether the Page is ready to be destroyed
     * Parameters: void
     * Returns: bool - true if the Page doesn't have a handler or that handler is ready to be destroyed
     *
     * Attributes: message; override
     */
    ag_msg virtual bool OnDestroy() override;

    /**
     * Method: TcWebNode::OnScroll
     * Purpose: Sends Scroll Command to controls
     * Parameters: const TPoint& point - point of the mouse
     *				const TPoint& direction - how far to send the scroll
     * Returns: bool - whether message was recieved
     *
     * Attributes: message; override
     */
    ag_msg virtual bool OnScroll(bool fromBars, const TPoint& point, const TPoint& direction, TDataArray<EventID_Cred>&) override;


    /**
     * Method: TWebNode::ProcessHtml
     * Purpose: Processes new HTML code from a file
     * Parameters: TStringSliceManager& html the string to process
     *              UINT start - the index to start at
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessHtml(TStringSliceManager& html, UINT& start, HWND win, TrecPointer<TArray<styleTable>>& styles);
    
    /**
     * Method: TcWebNode::PreCreate
     * Purpose: Allows Node to predict ahead of time what space it will need and how it will be shown
     * Parameters: TrecPointerSoft<TWebNode> parent - the Web Node that called the method
     *              TrecPointer<TArray<styleTable>> styles - list of CSS styles that the node should adhere to
     * Returns: void
     */
    void PreCreate(TrecSubPointerSoft<TPage, TcWebNode> parent, TrecPointer<TArray<styleTable>>& styles);

    /**
     * Method: TcWebNode::CreateWebNode
     * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
     * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
     *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
     *              HWND window - handle to the window the node is operating in
     *              TrecPointerSoft<TWebNode> parent - the node that called this method
     * Returns: UINT - Error Code
     */
    virtual UINT CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window);
protected:

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
     * Method: TWebNode::SetDisplay
     * Purpose: Sets up the display attributes of the web node according to the string provided
     * Parameters: const TString& display - the string holding display info
     * Return: void
     */
    void SetDisplay(const TString& display);

    /**
     * Method: TWebNode::IsText
     * Purpose: Reports whether this Node chiefly holds Text that can be inserted into parent text
     * Parameters: void
     * Returns: bool - whether the Node can be treated as text that can be inserted into parent text
     */
    bool IsText();

    /**
     * Method: TcWebNode::ProcessTextNodes
     * Purpose: Processes a Set of Text Nodes so that they can be compiled into a sinle line of text
     * Parameters:  TDataArray<TrecPointer<TcNodeElement>>& textNodes - the text nodes to process
     *              const D2D1_RECT_F& location - the area where the text can be located at
     * Returns: void
     */
    void ProcessTextNodes(TDataArray<TrecPointer<TcNodeElement>>& textNodes, D2D1_RECT_F& location);

    /**
     * Method: TcWebNode::RetrieveTextNodes
     * Purpose: Enables Parent Nodes to retrieve a collection of TextNodes
     * Parameters: TDataArray<TrecPointer<TcNodeElement>>& textNodes - the textNodes to collect
     * Returns: void:
     */
    void RetrieveTextNodes(TDataArray<TrecPointer<TcNodeElement>>& textNodes);

    /**
     * Method: TcWebNode::GetListPrePend
     * Purpose: Called by list-items, provides the list item with the Prepend String
     * Parameters: void
     * Returns: TString - the current prepender
     */
    TString GetListPrepend();

    /**
     * Method: TWebNode::ShrinkHeight
     * Purpose: Shrinks the Node to the minimum neccesary to display
     * Parameters: void
     * Returns: void
     */
    void ShrinkHeight();

    /**
     * Method: TWebNode::CompileMargin
     * Purpose: Converts String data into a margin
     * Parameters: const TSTring& marginString
     *              border_side side - the border side to focus on
     *              bool inner - inner border
     * Returns: void
     */
    void CompileMargin(const TString& marginString, tc_border_side side, bool inner);

    /**
     * Method: TcWebNode::ConvertMeasurement
     * Purpose: Converts a value based off of the measurement unit provided
     * Parameters: TString& atts - the value to convert
     * Return: float - the value provided
     */
    float ConvertMeasurement(TString& atts);

    /**
     * Method: TWebNode::DrawBorder
     * Purpose: Holds the logic for actually drawing a border around the content
     * Parameters: void
     * Returns: void
     */
    void DrawBorder();

    /**
     * Method: TWebNode::ProcessInnerHtml
     * Purpose: Compiles the html the node has been given
     * Parameters: void
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win, TrecPointer<TArray<styleTable>>& styles);
};

