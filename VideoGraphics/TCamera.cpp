#include "TCamera.h"

TCamera::TCamera()
{
}


/*
* Method: TCamera - setEngine
* Purpose: Sets the engine for the Arena control, if one is not already set
* Parameters: TrecPointer<ArenaEngine> e - the engine to set
* Returns: bool - success of function
*/
bool TCamera::setEngine(TrecPointer<TArenaEngine> e)
{

	arenaEngine = e;
	return true;
}

/*
* Method: TCamera - removeEngine
* Purpose: Extracts the engine from the control
* Parameters: void
* Returns: void
*/
void TCamera::removeEngine()
{
	arenaEngine.Nullify();
}

/*
* Method: TCamera - Rotate
* Purpose: Roates the camera
* Parameters: float leftRight - the horizontal angle to rotate
*				float upDown - the virtical angle to rotate
* Returns: void
*/
void TCamera::Rotate(float leftRight, float upDown)
{
	// Create a Float4 structure to hold w value (1.0f) as well as the current direction
	DirectX::XMFLOAT4 directLook{ direction_3.x, direction_3.y,direction_3.z,1.0f };

	// Adjust temporary direction if Camera is lookTo so it represents a transformable direction
	IF_IS_LOOK_AT(lookTo)
	{
		directLook.x -= location_3.x;
		directLook.y -= location_3.y;
		directLook.z -= location_3.z;
	}

	// Create the rotation matrix
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(upDown, leftRight, 0.0f);

	// Apply the rotation matrix to the direction
	DirectX::XMVECTOR lookAtXM = DirectX::XMVector4Transform(DirectX::XMVectorSet(
		directLook.x, directLook.y, directLook.z, directLook.w), rotation);

	// Retireve the results of the camera rotation
	DirectX::XMStoreFloat3(&direction_3, lookAtXM);

	// If this is a lookAt camera, make sure that highest value is a 1
	IF_IS_LOOK_TO(lookTo)
	{
		float maxVal = max(direction_3.x, max(direction_3.y, direction_3.z));
		direction_3.x /= maxVal;
		direction_3.y /= maxVal;
		direction_3.z /= maxVal;
	}

	// Make sure the new direction would not cause conflict with up
	if (direction_3.y && !direction_3.x && !direction_3.z)
	{
		up_3.x += 0.02;
	}
	else if (up_3.x)
	{
		up_3.x = 0.0f;
	}
	RefreshVectors();
}

/*
* Method: TCamera - Translate
* Purpose: Translates the camera across the coordinate space
* Parameters: float degree - the distance to translate
*			DirectX::XMFLOAT3 direction - the direction to translate
* Returns: void
*/
void TCamera::Translate(float degree, DirectX::XMFLOAT3 direction)
{
	//direction.x = direction.x * degree;
	///direction.y = direction.y * degree;
	//direction.z = direction.z * degree;

	location_3.x += (degree * direction.x);
	location_3.y += (degree * direction.y);
	location_3.z += (degree * direction.z);

	//direction_3.x += direction.x;
	//direction_3.y += direction.y;
	//direction_3.z += direction.z;
	RefreshVectors();
}

void TCamera::PanX(float x)
{
}

void TCamera::PanY(float y)
{
}

/*
* Method: TCamera - ChangeToLookAt
* Purpose: Turns camera into a Look At camera
* Parameters: void
* Returns: void
*/
void TCamera::ChangeToLookAt()
{
	IF_IS_LOOK_AT(lookTo)
		return;
	direction_3.x += location_3.x;
	direction_3.y += location_3.y;
	direction_3.z += location_3.z;
	lookTo = LOOK_AT;
}

/*
* Method: TCamera - ChangeToLookAt
* Purpose: Converts camera to a loot at camera with an object to look at
* Parameters: ArenaModel& am - the object to look at
*			float distance - the distance between the camera and the object
*			bool changeAngle -  adjusting the angle of the camera
* Returns:
*/
void TCamera::ChangeToLookAt(ArenaModel& am, float distance, bool changeAngle)
{
	DirectX::XMFLOAT3 newLoc = am.GetLocation();
	DirectX::XMFLOAT3 rawDistanceCoords;
	rawDistanceCoords.x = newLoc.x - location_3.x;
	rawDistanceCoords.y = newLoc.y - location_3.y;
	rawDistanceCoords.z = newLoc.z - location_3.z;

	float currentDist = sqrt(pow(rawDistanceCoords.x, 2) + pow(rawDistanceCoords.y, 2)
		+ pow(rawDistanceCoords.z, 2));

	if (distance > 0.0)
	{
		float difDistance = currentDist - distance;
	}
}

/*
* Method: TCamera - ChangeToLookTo
* Purpose: Converts camera to a look to camera
* Parameters: void
* Returns: void
*/
void TCamera::ChangeToLookTo()
{
	IF_IS_LOOK_TO(lookTo)
		return;
	float max = max(abs(direction_3.x), max(abs(direction_3.y), abs(direction_3.z)));
	DirectX::XMFLOAT3 atTo = direction_3;
	atTo.x -= location_3.x;
	atTo.y -= location_3.y;
	atTo.z -= location_3.z;
	direction_3.x = atTo.x / max;
	direction_3.y = atTo.y / max;
	direction_3.z = atTo.z / max;
	lookTo = LOOK_TO;
	RefreshVectors();
}

/*
* Method: TCamera - ChangeToLookTo
* Purpose: Change to LookTo camera with parameters
* Parameters: float x - x-coordindate
*				float y - y-coordinate
*				float z - z-coordinates
* Returns: void
*/
void TCamera::ChangeToLookTo(float x, float y, float z)
{
	lookTo = LOOK_TO;
	float max = max(abs(x), max(abs(y), abs(z)));
	direction_3.x = x / max;
	direction_3.y = y / max;
	direction_3.z = z / max;
	RefreshVectors();
}

UCHAR* TCamera::GetAnaGameType()
{
	return nullptr;
}

void TCamera::UpdatePos(float f, UCHAR dir)
{
	switch (dir)
	{
	case 0:
		location_3.x = f;
		break;
	case 1:
		location_3.y = f;
		break;
	case 2:
		location_3.z = f;
	}
	RefreshVectors();
}

void TCamera::UpdateDir(float f, UCHAR dir)
{
	switch (dir)
	{
	case 0:
		direction_3.x = f;
		break;
	case 1:
		direction_3.y = f;
		break;
	case 2:
		direction_3.z = f;
	}
	RefreshVectors();
}



DirectX::XMFLOAT3 TCamera::GetCameraLocation()
{
	return location_3;
}

DirectX::XMFLOAT3 TCamera::GetCameraDirection()
{
	return direction_3;
}

/*
* Method: TCamera - RefreshVectors
* Purpose: Refreshes the location and direction vectors
* Parameters: void
* Returns: void
*/
void TCamera::RefreshVectors()
{
	//DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(direction_3.x, direction_3.y, direction_3.z);

	direction = DirectX::XMVectorSet(direction_3.x, direction_3.y, direction_3.z, 1.0f);

	location = DirectX::XMVectorSet(location_3.x, location_3.y, location_3.z, 1.0f);
	//direction = DirectX::XMVectorSet(direction_3.x, direction_3.y, direction_3.z, 1.0f);
	up = DirectX::XMVectorSet(up_3.x, up_3.y, up_3.z, 1.0f);
	UpdateCamera();
}

/*
* Method: TCamera - UpdateCamera
* Purpose: Resets the camera matrix
* Parameters: void
* Returns: void
*/
void TCamera::UpdateCamera()
{

	camera = DirectX::XMMatrixLookToLH(location, direction, up);
}
