#pragma once
#include <EventHandler.h>
#include <ShlObj_core.h>
#include <TEnvironment.h>
#include <TDataLayout.h>
#include <TLayout.h>
#include <TTextInput.h>
#include <TContainerVariable.h>

class EnvironmentList : public TObject
{
public: 
	EnvironmentList(const TString& environment, const TString& builder);
	EnvironmentList(const EnvironmentList& list);
	EnvironmentList();

	virtual TString getVariableValueStr(const TString& varName)override;

	TString GetEnvironment()const;
	TString GetBuilder() const;

protected:
	TString environment, environmentBuilder;
};

class SavedEnvironment : public EnvironmentList
{
public: 
	SavedEnvironment(const TString& environment, const TString& builder, const TString& name, TrecPointer<TFileShell> fileLoc);
	SavedEnvironment(const SavedEnvironment& list);
	SavedEnvironment();

	virtual TString getVariableValueStr(const TString& varName)override;

	TrecPointer<TFileShell> GetFileLocation();
	TString GetName();

private:
	TrecPointer<TFileShell> fileLoc;
	TString name;
};

class EnvironmentHandler;

typedef void (EnvironmentHandler::* EnvironmentHandlerEvents)(TrecPointer<TPage> tc, EventArgs ea);

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
    public TapEventHandler
{
public:
	/**
	 * Method: EnvironmentHandler::EnvironmentHandler
	 * Purpose: Constructor
	 * Parameters: TrecPointer<TInstance> instance - the instance associated with this Handler
	 * Returns: new EnvironmentHandler Object
	 */
	EnvironmentHandler(TrecPointer<TProcess> instance);

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
	virtual void Initialize(TrecPointer<TPage> page) override;

	/**
	 * Method: EnvironmentHandler::HandleEvents
	 * Purpose: Handles Events produced from the set of TControls
	 * Parameters: TDataArray<EventID_Cred>& eventAr - list of events to process
	 * Returns: void
	 */
	virtual void HandleEvents(TDataArray<TPage::EventID_Cred>& eventAr)override;

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

	TDataMap<UINT> events;

	void OnSelectRecent(TrecPointer<TPage> tc, EventArgs ea);
	void OnSelectAvailable(TrecPointer<TPage> tc, EventArgs ea);
	void OnImportProject(TrecPointer<TPage> tc, EventArgs ea);
	void OnSelectWorkspace(TrecPointer<TPage> tc, EventArgs ea);
	void OnConfirm(TrecPointer<TPage> tc, EventArgs ea);
	void OnFileType(TrecPointer<TPage> tc, EventArgs ea);

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


	TrecSubPointer<TPage, TDataLayout> recentBinder, newBinder;

	TrecSubPointer<TVariable, TContainerVariable> availableEnvironments;
	TrecSubPointer<TVariable, TContainerVariable> savedEnvironments;

	TDataArray< EnvironmentHandlerEvents> envEvents;

	TrecPointer<EnvironmentList> selectedEnvType;
	TrecSubPointer<TPage, TLayout> grid;

	TrecPointer<TPage> confirmButton;

	TrecSubPointer<TPage, TTextInput> selectReport, builderReport, environmentReport, directoryReport, nameEntry;

	TrecPointer<TFileShell> currentWorkspace;

	environment_handler_mode mode;


	TrecPointer<TEnvironment> environment;


	// For Saved Environments
	TrecPointer<TFileShell> envFile;
};

