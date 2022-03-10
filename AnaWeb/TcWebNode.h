#pragma once
#include <TPage.h>
#include "AnaWeb.h"
#include <TStringSlice.h>
#include <TDataMap.h>



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
     * Method: TWebNode::ProcessInnerHtml
     * Purpose: Compiles the html the node has been given
     * Parameters: void
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win, TrecPointer<TArray<styleTable>>& styles);
};

