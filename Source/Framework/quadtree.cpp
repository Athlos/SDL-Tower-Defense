#include "quadtree.h"
#include "backbuffer.h"
#include "entity.h"

QuadTree::QuadTree(AxisAlignedBoundingBox* bounds)
	: m_bounds(bounds)
	, m_topLeft(0)
	, m_topRight(0)
	, m_bottomLeft(0)
	, m_bottomRight(0)
{
}

QuadTree::~QuadTree()
{
	delete m_topLeft;
	m_topLeft = 0;

	delete m_topRight;
	m_topRight = 0;

	delete m_bottomLeft;
	m_bottomLeft = 0;

	delete m_bottomRight;
	m_bottomRight = 0;

	delete m_bounds;
	m_bounds = 0;
}

void QuadTree::Subdivide()
{
	Position* topLeftPos = new Position( m_bounds->center->m_x - (m_bounds->halfDimension / 2.0f), m_bounds->center->m_y + (m_bounds->halfDimension / 2.0f) );
	Position* topRightPos = new Position(m_bounds->center->m_x + (m_bounds->halfDimension / 2.0f), m_bounds->center->m_y + (m_bounds->halfDimension / 2.0f) );
	Position* bottomLeftPos = new Position(m_bounds->center->m_x - (m_bounds->halfDimension / 2.0f), m_bounds->center->m_y - (m_bounds->halfDimension / 2.0f) );
	Position* bottomRightPos = new Position(m_bounds->center->m_x + (m_bounds->halfDimension / 2.0f), m_bounds->center->m_y - (m_bounds->halfDimension / 2.0f) );

	AxisAlignedBoundingBox* topLeftBounds = new AxisAlignedBoundingBox( topLeftPos, (m_bounds->halfDimension / 2.0f) );
	AxisAlignedBoundingBox* topRightBounds = new AxisAlignedBoundingBox( topRightPos, (m_bounds->halfDimension / 2.0f) );
	AxisAlignedBoundingBox* bottomLeftBounds = new AxisAlignedBoundingBox(bottomLeftPos, (m_bounds->halfDimension / 2.0f));
	AxisAlignedBoundingBox* bottomRightBounds = new AxisAlignedBoundingBox(bottomRightPos, (m_bounds->halfDimension / 2.0f));

	m_topLeft = new QuadTree(topLeftBounds);
	m_topRight = new QuadTree(topRightBounds);
	m_bottomLeft = new QuadTree(bottomLeftBounds);
	m_bottomRight = new QuadTree(bottomRightBounds);

	//Attempt to reinsert points into children trees
	std::vector<Entity*>::iterator pointIter = m_points.begin();

	while (pointIter != m_points.end())
	{
		Entity* current = *pointIter;

		if (AmountOfIntersectionsInChildren(current->GetCollisionBounds()) <= 1)
		{
			if (AddNodeToChildren(current))
			{
				pointIter = m_points.erase(pointIter);
			}
			else
			{
				++pointIter;
			}
		}
		else
		{
			++pointIter;
		}
	}
}

std::vector<Entity*> QuadTree::QueryRange(AxisAlignedBoundingBox* range)
{
	std::vector<Entity*> entitiesInRange = std::vector<Entity*>();

	if (!m_bounds->intersects(range))
	{
		return entitiesInRange; // if range does not intersect bounds, return empty list
	}

	//Check all points and add those that are within the range
	for (int i = 0; i < m_points.size(); ++i)
	{
		if (range->intersects(m_points[i]->GetCollisionBounds()))
		{
			entitiesInRange.push_back(m_points[i]);
		}
	}

	//check children for other points
	if (m_topLeft != 0)
	{
		std::vector<Entity*> topLeft = m_topLeft->QueryRange(range);
		std::vector<Entity*> topRight = m_topRight->QueryRange(range);
		std::vector<Entity*> bottomLeft = m_bottomLeft->QueryRange(range);
		std::vector<Entity*> bottomRight = m_bottomRight->QueryRange(range);

		entitiesInRange.insert(entitiesInRange.begin(), topLeft.begin(), topLeft.end());
		entitiesInRange.insert(entitiesInRange.begin(), topRight.begin(), topRight.end());
		entitiesInRange.insert(entitiesInRange.begin(), bottomLeft.begin(), bottomLeft.end());
		entitiesInRange.insert(entitiesInRange.begin(), bottomRight.begin(), bottomRight.end());
	}

	return entitiesInRange;
}

std::vector<Entity*> QuadTree::QueryPoint(Position* point)
{
	std::vector<Entity*> entitiesInRange = std::vector<Entity*>();

	if (!m_bounds->containsPosition(point))
	{
		return entitiesInRange; // if point does not intersect bounds, return empty list
	}

	for (int i = 0; i < m_points.size(); ++i)
	{
		/*if (range->containsPosition(m_points[i]->GetPosition()))
		{
		entitiesInRange.push_back(m_points[i]);
		}*/

		if (m_points[i]->GetCollisionBounds()->containsPosition(point))
		{
			entitiesInRange.push_back(m_points[i]);
		}
	}

	//check children for other points
	if (m_topLeft != 0)
	{
		std::vector<Entity*> topLeft = m_topLeft->QueryPoint(point);
		std::vector<Entity*> topRight = m_topRight->QueryPoint(point);
		std::vector<Entity*> bottomLeft = m_bottomLeft->QueryPoint(point);
		std::vector<Entity*> bottomRight = m_bottomRight->QueryPoint(point);

		entitiesInRange.insert(entitiesInRange.begin(), topLeft.begin(), topLeft.end());
		entitiesInRange.insert(entitiesInRange.begin(), topRight.begin(), topRight.end());
		entitiesInRange.insert(entitiesInRange.begin(), bottomLeft.begin(), bottomLeft.end());
		entitiesInRange.insert(entitiesInRange.begin(), bottomRight.begin(), bottomRight.end());
	}

	return entitiesInRange;
}

bool QuadTree::Insert(Entity* pos)
{
	if (!m_bounds->intersects(pos->GetCollisionBounds())) // Skip positions not inside this quad tree
	{
		return false;
	}

	if (m_points.size() >= NODE_CAPACITY && m_topLeft == 0)
	{
		Subdivide();
	}

	//Check if it collides with children nodes
	if (m_topLeft != 0)
	{
		if (AmountOfIntersectionsInChildren(pos->GetCollisionBounds()) > 1) // We have to insert into current level since it collides with more than one child
		{
			m_points.push_back(pos);

			return true;
		}
		else
		{
			//Attempt to insert into sub trees
			return AddNodeToChildren(pos);
		}
	}
	else
	{
		m_points.push_back(pos);
		return true;
	}

	//if (m_points.size() < NODE_CAPACITY) //Add if there is space
	//{
	//	m_points.push_back(pos);
	//	return true;
	//}
	//
	//if (m_topLeft == 0) // If full, then subdivide and add to one that has room
	//{
	//	Subdivide();
	//}
	//
	//Check how many children this collides with


}

void QuadTree::Draw(BackBuffer& backBuffer)
{
	backBuffer.SetDrawColour(0, 0, 0);

	backBuffer.DrawRectangle(m_bounds->center->m_x - m_bounds->halfDimension, m_bounds->center->m_y + m_bounds->halfDimension, m_bounds->center->m_x + m_bounds->halfDimension, m_bounds->center->m_y - m_bounds->halfDimension, 0);

	////Draw points
	//for (int i = 0; i < m_points.size(); ++i)
	//{
	//	backBuffer.DrawRectangle(m_points[i]->m_x, m_points[i]->m_y, m_points[i]->m_x + 2, m_points[i]->m_y + 2, 1);
	//}

	if (m_topLeft != 0)
	{
		m_topLeft->Draw(backBuffer);
		m_topRight->Draw(backBuffer);
		m_bottomLeft->Draw(backBuffer);
		m_bottomRight->Draw(backBuffer);
	}
}

int QuadTree::AmountOfIntersectionsInChildren(AxisAlignedBoundingBox* bounds)
{
	int numIntersections = 0;

	if (m_topLeft->m_bounds->intersects(bounds))
	{
		++numIntersections;
	}

	if (m_topRight->m_bounds->intersects(bounds))
	{
		++numIntersections;
	}

	if (m_bottomLeft->m_bounds->intersects(bounds))
	{
		++numIntersections;
	}

	if (m_bottomRight->m_bounds->intersects(bounds))
	{
		++numIntersections;
	}

	return numIntersections;
}

bool QuadTree::AddNodeToChildren(Entity * pos)
{
	if (m_topLeft->Insert(pos))
	{
		return true;
	}
	else if (m_topRight->Insert(pos))
	{
		return true;
	}
	else if (m_bottomLeft->Insert(pos))
	{
		return true;
	}
	else if (m_bottomRight->Insert(pos))
	{
		return true;
	}
	else
	{
		return false; // if node could not be inserted anywhere, should not trigger
	}
}
