#pragma once

class Position;

class AxisAlignedBoundingBox
{
	//Member Methods:
public:
	AxisAlignedBoundingBox();
	~AxisAlignedBoundingBox();

	AxisAlignedBoundingBox(Position* cent, float half);

	bool containsPosition(Position* pos);

	bool intersects(AxisAlignedBoundingBox* box);

	//Member Data:
public:
	Position* center;

	float halfDimension;
};

