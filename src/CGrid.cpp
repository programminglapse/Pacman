#include "CGrid.h"

CGrid::CGrid( const map& aMap ) :
	mMap( aMap )
{
}

const CGrid::map& CGrid::GetMap() const
{
	return mMap;
}

CGrid::map& CGrid::Map()
{
	return mMap;
}
