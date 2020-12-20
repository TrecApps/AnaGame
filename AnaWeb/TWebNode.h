#pragma once
#include <TObject.h>
#include <DrawingBoard.h>
#include <TPromptControl.h>
#include <TStringSlice.h>
#include <TDataMap.h>

/**
 * Class: EventPropagater
 * Purpose: Helps determine if a web node should report it's event first or let any child nodes report
 * 
 * Note: Information related to event propagation could be found here:
 *   https://stackoverflow.com/questions/4616694/what-is-event-bubbling-and-capturing#:~:text=Event%20bubbling%20and%20capturing%20are%20two%20ways%20of,in%20which%20order%20the%20elements%20receive%20the%20event.
 */
class EventPropagater {
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
 * Class: TWebNode
 * Purpose: Represents HTML body Nodes, holds on to the Appropriate TControl (if necessary)
 * 
 * SuperClass: TObject
 */
class TWebNode :
    public TObject
{
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
    UINT ProcessHtml(TStringSliceManager& html, UINT& start);

    /**
     * Method: TWebNode::ProcessInnerHtml
     * Purpose: Compiles the html the node has been given
     * Parameters: void
     * Returns: UINT - error code (0 for success)
     */
    UINT ProcessInnerHtml(TStringSliceManager& html, UINT& start);

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
     * Method: TWebNode::CreateWebNode
     * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
     * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
     *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
     *              TrecPointer<TArray<styleTable>> styles - list of CSS styles that the node should adhere to
     *              HWND window - handle to the window the node is operating in
     */
    UINT CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, TrecPointer<TArray<styleTable>> styles, HWND window);

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
    TDataArray<TrecPointer<TWebNode>> childNodes;

    /**
     * Represents the actual control, if need be
     */
    TrecPointer<TControl> control;

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
    TrecPointerSoft<TWebNode> self;

    TDataArray<FormattingDetails> formattingDetails;
};

