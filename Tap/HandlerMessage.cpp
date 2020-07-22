#include "HandlerMessage.h"

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
HandlerMessage::HandlerMessage(const TString& handlerName, handler_type type, UINT otherType, message_transmission transmission, UINT handlerIdTarget, const TString& message)
{
	this->handlerName.Set(handlerName);
	this->handlerType = type;
	this->handlerOtherType = otherType;
	this->transmission = transmission;
	this->handlerIdTarget = handlerIdTarget;
	this->message.Set(message);
}

/**
 * Method: HandlerMessage::GetHandlerName
 * Purpose: Retrieves the Name of the handler that the message is intended for
 * Parameters: void
 * Returns: TString - the Name of the Handler to look for
 */
TString HandlerMessage::GetHandlerName()
{
	return handlerName;
}

/**
 * Method: HandlerMessage::GetMessage_
 * Purpose: Retrieves the message of the Obejct
 * Parameters: void
 * Returns: TString - the contents of the message
 *
 * Note: the underscore was added to this method's name in order to avoid a naming conflict with Windows functions called "GetMessage"
 */
TString HandlerMessage::GetMessage_()
{
	return message;
}

/**
 * Method: HandlerMessage::GetHandlerType
 * Purpose: Gets the type of handler the message was intended for
 * Parameters: void
 * Returns: handler_type - the type of built-in handler
 */
handler_type HandlerMessage::GetHandlerType()
{
	return handlerType;
}

/**
 * Method: HandlerMessage::GetHandlerOtherType
 * Purpose: Retrieves the type of handler, use for 3rd Party EventHandler types
 * Parameters: void
 * Returns: UINT - the ID of the Event Handler type (as specified by the Application)
 */
UINT HandlerMessage::GetHandlerOtherType()
{
	return handlerOtherType;
}

/**
 * Method: HandlerMessage::GetHandlerIdType
 * Purpose: Retirevs the ID of the Handler to target
 * Parameters: void
 * Returns: UINT - the Ide of the handler (not used yet)
 */
UINT HandlerMessage::GetHandlerIdType()
{
	return handlerIdTarget;
}

/**
 * Method: HandlerMessage::GetMessageTransmission
 * Purpose: Get the requested means of identifying the Handler
 * Parameters: void
 * Returns: message_transmission - the means of identifying the handler
 */
message_transmission HandlerMessage::GetMessageTransmission()
{
	return transmission;
}
