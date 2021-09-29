#pragma once
#include "VideoGraphics.h"
#include <TObject.h>
#include <TrecReference.h>
#include <TDataArray.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#include <evr.h>
#include <TFileShell.h>
#include "DrawingBoard.h"

/**
 * Enum Class: PlayerState
 * Purpose: Sets up the states a player can be in
 */
enum class PlayerState
{
	ps_closed = 0,
	ps_ready,
	ps_opening,
	ps_started,
	ps_paused,
	ps_stopped,
	ps_closing
};

/**
 * Enum Class: player_node_type
 * Purpose: labels the node type
 */
typedef enum class player_node_type
{
	pnt_source, // IMFMediaSource
	pnt_transform, // TMFTransform
	pnt_sink
}player_node_type;

/**
 * Class: PlayerNode
 * Purpose: Holds a resource responsible for manipulating Samples, whether they are Sources, transforms, or Sinks
 */
class _VIDEO_GRAPHICS PlayerNode
{
public:

	PlayerNode(IMFMediaSource* source);
	PlayerNode(IMFTransform* transform);
	PlayerNode(IMFMediaSink* sink);
	~PlayerNode();

	void AttachOutput(TrecPointer<PlayerNode> node);
	IMFMediaSource* GetMediaSource();
	IMFTransform* GetMediaTransform();
	IMFMediaSink* GetMediaSink();
	player_node_type GetType();

private:
	player_node_type type;
	TDataArray<TrecPointer<PlayerNode>> output;

	// Node
	IMFMediaSource* source;
	IMFTransform* transform;
	IMFMediaSink* sink;
};

/**
 * Class: TPlayer2
 * Purpose: Represents a session/topology of a Video Player
 */
class _VIDEO_GRAPHICS TPlayer2 :
    public TObject
{
public:

	/**
	 * Method: TPlayer2::TPlayer2
	 * Purpose: Constructor
	 * Parameters: void
	 * Returns: new TPlayer2 object
	 */
	TPlayer2();

	/**
	 * Method: TPlayer2::SetSourceUrl
	 * Purpose: Sets up a Media source based off of the provided URL
	 * Parameters: const TString& url - the url of the resource to get a media source from
	 * Returns: bool - whether the operation was successful or not
	 */
	bool SetSourceUrl(const TString& url);

	/**
	 * Method: TPlayer2::SetSinkFile
	 * Purpose: Sets up a sink that writes to the specified file
	 * Parameters: TrecPointer<TFileShell> file - the file to write to
	 * Returns: bool - whether file was valid or not
	 */
	bool SetSinkFile(TrecPointer<TFileShell> file);

	/**
	 * Method: TPlayer2::SetBoardSink
	 * Purpose: Sets up a sink that writes to the drawing Board
	 * Parameters: TrecPointer<DrawingBoard> board - board to draw to
	 *				const D2D1_RECT_F& location - the area of the board to draw to
	 * Returns: bool - whether parameters were valid
	 */
	bool SetBoardSink(TrecPointer<DrawingBoard> board, const D2D1_RECT_F& location);

	/**
	 * Method: TPlayer2::PrepTopology
	 * Purpose: Sets up the Topology based off of the provided sources and sinks
	 * Parameters: UINT maxTransforms - the max number of transforms between a source or sink
	 * Returns: String - the results of the operation (empty means success)
	 */
	TString PrepTopology(UINT maxTransforms);

protected:

	TrecPointer<PlayerNode> source;

	TDataArray<TrecPointer<PlayerNode>> sinks;
};

