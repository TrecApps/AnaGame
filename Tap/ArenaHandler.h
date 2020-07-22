#pragma once
#include "EventHandler.h"

class ArenaHandler;
typedef void (ArenaHandler::* ArenaHandlerEvents)(TrecPointer<TControl> tc, EventArgs ea);

/**
 * Class: ArenaHandler
 * Purpose: Extends the EventHandler in order to manage an Arena
 */
class _TAP_DLL ArenaHandler :
	public EventHandler
{
public:

	/**
	 * Method: ArenaHandler::ArenaHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - the instance associated with the Page/handler
	 *				TString& name - name of the handler, used for identifying the Camera Handler
	 * Returns: New ArenaHandler instance
	 */
	ArenaHandler(TrecPointer<TInstance> instance, TString& name);
	/**
	 * Method: ArenaHandler::~ArenaHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~ArenaHandler();

	/**
	 * Method: ArenaHandler::Initialize
	 * Purpose: Initializes the Handler so that it has direct Access to certain Controls held by the page
	 * Parameters: TrecPointer<Page> page - page that holds the Controls to latch on to
	 * Returns: void
	 */
	virtual void Initialize(TrecPointer<Page> page) override;
	/**
	 * Method: ArenaHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: ArenaHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 *
	 * Note: The Arena expects the message to be of the format:
	 *     "[object] [task] [float;float...]"
	 *
	 *  where "[object]" currently would be "Camera".
	 *  "[task]" can be "Location" (3 floats), "Direction" (3 floats), "Tanslate" (3 floats), or "Rotate" (2 floats)
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

private:
	/**
	 * The Dedicated Arena to show and control
	 */
	TrecSubPointer<TControl, TArena> arenaControl;

	/**
	 * The Engine held by the arena control
	 */
	TrecPointer<TArenaEngine> engine;

	/**
	 * Name of the Handler/page pair (used for sending a message to the camera control
	 */
	TString name;

	/**
	 * Method: ArenaHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message) override;

	/**
	 * List of Handler Methods specific to the ArenaHandler
	 */
	TDataArray<ArenaHandlerEvents> arenaHandlers;
};

