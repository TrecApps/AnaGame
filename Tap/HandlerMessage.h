#pragma once
#include <TObject.h>
#include <TString.h>

#include "Tap_dll.h"

/**
 * Enum Class: handler_type
 * Purpose: allows Tap objects to identify an Event Handler by type
 */
typedef enum class handler_type
{
	handler_type_generic,     // Generic Handler, will need a numeric code
	handler_type_console,     // Console Handler, references a "TerminalHandler"
	handler_type_camera,      // references a "CameraHandler"
	handler_type_file_manager,// references a "FileHandler"
	handler_type_compiler,    // references a compile related handler that has not been written yet
	handler_type_interpreter, // references an interpretor related handler that has not been written yet
	handler_type_okay,        // references the "OkayHandler"
	handler_type_arena,       // references the "ArenaHandler"

	handler_type_other
}handler_type;

/**
 * Enum Class: message_transmission
 * Purpose: Informs the TINstance (or whatever object is transmitting the message) how to identify the handler
 *  the message is intended for
 */
typedef enum class message_transmission
{
	message_transmission_firm_id,      // Search based solely on the ID of the Handler
	message_transmission_firm_name,    // Search based solely on the Name of the Handler
	message_transmission_id_over_name, // Search focuses on the ID first and the falls back upon the name
	message_transmission_name_over_id, // Search focuses on the name and falls back on the ID if Name is not matched
	message_transmission_by_type,      // Search based off of type of handler solely
	message_transmission_create_new,   // Create a New Handler
	message_transmission_name_type     // Search based off of the name and type of handler
};

/**
 * Class: HandlerMessage
 * Purpose: Allows Event Handlers to communicate between each other
 */
class _TAP_DLL HandlerMessage : public TObject
{
public:

	/**
	 * Method: HandlerMessage::HandlerMessage
	 * Purpose: Constructor
	 * Parameters: const TString& handlerName - name of the Handler
	 *				handler_type type - the type of handler
	 *				UINT otherType - the type of handler if the previous parameter is "generic" or "other"
	 *				message_transmission transmission - the means of choosing the Handler
	 *				UINT handlerIdTarget - the ID of the Handler (not used yet)
	 *				const TString& message - the actual Contents of the message
	 * Returns: New HandlerMessage Object
	 */
	HandlerMessage(const TString& handlerName, handler_type type, UINT otherType, message_transmission transmission, UINT handlerIdTarget, const TString& message);

	/**
	 * Method: HandlerMessage::GetHandlerName
	 * Purpose: Retrieves the Name of the handler that the message is intended for
	 * Parameters: void
	 * Returns: TString - the Name of the Handler to look for
	 */
	TString GetHandlerName();

	/**
	 * Method: HandlerMessage::GetMessage_
	 * Purpose: Retrieves the message of the Obejct
	 * Parameters: void
	 * Returns: TString - the contents of the message
	 *
	 * Note: the underscore was added to this method's name in order to avoid a naming conflict with Windows functions called "GetMessage"
	 */
	TString GetMessage_();

	/**
	 * Method: HandlerMessage::GetHandlerType
	 * Purpose: Gets the type of handler the message was intended for
	 * Parameters: void
	 * Returns: handler_type - the type of built-in handler
	 */
	handler_type GetHandlerType();
	/**
	 * Method: HandlerMessage::GetHandlerOtherType
	 * Purpose: Retrieves the type of handler, use for 3rd Party EventHandler types
	 * Parameters: void
	 * Returns: UINT - the ID of the Event Handler type (as specified by the Application)
	 */
	UINT GetHandlerOtherType();
	/**
	 * Method: HandlerMessage::GetHandlerIdType
	 * Purpose: Retirevs the ID of the Handler to target
	 * Parameters: void
	 * Returns: UINT - the Ide of the handler (not used yet)
	 */
	UINT GetHandlerIdType();
	/**
	 * Method: HandlerMessage::GetMessageTransmission
	 * Purpose: Get the requested means of identifying the Handler
	 * Parameters: void
	 * Returns: message_transmission - the means of identifying the handler
	 */
	message_transmission GetMessageTransmission();

private:
	/**
	 * The Name of the Handler to target
	 */
	TString handlerName;
	/**
	 * The Built in Handler type
	 */
	handler_type handlerType;
	/**
	 * The type of Handler (use for 3rd party Handlers)
	 */
	UINT handlerOtherType;
	/**
	 * the ID of the Target Handler
	 */
	UINT handlerIdTarget;
	/**
	 * the means to identify the target Handler
	 */
	message_transmission transmission;
	/**
	 * the Contents of the message
	 */
	TString message;
};

