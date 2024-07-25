#ifndef C_GRID__H
#define C_GRID__H

#include <vector>

class CGrid
{
public:
	using map = std::vector<std::vector<int>>;

	CGrid( const map& aMap );

	const map& GetMap() const;
	map& Map();

private:
	map mMap;
};

#endif // C_GRID__H
