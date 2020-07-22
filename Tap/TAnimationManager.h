#pragma once
#include <TObject.h>


#include "TStoryBoard.h"
#include <TMap.h>
class TWindow;

/**
 * Class: TAnimationManager
 * Purpose: Holds a collection of Story Boards and controls their process
 */
class _TAP_DLL TAnimationManager :public TObject
{
public:

	/**
	 * Method: TAnimationManager::TAnimationManager
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: new TAnimation Manager
	 */
	TAnimationManager();

	/**
	 * Method: TAnimationManager::~TAnimationManager
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TAnimationManager();


	/**
	 * Method: TAnimationManager::SetWindow
	 * Purpose: Sets the Window associated with this Animation Manager
	 * Parameters: TrecPointer<TWindow> win -  the Window that created this manager
	 * Returns: void
	 */
	void SetWindow(TrecPointer<TWindow> win);


	/**
	 * Method: TAnimationManager::AddAnimationBegin
	 * Purpose: Add an Animation to the "Begin" Story Board
	 * Parameters: TrecPointer<Animation> a - the animation to add
	 * Returns: void
	 */
	void AddAnimationBegin(TrecPointer<Animation> a);

	/**
	 * Method: TAnimationManager::AddAnimationEnd
	 * Purpose: Add an Animation to the "End" Story Board
	 * Parameters: TrecPointer<Animation> a - the Animation to Add 
	 * Returns: void
	 */
	void AddAnimationEnd(TrecPointer<Animation> a);


	/**
	 * Method: TAnimationManager::AddStoryBoard
	 * Purpose: Adds a New Story Board to the Manager
	 * Parameters: TString& name - the Name of the Story Board to Add
	 *				TrecPointer<TStoryBoard> stpry - the Story to Add
	 * Returns: void
	 */
	void AddStoryBoard(TString& name, TrecPointer<TStoryBoard> stpry);
	

	/**
	 * Method: TAnimationManager::CleanBegin
	 * Purpose: Clears the "Begin" Story board of Animations
	 * Parameters: void
	 * Returns: void
	 */
	void CleanBegin();


	/**
	 * Method: TAnimationManager::StartBegin
	 * Purpose: Starts the Begin Story Board
	 * Parameters: void
	 * Returns: void 
	 */
	void StartBegin();

	/**
	 * Method: TAnimationManager::StartEnd
	 * Purpose: Starts the End Story Board
	 * Parameters: void
	 * Returns: void 
	 */
	void StartEnd();

	/**
	 * Method: TAnimationManager::StartStory
	 * Purpose: Starts the specified Story Board
	 * Parameters:  TString& name - the Name of the Story Board to Start
	 * Returns: void
	 */
	void StartStory(TString& name);

	/**
	 * Method: TAnimationManager::StartNewPersistant
	 * Purpose: Starts All StoryBoards considered "Persistent"
	 * Parameters: void
	 * Returns: void
	 */
	void StartNewPersistant();


	/**
	 * Method: TAnimationManager::StopStory
	 * Purpose: Stops the specified Story Board
	 * Parameters:  TString& name - the Name of the Story Board to Stop
	 * Returns: void
	 */
	void StopStory(TString& name);

	/**
	 * Method: TAnimationManager::Stop
	 * Purpose:  Stops all Story Boards
	 * Parameters: void
	 * Returns: void
	 */
	void Stop();


	/**
	 * Method: TAnimationManager::PauseStory
	 * Purpose: Pauses the specified Story Board
	 * Parameters:  TString& name - the Name of the Story Board to Pause
	 * Returns: void
	 */
	void PauseStory(TString& name);

	/**
	 * Method: TAnimationManager::Pause
	 * Purpose: Pauses all Story Boards
	 * Parameters: void
	 * Returns: void
	 */
	void Pause();


	/**
	 * Method: TAnimationManager::ResumeStory
	 * Purpose: Resumes the specified Story Board
	 * Parameters: TString& name - the Name of the Story Board to Resume
	 * Returns: void
	 */
	void ResumeStory(TString& name);

	/**
	 * Method: TAnimationManager::Resume
	 * Purpose: Resumes all StoryBoards
	 * Parameters: void
	 * Returns: void
	 */
	void Resume();
protected:
	/**
	 * Built-in Story Boards 
	 */
	TStoryBoard begin, end;

	/**
	 * Reports whether the built-in storyBoards are paused or not
	 */
	bool beginPaused, endPaused;

	/**
	 * Holds a list of Story Boards
	 */
	TMap<TStoryBoard> stories;

	/**
	 * Points to Story Boards that are Persistent
	 */
	TTrecPointerArray<TStoryBoard> newPersistantStoryBoards;
	
	/**
	 * List of Stories that are paused
	 */
	TTrecPointerArray<TStoryBoard> pausedStories;
	
	/**
	 * The Window holding the Manager
	 */
	TrecPointer<TWindow> window;
};

