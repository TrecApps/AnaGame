#pragma once
#include "TObject.h"
#include "TrecLib.h"

/**
 * Class: TPoint
 * Purpose: Basic class holding information about a 2D point
 * 
 * SuperClass: TObject - allows it to be used by Environment Scripts
 */
class _TREC_LIB_DLL TPoint :
	public TObject
{
public:
	/**
	 * x: the x-coordinate
	 * y: the y-coordinate
	 */
	float x, y;


	/**
	 * Method: TPoint::GetType
	 * Purpose: Returns a String Representation of the object type
	 * Parameters: void
	 * Returns: TString - representation of the object type
	 */
	virtual TString GetType()override;

	/**
	 * Method: TPoint::TPoint
	 * Purpose: Default constructor
	 * Parameters: void
	 * Returns: New TPoint Object
	 */
	TPoint();

	/**
	 * Method: TPoint::TPoint
	 * Purpose: Value Constructor
	 * Parameters: float x - x-coordinate of the point
	 *				float y - ycoordinae of the point
	 * Returns: New TPoint Object
	 */
	TPoint(float x, float y);

	/**
	 * Method: TPoint::TPoint
	 * Purpose: Copy Contructor
	 * Parameters:const TPoint& point - reference to the Point to copy from
	 * Returns: New TPoint Object
	 */
	TPoint(const TPoint& point);

	/**
	 * Method: TPoint::TPoint
	 * Purpose:
	 * Parameters: const TPoint* point - pointer to the point to copy from
	 * Returns: New TPOint object
	 */
	TPoint(const TPoint* point);

	/**
	 * Method: TPoint::operator=
	 * Purpose: Allows code to use the = operator on this class
	 * Parameters: const TPoint& point - the point to copy from
	 * Returns:
	 */
	TPoint operator=(const TPoint& point);
};

