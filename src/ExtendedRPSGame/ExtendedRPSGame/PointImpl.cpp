#include "stdafx.h"
#include "PointImpl.h"
#include <cstdlib>

int PointImpl::DistanceInStepsFrom(const PointImpl& other) const
{
	int verticalDiff = abs(this->getY() - other.getY());
	int horizontalDiff = abs(this->getX() - other.getX());
	return verticalDiff + horizontalDiff;
}

