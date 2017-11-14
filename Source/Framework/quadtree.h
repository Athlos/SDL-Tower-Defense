#pragma once

#include "position.h"
#include "axisalignedboundingbox.h"

#include <vector>

class BackBuffer;
class Entity;

class QuadTree
{
	//Member Methods:
public:
	QuadTree(AxisAlignedBoundingBox* bounds);
	~QuadTree();

	void Subdivide(); // Divide quad tree into four children
	std::vector<Entity*> QueryRange(AxisAlignedBoundingBox* range);

	bool Insert(Entity* pos);

	void Draw(BackBuffer& backBuffer);

	//Member Data:
private:
	const int NODE_CAPACITY = 4; // Max elements that can be stored

	AxisAlignedBoundingBox* m_bounds; // Represents the bounds of this quad tree

	std::vector<Entity*> m_points; // Array of points stored in this quad tree

	//Sub trees
	QuadTree* m_topLeft;
	QuadTree* m_topRight;
	QuadTree* m_bottomLeft;
	QuadTree* m_bottomRight;
};

