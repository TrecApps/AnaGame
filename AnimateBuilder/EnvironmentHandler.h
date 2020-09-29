#pragma once
#include <EventHandler.h>
#include <ShlObj_core.h>
#include <TEnvironment.h>

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

class EnvironmentHandler;

typedef void (EnvironmentHandler::* EnvironmentHandlerEvents)(TrecPointer<TControl> tc, EventArgs ea);

typedef enum class environment_handler_mode
{
	ehm_not_set,
	ehm_recent_set,
	ehm_available_set,
	ehm_import_set
}environment_handler_mode;

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

	/**
	 * Method: EnvironmentHandler::GetEnvironment
	 * Purpose: PRetrieves the environment procured byt the Dialog
	 * Parameters: void
	 * Returns: TrecPointer<TEnvironment> - the environment selected by the user
	 */
	TrecPointer<TEnvironment> GetEnvironment();

protected:

	void OnSelectRecent(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectAvailable(TrecPointer<TControl> tc, EventArgs ea);
	void OnImportProject(TrecPointer<TControl> tc, EventArgs ea);
	void OnSelectWorkspace(TrecPointer<TControl> tc, EventArgs ea);
	void OnConfirm(TrecPointer<TControl> tc, EventArgs ea);
	void OnFileType(TrecPointer<TControl> tc, EventArgs ea);

	/**
	 * Method: EnvironmentHandler::ShouldProcessMessageByType
	 * Purpose: Reports whether this Object is of the correct type to recieve the message
	 * Parameters: TrecPointer<HandlerMessage> message - the message to scan
	 * Returns: bool - true if the type matches, false oherwise
	 */
	virtual bool ShouldProcessMessageByType(TrecPointer<HandlerMessage> message);

	void RefreshView();

	void RefreshEnvironmentList();

	PIDLIST_ABSOLUTE BrowseFolder(const TString& message);


	TrecSubPointer<TControl, TDataBind> recentBinder, newBinder;

	TTrecPointerArray<EnvironmentList> availableEnvironments;

	TDataArray< EnvironmentHandlerEvents> envEvents;

	TrecPointer<EnvironmentList> selectedEnvType;
	TrecSubPointer<TControl, TLayout> grid;

	TrecPointer<TControl> confirmButton;

	TrecSubPointer<TControl, TTextField> selectReport, builderReport, environmentReport, directoryReport, nameEntry;

	TrecPointer<TFileShell> currentWorkspace;

	environment_handler_mode mode;

	TrecPointer<TWindow> window;

	TrecPointer<TEnvironment> environment;
};

