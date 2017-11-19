#pragma once

class Position;

class AxisAlignedBoundingBox
{
	//Member Methods:
public:
	AxisAlignedBoundingBox();
	~AxisAlignedBoundingBox();

	AxisAlignedBoundingBox(Position* cent, float half); // AABB constructor takes in center of bounds and diameter

	bool containsPosition(Position* pos); // Returns if the current position is within the bounds

	bool intersects(AxisAlignedBoundingBox* box); // Return if a box intersects with the bounds

	//Member Data:
public:
	Position* center; // Center of the bounds

	float halfDimension; // Half the diameter of a bounding box
};

