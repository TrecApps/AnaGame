#pragma once
#include "TObject.h"
#include <Animation.h>
#include <TTrecPointerArray.h>
#include "Tap_dll.h"

class TWindow;

/**
 * Class: ThreadData
 * Purpose: Holds information needed by the Animation threads created by the StoryBoard
 */
class ThreadData
{
public:
	/**
	 * List of animations to run through
	 */
	TTrecPointerArray<Animation> animations;

	/**
	 * whether to maintain the loop
	 */
	bool persist;

	/**
	 * dictates the refresh rate and length of the Animation
	 */
	UINT shortestInterval, longestAni;
	/**
	 * the Window to Draw
	 */
	TrecPointer<TWindow> win;
	/**
	 * Handle to the thread
	 */
	HANDLE animationThread;
	
};

/**
 * Class: TStoryBoard
 * Purpose: Holds a collection of Animations and manages a Thread running through them
 */
class _TAP_DLL TStoryBoard :
	public TObject
{
	friend class TAnimationManager;
public:

	/**
	 * Method: TStoryBoard::TStoryBoard
	 * Purpose: Default Constructor
	 * Parameters: void
	 * Returns: New Story Board Object
	 */
	TStoryBoard();

	/**
	 * Method: TStoryBoard::~TStoryBoard
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	~TStoryBoard();

	/**
	 * Method: TStoryBoard::AddAnimation
	 * Purpose: Adds an Animation to the Story Board
	 * Parameters: TrecPointer<Animation> a -  the Animation to add
	 * Returns: bool - true if the Animation was added (false if null or the thread was actively running)
	 */
	bool AddAnimation(TrecPointer<Animation> a);

	/**
	 * Method: TStoryBoard::SetWindow
	 * Purpose: Identifies the Window the Story board is working on
	 * Parameters: TrecPointer<TWindow> w - The Window to associate the Story Board with
	 * Returns: void
	 */
	void SetWindow(TrecPointer<TWindow> w);

	

	/**
	 * Method: TStoryBoard::SetPersistant
	 * Purpose: Marks the Story Board as persistent (meaning rather than stop, it will run in a loop)
	 * Parameters: void
	 * Returns: void
	 */
	void SetPersistant();

	/**
	 * Method: TStoryBoard::IsPersistant
	 * Purpose: reports whether the Story Board is persistent or not
	 * Parameters: void
	 * Returns: bool - whether the Story Board is persistent
	 */
	bool IsPersistant();


	/**
	 * Method: TStoryBoard::Run
	 * Purpose: Starts the Animation thread
	 * Parameters: void
	 * Returns: bool - whether a thread was created or not
	 */
	bool Run();

	/**
	 * Method: TStoryBoard::Pause
	 * Purpose: Pauses the running thread
	 * Parameters: void 
	 * Returns: bool - whether there was a thread to pause
	 */
	bool Pause();

	/**
	 * Method: TStoryBoard::Resume
	 * Purpose: Resumes the thread if paused
	 * Parameters: void
	 * Returns: void
	 */
	void Resume();

	/**
	 * Method: TStoryBoard::Terminate
	 * Purpose: Terminates the thread
	 * Parameters: void
	 * Returns: void
	 */
	void Terminate();

protected:
	/**
	 * Data to send to the Thread function
	 */
	ThreadData threadData;
	
	/**
	 * ID of the thread created
	 */
	DWORD threadID;

	/**
	 * Method: TStoryBoard::Empty
	 * Purpose: Empties the Story Board of Animtions
	 * Parameters: void
	 * Returns: void
	 */
	void Empty();
};

/**
 * Function: ProcessAnimations
 * Purpose: Function in the Animation Thread that runs the Animations
 * Parameters: LPVOID param -  the data needed to run the thread (a ThreadData object)
 * Returns: DWORD - error code
 */
DWORD WINAPI ProcessAnimations(LPVOID param);