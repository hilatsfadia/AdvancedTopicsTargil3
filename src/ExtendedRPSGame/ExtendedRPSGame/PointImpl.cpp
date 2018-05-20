#include "stdafx.h"
#include "PointImpl.h"
#include <cstdlib>

inline bool operator==(const PointImpl & lhs, const PointImpl & rhs)
{
	return ((lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY()));
}

int PointImpl::DistanceInStepsFrom(const PointImpl& other) const
{
	int verticalDiff = abs(this->getY() - other.getY());
	int horizontalDiff = abs(this->getX() - other.getX());
	return verticalDiff + horizontalDiff;
}

