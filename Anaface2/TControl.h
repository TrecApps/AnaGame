#pragma once
#include "Anaface2.h"
#include <TPage.h>
#include <TTextElement.h>
#include <TDataMap.h>

/**
 * Enum Class: dimension_spec
 * Purpose: Indicated the Dimension Detail to work with
 */
typedef enum class dimension_spec
{
	ds_width,
	ds_width_min,
	ds_width_max,
	ds_height,
	ds_height_min,
	ds_height_max,
}dimension_spec;


/**
 * Class: EventTypeId
 * Purpose: Stored by Controls for checking whether or not they actually have a message handler
 */
class EventTypeID
{
public:
	EventTypeID();
	EventTypeID(const EventTypeID& copy);

	R_Message_Type eventType;
	int eventID;
};


/**
 * Enum Class: messageState
 * Purpose: Used by the control to determine which to draw
 */
typedef enum class  messageState
{
	normal,      // Control is in normal mode
	mouseHover,  // Control is in Mouse-Honer mode
	mouseLClick, // Control is in Mouse-Click mode
	mouseRClick  // Control is in Mouse-Right-Click mode
} messageState;

/*
 * Class: TControlComponent
 * Purpose: Draws the Border of a Given TControl
 */
class _ANAFACE_DLL2 TControlComponent : TObject
{
	friend class TControl;
	friend class TControlComponent;
	friend class TrecPointerKey;
public:

	/**
	 * Method: TControlComponent::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType() override;

	/*
	* Method: TControlComponent::TControlComponent
	* Purpose: Constructor
	* Parameters: TrecPointer<TControlComponent> & rBord - Reference to the Border to copy
	*				TControl* tc_holder - the Control to work for
	* Returns: New TControlComponent Object
	*/
	TControlComponent(TrecPointer<TControlComponent>&, TrecPointer<TPage> parent);
	/*
	* Method: TControlComponent::~TControlComponent
	* Purpose: Destructor
	* Parameters: void
	* Returns: void
	*/
	~TControlComponent();

	// Various On Create Methods depending on the shape of the control

	/*
	* Method: TControlComponent::onCreate
	* Purpose: Sets up a basic Rectangular border
	* Parameters: RECT location - the location the Border should draw on
	* Returns: bool - success
	*/
	bool onCreate(D2D1_RECT_F);
	/*
	* Method: TControlComponent::onCreate
	* Purpose: Sets up a border optimized for dawing ovals
	* Parameters: D2D1_ELLIPSE e - the oval to draw
	* Returns: bool - success
	*/
	bool onCreate(D2D1_ELLIPSE);
	/*
	* Method: TControlComponent::onCreate
	* Purpose: Sets up a Border that has a rectangle with rounded corners
	* Parameters: D2D1_ROUNDED_RECT rr - the border to draw
	* Returns: bool - success
	*/
	bool onCreate(D2D1_ROUNDED_RECT);

	// Drawing Method

	/*
	* Method: TControlComponent::onDraw
	* Purpose: Draws the border
	* Parameters: void
	* Returns: void
	*/
	void onDraw(D2D1_RECT_F& loc);

	// Handling Colors
	// NOTE: For Compatibility between AnaGame for Windows and an eventual AnaGame for Unix,
	//	Parameters and Return values should be replaced with a cross-platform structure 


	// Methods for handling Opaquency. Note, there is no getOpaquency() method as it is handled by the getColored method 

	/*
	 * Method: TControlComponent::setThickness
	 * Purpose: Allows the thickness to be updated after creation
	 * Parameters: float f - the thickness to set to
	 * Returns: void
	 */
	void setThickness(float f);
	/*
	* Method: TControlComponent::setOpaquency
	* Purpose: Allows the Opaquency to be updated after creation
	* Parameters: float f - the opaquency to set to (no upadte occurs if value is out of range)
	* Returns: void
	*/
	void setOpaquency(float f);
	/*
	* Method: TControlComponent::getThickness
	* Purpose: Allows the current thickness to be seen
	* Parameters: void
	* Returns: float - the current thickness
	*/
	float getThickness();

	// Getting the Location

	/*
	 * Method: TControlComponent::getLocation
	 * Purpose: Retrieves the Location of the current border
	 * Parameters: void
	 * Returns: RECT - the current location
	 */
	D2D1_RECT_F getLocation();
	/*
	* Method: TControlComponent::ShiftHorizontal
	* Purpose: Moves the Border left-or right, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the Border and which direction
	* Returns: void
	*/
	void ShiftHorizontal(int degrees);
	/*
	* Method: TControlComponent::ShiftVertical
	* Purpose:Moves the Border up or down, expected to be used by TLayouts
	* Parameters: int degrees - how much to move the border and which direction
	* Returns: void
	*/
	void ShiftVertical(int degrees);

	/*
	 * Method: TControlComponent::GetBrush
	 * Purpose: Retrieves the Brush used by the control component
	 * Parameters: void
	 * Returns: void
	 */
	TrecPointer<TBrush> GetBrush();

	/**
	 * Method: TControlComponent::GetControlBorder
	 * Purpose: Retrieves a new Control Component that represents a border
	 * Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render target to draw against
	 *				TrecPointer<TPage> parent - the TControl to which the TControlComponent is a party to
	 * Returns: TrecPointer<TControlComponent> - the component requested
	 * 
	 * Atributes: static
	 */
	static TrecPointer<TControlComponent> GetControlBorder(TrecPointer<DrawingBoard> board, TrecPointer<TPage> parent);


	/**
	 * Method: TControlComponent::GetControlContent
	 * Purpose: Retrieves a new Control Component that represents a border
	 * Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render target to draw against
	 *				TrecPointer<TPage> parent - the TControl to which the TControlComponent is a party to
	 * Returns: TrecPointer<TControlComponent> - the component requested
	 *
	 * Atributes: static
	 */
	static TrecPointer<TControlComponent> GetControlContent(TrecPointer<DrawingBoard> board, TrecPointer<TPage> parent);

private:


	/**
 	 * Method:  TControlComponent::TControlComponent
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> dbp - Smart Pointer to the Render target to draw against
	 *				TrecPointer<TPage> parent - the TControl to which the TControlComponent is a party to
	 * Returns: New TControlComponent Object
	 */
	TControlComponent(TrecPointer<DrawingBoard>, TrecPointer<TPage> parent);

	/**
	 * Whether this component represents content
	 */
	bool isContent;

	/**
	 * The Brush to Paint with
	 */
	TrecPointer<TBrush> brush;

	void ResetBrush();

	/**
	 * Used by the Builder to Highlight Controls under editing (not sure if it is still useful?)
	 */
	TrecPointer <TBrush> BuilderFocusBrush;
	/**
	 * thickness of the border
	 */
	float thickness;
	/**
	 * Holds a special style, NOTE: Feature not Implemented
	 */
	TString style;

	/**
	 * Pointer to the DrawingBoard to Draw to
	 */
	TrecPointer<DrawingBoard> drawingBoard;
	/**
	 * Holds images to draw if provided
	 */
	TrecSubPointer<TBrush, TBitmapBrush> image;

	/**
	 * The TControl that owns the Border
	 */
	TrecPointerSoft<TPage> parent;

	/**
	 * Shape to Draw
	 */
	TShape shape;

	/**
	 * Holds color, used for creating the brush
	 */
	TGradientStopCollection stopCollection;

	/**
	 * whether we are creating a radial or gradient brush (ignored if only one color is given)
	 */
	bool useRadial;

	// Alternate shapes


	/**
	 * the rect to pass into the Border and content if used
	 */
	D2D1_RECT_F loci;

	/**
	 * the ellipse to pass into the Border and content if used
	 */
	D2D1_ELLIPSE circle;

	/**
	 * the rounded rect to pass into the Border and content if used
	 */
	D2D1_ROUNDED_RECT roundedRect;
};




/**
 * Class: TControl
 * Purpose: Base Control type for the Anaface Engine
 * 
 * Superclass: TPage - enables integration with Anagame's new UI-Architexture
 */
class _ANAFACE_DLL2 TControl :
    public TPage
{
public:


	/**
	 * Method: TControl::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TControl::TControl
	 * Purpose: Constructor
	 * Parameters: TrecPointer<DrawingBoard> db - Smart Pointer to the Render Target to draw on
	 *				TrecPointer<TArray<styleTable>> styTab - Smart Pointer to the list of styles to draw from
	 * Return: New TControl Object
	 */
	TControl(TrecPointer<DrawingBoard> drawingBoard, TrecPointer<TArray<styleTable>> styles);

	/**
	 * Method: TControl::setActive
	 * Purpose: Enables controls to be either active (default) or inactive (they don't draw or respond to events)
	 * Parameters: bool act - whether control should be active or not
	 * Returns: void
	 */
	void setActive(bool act);

	/**
	 * Method: TControl::GetActive
	 * Purpose: Reports whether the control is 'active' or not
	 * Parameters: void
	 * Returns: bool - whether the control is active or not
	 */
	bool GetActive();


	/**
	 * Method: TControl::onCreate
	 * Purpose: Allows the Control To contstruct itself based off of the location it has and the
	 *		screen space it is given
	 * Parameters: const D2D1_RECT_F& loc- the location this control is bound by
	 * Returns: bool - success
	 * Note: You Must call this on the Root Control before any control can be drawn on sreen
	 *
	 * Attributes: virtual
	 */
	virtual bool onCreate(const D2D1_RECT_F& loc, TrecPointer<TWindowEngine> d3d);

	/**
	 * Method:  TControl::updateArrayID
	 * Purpose: Stores an index to an external array, to be determined by the software using the control
	 * Parameters: int aid - the array index to store, default -1
	 * Returns: void
	 */
	void updateArrayID(int aid);


	/**
	 * Method: TControl::getMargin
	 * Purpose: Retrieves the space between the control's location and the area it was given
	 * Parameters: void
	 * Returns: D2D1_RECT_F - the margin
	 */
	D2D1_RECT_F getMargin();

	/**
	 * Method: TControl::SetMargin
	 * Purpose: Sets the margin of the Control
	 * Parameters: const D2D1_RECT_F& newMargin - the margin to set
	 * Returns: bool - whether the margin is set or not (should be true, but if the margin is large enough that the resulting location will be inversed, can return false)
	 */
	bool SetMargin(const D2D1_RECT_F& newMargin);

	/**
	 * Method: TControl::ShiftHorizontal
	 * Purpose: Moves the Control left-or right, expected to be used by TLayouts
	 * Parameters: int degrees - how much to move the control and which direction
	 * Returns: void
	 */
	void ShiftHorizontal(int degrees);

	/**
	 * Method: TControl::ShiftVertical
	 * Purpose:Moves the Control up or down, expected to be used by TLayouts
	 * Parameters: int degrees - how much to move the control and which direction
	 * Returns: void
	 */
	void ShiftVertical(int degrees);

	/**
	 * Method: TControl::SetDimension
	 * Purpose: Sets the specified dimension according to the value provided
	 * Parameters: UINT dimensionValue - the value to set the specified dimension to
	 *				dimension_spec - the dimension to set the value to
	 * Returns: bool - whether the dimension was set or not (should be true, but if the minimum is set, for example, then the max must be set to 0 or above the minimum)
	 */
	bool SetDimension(UINT dimensionValue, dimension_spec spec);

	/**
	 * Method: TControl::GetDimension
	 * Purpose: Retrieves the specified dimension of the control
	 * Parameters: dimension_spec - the dimension to retrieve
	 * Returns: UINT - the dimension value (if 0, then assume dimension has not been set)
	 */
	UINT GetDimension(dimension_spec spec);


	/**
	 * Method: TControl::AddClass
	 * Purpose: Labels to serve as id's to draw styles from, useful in determining what HTML/TML element was used in creating the control
	 * Parameters: TString& t - Class Name to add
	 * Returns: void
	 */
	void AddClass(const TString& t);

	/**
	 * Method: TControl::GetID
	 * Purpose: Retrieves the ID for the Control
	 * Parameters: void
	 * Returns: TString - the ID of the Control (Careful, String could be blank)
	 */
	TString GetID();


	/**
	 * Method: TControl::HandlesEvents
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
	 * Method: TControl::Draw
	 * Purpose: Draws the Page to the Window
	 * Parameters: TrecPointer<TVariable> object - Memory Safe means of enabling Data-Binding, if the Page has to tailor it's drawing to data provided by this parameter
	 * Returns: void
	 *
	 * Attributes: override
	 */
	virtual void Draw(TrecPointer<TVariable> object) override;


	/**
	 * Method: TControl::OnRButtonUp
	 * Purpose: Responds to the Right Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TControl::OnRButtonDown
	 * Purpose: Responds to the Right Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnRButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TControl::OnLButtonUp
	 * Purpose: Responds to the Left Button Up Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonUp(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;


	/**
	 * Method: TControl::OnMouseMove
	 * Purpose: Responds to the Mouse Move Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnMouseMove(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>&) override;

	/**
	 * Method: TControl::OnLButtonDblClk
	 * Purpose: Responds to the Left Button Double CLick Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				TPoint point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 * Returns: void
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual void OnLButtonDblClk(UINT nFlags, TPoint point, message_output& mOut, TDataArray<EventArgs>&) override;

	/**
	 * Method: TControl::OnLButtonDown
	 * Purpose: Responds to the Left Button Down Message
	 * Parameters: UINT nFlags - flags associated with the message
	 *				const TPoint& point - the point included in the message
	 *				message_output& mOut -  the result of the message
	 *				TDataArray<EventID_Cred>& - list of events to be handled
	 * Returns: void
	 *
	 * Attributes: message; abstract
	 */
	ag_msg virtual void OnLButtonDown(UINT nFlags, const TPoint& point, message_output& mOut, TDataArray<EventID_Cred>&, TDataArray<EventArgs>& args) override;

	/**
	 * Method: TControl::OnResize
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
	ag_msg virtual void OnResize(D2D1_RECT_F& newLoc, UINT nFlags, TDataArray<EventID_Cred>& eventAr, TDataArray<EventArgs>&) override;

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
	 * Method: TControl::OnScroll
	 * Purpose: Sends Scroll Command to controls
	 * Parameters: const TPoint& point - point of the mouse
	 *				const TPoint& direction - how far to send the scroll
	 * Returns: bool - whether message was recieved
	 *
	 * Attributes: message; override
	 */
	ag_msg virtual bool OnScroll(bool, const TPoint& point, const TPoint& direction, TDataArray<EventArgs>&) override;



protected:

	/**
	 * Attributes for creation
	 */
	TDataMap<TString> attributes;
	
	/**
	 * whether the sizes are fixed upon creation or not
	 */
	bool fixedWidth, fixedHeight;

	/**
	 * The Border and Content of the Control
	 */
	TrecPointer<TControlComponent> border, content;

	/**
	 * Text Support for TControls
	 */
	TrecPointer<TTextElement> text;

	/**
	 * List of Style classes
	 */
	TString className;
	/**
	 * The Name of the Control (comparable to HTML ID's)
	 */
	TString ID;
	/**
	 * Where the Control is to reside
	 */
	D2D1_RECT_F location, margin, bounds;


	/**
	 * the ellipse to pass into the Border and content if used
	 */
	D2D1_ELLIPSE ellipse;
	/**
	 * the rounded rect to pass into the Border and content if used
	 */
	D2D1_ROUNDED_RECT roundedRect;

	TrecPointer<TArray<styleTable>> styles;

	/**
	 * ScrollBars in case content is larger than it's allocated drawing space
	 */
	TrecPointer<TScrollBar> vScroll, hScroll;


	/**
	 * The shape the control is in
	 */
	TShape shape;

	/**
	 * the Matrix to manage rotations and other transformations with
	 */
	D2D1_MATRIX_3X2_F controlTransform;

	/**
	 * Dimension support
	 */
	TrecPointer<Dimensions> dimensions;


	/**
	 * Holds index of this TControl if stored in an external container
	 */
	int arrayID;


	/**
	 * should response to message stop parent from doing the same?
	 */
	bool overrideParent;

	/**
	 * mechanism as to which version of the TControl should be drawn based off of user interaction
	 */
	messageState mState;

	/**
	 * Information about the arguements set when an event is generated
	 */
	EventArgs args;

	/**
	 * List of Events to report given certain messages
	 */
	TDataArray<EventTypeID> eventList;

	/**
	 * Controls can be disabled, through this bool
	 */
	bool isActive;

	/**
	 * Keeps track of being Clicked, and mouse movements
	 */
	bool isRightClicked, isLeftClicked, isMouseIn;

protected:
	/**
	 * Method: TControl::HasEvent
	 * Purpose: Whether the TControl Possesses the Event type specified
	 * Parameters: R_Message_Type mType - the message Type to check for
	 * Returns: int - the index of the event method (or -1)
	 */
	int HasEvent(R_Message_Type mType);

	/**
	 * Method: TControl::OnCreateSize
	 * Purpose: Aids the creation Method by Handling Sizing Logic
	 * Parameters: void
	 * Returns: void
	 */
	void OnCreateSize();

	/**
	 * Method: TControl::SetSize
	 * Purpose: Aids the setting of the controls location upon being set with new bounds
	 * Parameters: void
	 * Returns: void
	 */
	void SetSize();

	/**
	 *
	 */
	void OnCreateStyle(TDataMap<TString>& atts);
};

