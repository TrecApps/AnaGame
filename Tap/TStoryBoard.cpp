#include "TStoryBoard.h"
#include "TWindow.h"

/**
 * Function: ProcessAnimations
 * Purpose: Function in the Animation Thread that runs the Animations
 * Parameters: LPVOID param -  the data needed to run the thread (a ThreadData object)
 * Returns: DWORD - error code
 */
DWORD __stdcall ProcessAnimations(LPVOID param)
{
	ThreadData* data = reinterpret_cast<ThreadData*>(param);
	if (!data)
		return 1;

	float progress = 0.0f;

	do
	{
		if (progress >= 1.0f)
			progress = 0.0f;
		for (UINT c = 0; c < data->animations.Size(); c++)
			if(data->animations[c].Get())
				data->animations[c]->Update(progress);

		if (data->win.Get())
			TrecPointerKey::GetTrecPointerFromSoft<TWindow>(data->win)->InduceDraw();

		progress += (static_cast<float>(data->shortestInterval) / static_cast<float>(data->longestAni));

		// To-do Add Sleep
		Sleep(data->shortestInterval);
		
	} while (data->win.Get() && (data->persist ? true : progress < 0.999f));

	data->animationThread = 0;
	return 0;
}

/**
 * Method: TStoryBoard::TStoryBoard
 * Purpose: Default Constructor
 * Parameters: void
 * Returns: New Story Board Object
 */
TStoryBoard::TStoryBoard()
{
	threadData.animationThread = 0;
	threadData.longestAni = 0;
	threadData.persist = false;
	threadData.shortestInterval = 60000;
}

/**
 * Method: TStoryBoard::~TStoryBoard
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TStoryBoard::~TStoryBoard()
{
	Terminate();
}

/**
 * Method: TStoryBoard::AddAnimation
 * Purpose: Adds an Animation to the Story Board
 * Parameters: TrecPointer<Animation> a -  the Animation to add
 * Returns: bool - true if the Animation was added (false if null or the thread was actively running)
 */
bool TStoryBoard::AddAnimation(TrecPointer<Animation> a)
{
	if (!a.Get())
		return false;

	// Don't add an animation if an animation thread is underway
	if (threadData.animationThread)
		return false;

	if (threadData.longestAni < a->GetMillisecondLength())
		threadData.longestAni = a->GetMillisecondLength();

	if (threadData.shortestInterval > a->GetMillisecondRefresh())
		threadData.shortestInterval = a->GetMillisecondRefresh();

	for (UINT c = 0; c < threadData.animations.Size(); c++)
	{
		threadData.animations[c]->SetMillisecindStoryLength(threadData.longestAni);
	}

	threadData.animations.push_back(a);
	return true;
}

/**
 * Method: TStoryBoard::SetWindow
 * Purpose: Identifies the Window the Story board is working on
 * Parameters: TrecPointer<TWindow> w - The Window to associate the Story Board with
 * Returns: void
 */
void TStoryBoard::SetWindow(TrecPointer<TWindow> w)
{
	if (w.Get())
		threadData.win = TrecPointerKey::GetSoftPointerFromTrec<TWindow>(w);
}

/**
 * Method: TStoryBoard::SetPersistant
 * Purpose: Marks the Story Board as persistent (meaning rather than stop, it will run in a loop)
 * Parameters: void
 * Returns: void
 */
void TStoryBoard::SetPersistant()
{
	threadData.persist = true;
}

/**
 * Method: TStoryBoard::IsPersistant
 * Purpose: reports whether the Story Board is persistent or not
 * Parameters: void
 * Returns: bool - whether the Story Board is persistent
 */
bool TStoryBoard::IsPersistant()
{
	return threadData.persist;
}

/**
 * Method: TStoryBoard::Run
 * Purpose: Starts the Animation thread
 * Parameters: void
 * Returns: bool - whether a thread was created or not
 */
bool TStoryBoard::Run()
{
	if (threadData.animationThread)
		return false;

	threadData.animationThread = CreateThread(nullptr,
		0,
		ProcessAnimations,
		&threadData,
		0,
		&threadID);

	return threadData.animationThread != 0;
}

/**
 * Method: TStoryBoard::Pause
 * Purpose: Pauses the running thread
 * Parameters: void
 * Returns: bool - whether there was a thread to pause
 */
bool TStoryBoard::Pause()
{
	if (threadData.animationThread)
	{
		SuspendThread(threadData.animationThread);
		return true;
	}
	return false;
}

/**
 * Method: TStoryBoard::Resume
 * Purpose: Resumes the thread if paused
 * Parameters: void
 * Returns: void
 */
void TStoryBoard::Resume()
{
	if (threadData.animationThread)
		ResumeThread(threadData.animationThread);
}


/**
 * Method: TStoryBoard::Terminate
 * Purpose: Terminates the thread
 * Parameters: void
 * Returns: void
 */
void TStoryBoard::Terminate()
{
	// TO-DO: Research safer way to terminate thread

	if (threadData.animationThread)
		TerminateThread(threadData.animationThread, 0);
	threadData.animationThread = 0;
}

/**
 * Method: TStoryBoard::Empty
 * Purpose: Empties the Story Board of Animtions
 * Parameters: void
 * Returns: void
 */
void TStoryBoard::Empty()
{
	threadData.animations.RemoveAll();
}
