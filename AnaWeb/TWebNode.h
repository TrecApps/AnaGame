#pragma once
#include <TObject.h>
#include <DrawingBoard.h>
#include <TControl.h>
#include <TStringSlice.h>
#include <TDataMap.h>

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
     * Method: GetType
     * Purpose: Reports the Type of this object
     * Parameters: void
     * Returns: TString - the string representation of this objects type
     */
    virtual TString GetType() override;
protected:
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
};

