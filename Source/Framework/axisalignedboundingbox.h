#pragma once

class Position;

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox();
	~AxisAlignedBoundingBox();

public:
	Position* center;

	float halfDimension;

public:
	AxisAlignedBoundingBox(Position* cent, float half);

	bool containsPosition(Position* pos);

	bool intersects(AxisAlignedBoundingBox* box);
};

