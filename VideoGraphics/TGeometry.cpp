#include "TGeometry.h"

/**
 * Method: TGeometry::TGeometry
 * Purpose: Creates a Geometry from a set of points
 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
				const TDataArray<POINT_2D>& points -  - the set of points to create the geometry from
 * Returns: New TGeometry of a series of points
 */
TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, const TDataArray<POINT_2D>& points)
{
	valid = false;
	if (!fact.Get())
		return;
	   
	TrecComPointer<ID2D1PathGeometry>::TrecComHolder pathHolder;
	fact->CreatePathGeometry(pathHolder.GetPointerAddress());

	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1PathGeometry>(pathHolder);


	ID2D1GeometrySink* sink = nullptr;

	if (FAILED(reinterpret_cast<ID2D1PathGeometry*>(geo.Get())->Open(&sink)) || points.Size() < 2)
		return;

	sink->BeginFigure(points.data()[0], D2D1_FIGURE_BEGIN_FILLED);

	sink->AddLines(&(points.data()[1]), points.Size() - 1);

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);

	sink->Close();

	sink->Release();
	valid = true;
	//geoType = geo_type_path;
}

/**
 * Method: TGeometry::TGeometry
 * Purpose: Creates a Geometry from a rectangle
 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
 *				const RECT_2D& r - the rectangle to create the geometry from
 * Returns:  New TGeometry of a rect
 */
TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, const RECT_2D& r)
{
	valid = false;
	if (!fact.Get())
		return;

	TrecComPointer<ID2D1RectangleGeometry>::TrecComHolder rectHolder;
	fact->CreateRectangleGeometry(r, rectHolder.GetPointerAddress());

	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1RectangleGeometry>(rectHolder);
	valid = true;
	//geoType = geo_type_rect;
}

/**
 * Method: TGeometry::TGeometry
 * Purpose: Creates a rounded rectangle from a set of points
 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
 *				const ROUNDED_RECT_2D& r - the rounded rectangle to create the geometry from
 * Returns:  New TGeometry of a rounded round
 */
TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, const ROUNDED_RECT_2D& r)
{
	valid = false;
	if (!fact.Get())
		return;

	TrecComPointer<ID2D1RoundedRectangleGeometry>::TrecComHolder rectHolder;
	fact->CreateRoundedRectangleGeometry(r, rectHolder.GetPointerAddress());

	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1RoundedRectangleGeometry>(rectHolder);
	valid = true;
	//geoType
}

/**
 * Method: TGeometry::TGeometry
 * Purpose: Creates a ellipse from a set of points
 * Parameters: TrecComPointer<ID2D1Factory1> fact - used to create the geometry
 *				const ELLIPSE_2D& r - the ellipse to create the geometry from
 * Returns: New TGeometry of an ellipse
 */
TGeometry::TGeometry(TrecComPointer<ID2D1Factory1> fact, const ELLIPSE_2D& r)
{
	valid = false;
	if (!fact.Get())
		return;

	TrecComPointer<ID2D1EllipseGeometry>::TrecComHolder circleHolder;

	fact->CreateEllipseGeometry(r, circleHolder.GetPointerAddress());
	geo = TrecPointerKey::GetComPointer<ID2D1Geometry, ID2D1EllipseGeometry>(circleHolder);
	valid = true;
}

/**
 * Method: TGeometry::~TGeometry
 * Purpose: Destructor
 * Parameters: void
 * Returns: void
 */
TGeometry::~TGeometry()
{
	geo.Nullify();
}

/**
 * Method: TGeometry::IsValid
 * Purpose: Reports whether Geomatry construction was successful
 * Parameters: void
 * Returns:bool - whether the object is ready for use
 */
bool TGeometry::IsValid() const
{
	return valid;
}

/**
 * Method: TGeometry::GetArea
 * Purpose: Reports the Area
 * Parameters: float& res - reference to the float that will hold the area of the geometry
 * Returns:bool - whether the operation was successful
 */
bool TGeometry::GetArea(float& res)
{
	if(!geo.Get())
		return false;
	return SUCCEEDED(geo->ComputeArea(nullptr, &res));
}

/**
 * Method: TGeometry::GetParameter
 * Purpose: Reports the Parameter
 * Parameters: float& res - reference to the float that will hold the parameter of the geometry
 * Returns:bool - whether the operation was successful
 */
bool TGeometry::GetParameter(float& res)
{
	if (!geo.Get())
		return false;
	return SUCCEEDED(geo->ComputeLength(nullptr, &res));
}

/**
 * Method: TGeometry::IsInside
 * Purpose: Whether the point falls within the geomatry
 * Parameters: BOOL& res - reference to whether the point is inide or not
 *				POINT_2D& - the point to chack
 * Returns:bool - whether the operation was successful
 */
bool TGeometry::IsInside(BOOL& res, POINT_2D& p)
{
	if (!geo.Get())
		return false;
	return SUCCEEDED(geo->FillContainsPoint(p,nullptr, &res));
}

/**
 * Method: TGeometry::GetBounds
 * Purpose: Gets the boundaries of the geomatry in rectangle form
 * Parameters: RECT_2D& bounds - reference to hold the boundaries of the geometry
 * Returns:bool - whether the operation was successful
 */
bool TGeometry::GetBounds(RECT_2D& bounds)
{
	if (!geo.Get())
		return false;
	return SUCCEEDED(geo->GetBounds(nullptr, &bounds));
}

/**
 * Method: TGeometry::GetUnderlyingGeometry
 * Purpose: Allows access to the underlying Geometry
 * Parameters: void
 * Returns: TrecComPointer<ID2D1Geometry> - the underlying geometry
 */
TrecComPointer<ID2D1Geometry> TGeometry::GetUnderlyingGeometry()
{
	return geo;
}
