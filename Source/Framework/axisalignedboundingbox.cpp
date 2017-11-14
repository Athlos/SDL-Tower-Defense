#include "axisalignedboundingbox.h"
#include "position.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Position* cent, float half)
{
	center = cent;
	halfDimension = half;
}

bool AxisAlignedBoundingBox::containsPosition(Position* pos)
{
	if (pos->m_x < center->m_x + halfDimension && pos->m_x > center->m_x - halfDimension) // Check within x coordinates
	{
		if (pos->m_y < center->m_y + halfDimension && pos->m_y > center->m_y - halfDimension) // Check within y coordinates
		{
			return true;
		}
	}
	return false;
}

bool AxisAlignedBoundingBox::intersects(AxisAlignedBoundingBox* box)
{
	if (center->m_x + halfDimension > box->center->m_x - box->halfDimension && center->m_x - halfDimension < box->center->m_x + box->halfDimension) 
	{
		if (center->m_y + halfDimension > box->center->m_y - box->halfDimension && center->m_y - halfDimension < box->center->m_y + box->halfDimension)
		{
			return true;
		}
	}

	return false;
}
