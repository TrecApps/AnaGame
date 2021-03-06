#pragma once
#include "EventHandler.h"

class CameraHandler;


typedef void (CameraHandler::* CameraHandlerEvents)(TrecPointer<TControl> tc, EventArgs ea);

/**
 * Class: CameraHandler
 * Purpose: Extends the EventHandler to handle events generated by a Camera Page
 * 
 * SuperClass: EventHandler
 */
class _TAP_DLL CameraHandler :
	public EventHandler
{
public:

	/**
	 * Method: CameraHandler::CameraHandler
	 *Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - the instance associated with the Page/handler
	 *				TString& name - name of the handler, used for identifying the Arena Handler
	 * Returns: New ArenaHandler instance
	 */
	CameraHandler(TrecPointer<TInstance> instance, TString& name);

	/**
	 * Method: CameraHandler::~CameraHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~CameraHandler();


	/**
	 * Method: CameraHandler::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: CameraHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void Initialize(TrecPointer<Page> page) override;

	/**
	 * Method: CameraHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 * 
	 * Attributes: override
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: CameraHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 * 
	 * Attributes: override
	 *
	 * Note: The Camera Handler expects the message to come in the form:
	 *      "Camera Location [float;float;float] Direction [float;float;float]"
	 *
	 *      "Location" and "Direction" can be swapped
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;


protected:

	/**
	 * Method: CameraHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 * 
	 * Attributes: override
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	/**
	 * Method: CameraHandler::SendMessageToArena
	 * Purpose: Prepares a message to send to the Arena Handler
	 * Parameters: const TString& target - The Name of the object to operate on
	 *				const TString& attribute - the Task to perform on the object
	 *				const TString& value - set of floats to provide
	 * Returns: void
	 */
	void SendMessageToArena(const TString& target, const TString& attribute, const TString& value);


	// Data to work with

	/**
	 * Whether the buttons will cause the camera to rotate or not
	 */
	bool rotateMode;

	/**
	 * Name of the Arena Engine to work with
	 */
	TString arenaName;
	
	/**
	 * Holder floats determining the x-y-z Location and Direction of the Camera
	 */

	float t_lx, t_ly, t_lz, t_dx, t_dy, t_dz;
	
	/**
	 * When changing by button, how much to change by
	 */
	float change;

	// Controls

	/**
	 * Text Controls reporting the x-y-z location and direction of the camera
	 */
	TrecSubPointer<TControl, TTextField> d_x, d_y, d_z, l_x, l_y, l_z;

	/**
	 * Control used for toggling the camera model mode (not active right now)
	 */
	TrecPointer<TControl> camModToggleSign;

	/**
	 * Method: CameraHandler::UpdatePanelText
	 * Purpose: Updates the various text controls that show the x-y-z values of both the Direction and Location of the camera
	 * Parameters: void
	 * Returns: void
	 */
	void UpdatePanelText();

	// Handler Resources

	/**
	 * List of CameraHandler handler methods
	 */
	TDataArray<CameraHandlerEvents> arenaHandlers;

	// Handler Methods


	/**
	 * Method: CameraHandler::TextDirectionX
	 * Purpose: Responds to text changes from the x-direction control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void TextDirectionX(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::TextLocationX
	 * Purpose: Responds to text changes from the x-location control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void TextLocationX(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::TextDirectionY
	 * Purpose: Responds to text changes from the y-direction control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void TextDirectionY(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::TextLocationY
	 * Purpose: Responds to text changes from the y-location control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void TextLocationY(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::TextDirectionZ
	 * Purpose: Responds to text changes from the z-direction control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void TextDirectionZ(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::TextLocationZ
	 * Purpose: Responds to text changes from the z-location control
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void TextLocationZ(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnUp
	 * Purpose: Responds to the Up button, and moves the camera "up", whether location or rotation
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnUp(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnDown
	 * Purpose: Responds to the Up button, and moves the camera "down", whether location or rotation
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnDown(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnLeft
	 * Purpose: Responds to the Up button, and moves the camera "left", whether location or rotation
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnLeft(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnRight
	 * Purpose: Responds to the Up button, and moves the camera "right", whether location or rotation
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnRight(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnNear
	 * Purpose: Responds to the Up button, and moves the camera "near", when set to location (rotation is not affected)
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnNear(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnFar
	 * Purpose: Responds to the Up button, and moves the camera "far", when set to location (rotation is not affected)
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnFar(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnSetCameraRotate
	 * Purpose: Sets the mode for rotation when buttons are clicked
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnSetCameraRotate(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnSetCameraTranslate
	 * Purpose: Sets the mode for translation when buttons are clicked
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnSetCameraTranslate(TrecPointer<TControl> tc, EventArgs ea);


	/**
	 * Method: CameraHandler::OnSelectObject
	 * Purpose: [TBD - written when using the old architexture]
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnSelectObject(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnToggleObjectAndCamera
	 * Purpose: [TBD - written when using the old architexture]
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnToggleObjectAndCamera(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: CameraHandler::OnGetDefaultObject
	 * Purpose: [TBD - written when using the old architexture]
	 * Parameters: TrecPointer<TControl> tc - The Control that generated the event
	 *				EventArgs ea - The parameters of the event
	 * Returns: void
	 */
	void OnGetDefaultObject(TrecPointer<TControl> tc, EventArgs ea);

};

