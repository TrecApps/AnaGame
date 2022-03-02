#pragma once
#include <TPage.h>
#include "AnaWeb.h"
#include <TStringSlice.h>
class ANA_WEB_DLL TcWebNode :
    public TPage
{
protected:
    typedef enum class NodeContainerType
    {
        nct_text,
        nct_control,
        nct_web,
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
    public :
        virtual NodeContainerType GetElementType() override;
        virtual D2D1_RECT_F GetLocation() override;
    };

    class TcWebNodeElement : public TcNodeElement
    {
    public:
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
    UINT ProcessHtml(TStringSliceManager& html, UINT& start, HWND win);

    /**
     * Method: TWebNode::ProcessInnerHtml
     * Purpose: Compiles the html the node has been given
     * Parameters: void
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessInnerHtml(TStringSliceManager& html, UINT& start, HWND win);
};

