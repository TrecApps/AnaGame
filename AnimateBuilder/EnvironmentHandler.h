#pragma once
#include <EventHandler.h>

class EnvironmentList : public TObject
{
public: 
	EnvironmentList(const TString& environment, const TString& builder);
	EnvironmentList(const EnvironmentList& list);
	EnvironmentList();

	virtual TString getVariableValueStr(const TString& varName)override;

	TString GetEnvironment()const;
	TString GetBuilder() const;

private:
	TString environment, environmentBuilder;
};


/**
 * Class: EnvironmentHandler
 * Purpose: Extends the EventHandler to handle Environment Dialog events
 */
class EnvironmentHandler :
    public EventHandler
{
public:
	/**
	 * Method: EnvironmentHandler::EnvironmentHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
	 * Returns: new EnvironmentHandler Object
	 */
	EnvironmentHandler(TrecPointer<TInstance> instance);

	/**
	 * Method: EnvironmentHandler::~EnvironmentHandler
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~EnvironmentHandler();


	/**
	 * Method: EnvironmentHandler::Initialize
	 * Purpose: Environment Handler initialization
	 * Parameters: TrecPointer<Page> page - the page the Handler is to associate with
	 * Returns: void
	 */
	virtual void Initialize(TrecPointer<Page> page) override;

	/**
	 * Method: EnvironmentHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<EventID_Cred>& eventAr)override;

	/**
	 * Method: EnvironmentHandler::ProcessMessage
	 * Purpose: Processes the message sent to the handler
	 * Parameters: TrecPointer<HandlerMessage> message - the message to recieve and Process
	 * Returns: void
	 */
	virtual void ProcessMessage(TrecPointer<HandlerMessage> message)override;

protected:
	/**
	 * Method: EnvironmentHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message);

	TrecSubPointer<TControl, TDataBind> recentBinder, newBinder;

	TTrecPointerArray<EnvironmentList> availableEnvironments;
};

