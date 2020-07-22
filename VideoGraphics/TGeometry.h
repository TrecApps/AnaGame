#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include <TDataArray.h>
#include "Structure2D.h"

/**
 * enum geo_type
 * Purpose: Helps the TGeomatry object track how it was created
 */
typedef enum class geo_type
{
	geo_type_rect,
	geo_type_rounded_rect,
	geo_type_ellipse,
	geo_type_path
}geo_type;

/**
 * class TGeometry
 * Purpose: Helps simplifiy the management of geometries in 2D drawing
 */
class _VIDEO_GRAPHICS TGeometry :
	public TObject
{
public:

	/**
	 * Method: TGeometry::TGeometry
	 * Purpose: Creates a Geometry from a set of points
	 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
					const TDataArray<POINT_2D>& points -  - the set of points to create the geometry from
	 * Returns: New TGeometry of a series of points
	 */
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const TDataArray<POINT_2D>& points);

	/**
	 * Method: TGeometry::TGeometry
	 * Purpose: Creates a Geometry from a rectangle
	 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
	 *				const RECT_2D& r - the rectangle to create the geometry from
	 * Returns:  New TGeometry of a rect
	 */
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const RECT_2D& r);

	/**
	 * Method: TGeometry::TGeometry
	 * Purpose: Creates a rounded rectangle from a set of points
	 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
	 *				const ROUNDED_RECT_2D& r - the rounded rectangle to create the geometry from
	 * Returns:  New TGeometry of a rounded round
	 */
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const ROUNDED_RECT_2D& r);

	/**
	 * Method: TGeometry::TGeometry
	 * Purpose: Creates a ellipse from a set of points
	 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
	 *				const ELLIPSE_2D& r - the ellipse to create the geometry from
	 * Returns: New TGeometry of an ellipse
	 */
	TGeometry(TrecComPointer<ID2D1Factory1> fact, const ELLIPSE_2D& r);


	/**
	 * Method: TGeometry::~TGeometry
	 * Purpose: Destructor
	 * Parameters: void
	 * Returns: void
	 */
	virtual ~TGeometry();


	/**
	 * Method: TGeometry::IsValid
	 * Purpose: Reports whether Geomatry construction was successful
	 * Parameters: void
	 * Returns:bool - whether the object is ready for use
	 */
	bool IsValid()const;


	/**
	 * Method: TGeometry::GetArea
	 * Purpose: Reports the Area
	 * Parameters: float& res - reference to the float that will hold the area of the geometry
	 * Returns:bool - whether the operation was successful
	 */
	bool GetArea(float& res);

	/**
	 * Method: TGeometry::GetParameter
	 * Purpose: Reports the Parameter
	 * Parameters: float& res - reference to the float that will hold the parameter of the geometry
	 * Returns:bool - whether the operation was successful
	 */
	bool GetParameter(float& res);

	/**
	 * Method: TGeometry::IsInside
	 * Purpose: Whether the point falls within the geomatry
	 * Parameters: BOOL& res - reference to whether the point is inide or not
	 *				POINT_2D& - the point to chack
	 * Returns:bool - whether the operation was successful
	 */
	bool IsInside(BOOL& res, POINT_2D&);

	/**
	 * Method: TGeometry::GetBounds
	 * Purpose: Gets the boundaries of the geomatry in rectangle form
	 * Parameters: RECT_2D& bounds - reference to hold the boundaries of the geometry
	 * Returns:bool - whether the operation was successful
	 */
	bool GetBounds(RECT_2D& bounds);

	/**
	 * Method: TGeometry::GetUnderlyingGeometry
	 * Purpose: Allows access to the underlying Geometry
	 * Parameters: void
	 * Returns: TrecComPointer<ID2D1Geometry> - the underlying geometry
	 */
	TrecComPointer<ID2D1Geometry> GetUnderlyingGeometry();

protected:
	/**
	 * the unerlying Geometry data
	 */
	TrecComPointer<ID2D1Geometry> geo;

	/**
	 * whether creation of the object was successful
	 */
	bool valid;
	/**
	 * the type of geometry this object holds
	 */
	geo_type geoType;
};

