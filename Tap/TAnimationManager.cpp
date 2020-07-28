#include "TAnimationManager.h"

/**
 * Method: TAnimationManager::TAnimationManager
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: new TAnimation Manager
 */
TAnimationManager::TAnimationManager()
{
	beginPaused = endPaused = false;
}

/**
 * Method: TAnimationManager::~TAnimationManager
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TAnimationManager::~TAnimationManager()
{
	begin.Terminate();
	end.Terminate();
}

/**
 * Method: TAnimationManager::SetWindow
 * Purpose: Sets the Window associated with this Animation Manager
 * Parameters: TrecPointer<TWindow> win -  the Window that created this manager
 * Returns: void
 */
void TAnimationManager::SetWindow(TrecPointer<TWindow> win)
{
	window = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(win);
	begin.SetWindow(win);
	end.SetWindow(win);
}

/**
 * Method: TAnimationManager::AddAnimationBegin
 * Purpose: Add an Animation to the "Begin" Story Board
 * Parameters: TrecPointer<Animation> a - the animation to add
 * Returns: void
 */
void TAnimationManager::AddAnimationBegin(TrecPointer<Animation> a)
{
	begin.AddAnimation(a);
}

/**
 * Method: TAnimationManager::AddAnimationEnd
 * Purpose: Add an Animation to the "End" Story Board
 * Parameters: TrecPointer<Animation> a - the Animation to Add
 * Returns: void
 */
void TAnimationManager::AddAnimationEnd(TrecPointer<Animation> a)
{
	end.AddAnimation(a);
}

/**
 * Method: TAnimationManager::AddStoryBoard
 * Purpose: Adds a New Story Board to the Manager
 * Parameters: TString& name - the Name of the Story Board to Add
 *				TrecPointer<TStoryBoard> stpry - the Story to Add
 * Returns: void
 */
void TAnimationManager::AddStoryBoard(TString& name, TrecPointer<TStoryBoard> story)
{
	if (name.GetSize() && story.Get())
	{
		stories.addEntry(name, story);
		if (story->IsPersistant())
			newPersistantStoryBoards.push_back(story);
	}

}

/**
 * Method: TAnimationManager::CleanBegin
 * Purpose: Clears the "Begin" Story board of Animations
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::CleanBegin()
{
	begin.Terminate();
	begin.Empty();
}

/**
 * Method: TAnimationManager::StartBegin
 * Purpose: Starts the Begin Story Board
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::StartBegin()
{
	begin.Run();
}

/**
 * Method: TAnimationManager::StartEnd
 * Purpose: Starts the End Story Board
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::StartEnd()
{
	end.Run();
}

/**
 * Method: TAnimationManager::StartStory
 * Purpose: Starts the specified Story Board
 * Parameters:  TString& name - the Name of the Story Board to Start
 * Returns: void
 */
void TAnimationManager::StartStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (story.Get())
		story->Run();
}

/**
 * Method: TAnimationManager::StartNewPersistant
 * Purpose: Starts All StoryBoards considered "Persistent"
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::StartNewPersistant()
{
	for (UINT Rust = 0; Rust < newPersistantStoryBoards.Size(); Rust++)
	{
		if (newPersistantStoryBoards[Rust].Get())
			newPersistantStoryBoards[Rust]->Run();
	}
	newPersistantStoryBoards.RemoveAll();
}

/**
 * Method: TAnimationManager::StopStory
 * Purpose: Stops the specified Story Board
 * Parameters:  TString& name - the Name of the Story Board to Stop
 * Returns: void
 */
void TAnimationManager::StopStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (story.Get())
		story->Terminate();
}

/**
 * Method: TAnimationManager::Stop
 * Purpose:  Stops all Story Boards
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::Stop()
{
	begin.Terminate();
	end.Terminate();

	for (UINT Rust = 0; Rust < stories.count(); Rust++)
	{
		TrecPointer<TStoryBoard> story = stories.GetEntryAt(Rust)->object;
		if (story.Get())
			story->Terminate();
	}
}

/**
 * Method: TAnimationManager::PauseStory
 * Purpose: Pauses the specified Story Board
 * Parameters:  TString& name - the Name of the Story Board to Pause
 * Returns: void
 */
void TAnimationManager::PauseStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (story.Get() && story->Pause())
		pausedStories.push_back(story);
}

/**
 * Method: TAnimationManager::Pause
 * Purpose: Pauses all Story Boards
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::Pause()
{
	beginPaused = begin.Pause();
	endPaused = end.Pause();

	for (UINT Rust = 0; Rust < stories.count(); Rust++)
	{
		TrecPointer<TStoryBoard> story = stories.GetEntryAt(Rust)->object;
		if (story.Get() && story->Pause())
			pausedStories.push_back(story);
	}
}

/**
 * Method: TAnimationManager::ResumeStory
 * Purpose: Resumes the specified Story Board
 * Parameters: TString& name - the Name of the Story Board to Resume
 * Returns: void
 */
void TAnimationManager::ResumeStory(TString& name)
{
	TrecPointer<TStoryBoard> story = stories.retrieveEntry(name);

	if (!story.Get())
		return;
	story->Resume();

	for (UINT Rust = 0; Rust < pausedStories.Size(); Rust++)
	{
		if (pausedStories[Rust].Get() == story.Get())
		{
			pausedStories.RemoveAt(Rust);
			return;
		}
	}
}

/**
 * Method: TAnimationManager::Resume
 * Purpose: Resumes all StoryBoards
 * Parameters: void
 * Returns: void
 */
void TAnimationManager::Resume()
{
	if (endPaused)
	{
		endPaused = false;
		end.Resume();
	}
	if (beginPaused)
	{
		beginPaused = false;
		begin.Resume();
	}

	for (UINT Rust = 0; Rust < pausedStories.Size(); Rust++)
	{
		pausedStories[Rust]->Resume();
	}
	pausedStories.RemoveAll();
}
