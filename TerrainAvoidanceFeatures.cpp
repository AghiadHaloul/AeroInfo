#include<iostream>
using namespace std;
//4- Terrain Avoidance Implementation 
//This class contains the input parameters (altitude, descentRate, terrainClosureRate)
class FlyingPoint
{
public:
	double descentRateX;
	double heightAboveTerrainY;
	double terrainClosureRateZ;
	FlyingPoint(double descRate, double alt, double terClosureRate)
	{
		this->descentRateX = descRate;
		this->heightAboveTerrainY = alt;
		this->terrainClosureRateZ = terClosureRate;
	}
	
};

// Compares the location of a point in a plane with a line segment
bool pointLeftOfLine(FlyingPoint query, FlyingPoint segmentStart, FlyingPoint segmentEnd)
{
	double location = (segmentEnd.descentRateX - segmentStart.descentRateX) * (query.heightAboveTerrainY - segmentStart.heightAboveTerrainY) -
		(query.descentRateX - segmentStart.descentRateX) * (segmentEnd.heightAboveTerrainY - segmentStart.heightAboveTerrainY);
	cout << location << endl;
	if (location>0) return true;
	return false;
}

//Excessive Descent Rate MODE1 
int excessiveDescentRate(double radarAltimeter, double barometricAltimeter)
{
	// Returns 0 if clear
	// Returns 1 for "SINKRATE"
	// Returns 2 for "PULL UP!"
	if (radarAltimeter > 2400) return 0;

	FlyingPoint sinkLineP1(0, 1400, 0), sinkLineP2(2400, 4500, 0);
	FlyingPoint pullLineP1(0, 1800, 0), pullLineP2(250, 1900, 0);
	FlyingPoint pullLineP3(250, 1900, 0), pullLineP4(2450, 6500, 0);
	FlyingPoint currentLocation(radarAltimeter, barometricAltimeter, 0);

	//Check if our point is left of the Sinkrate area
	if (pointLeftOfLine(currentLocation, sinkLineP1, sinkLineP2))
		return 0;

	//Check if our point is left of the pull up area
	if (radarAltimeter <= 250) {
		//Check the small segment
		if (pointLeftOfLine(currentLocation, pullLineP3, pullLineP4))
			return 1;
		else
			return 2;
	}
	else if (radarAltimeter > 250) {
		//Check the larger segment
		if (pointLeftOfLine(currentLocation, pullLineP1, pullLineP2))
			return 1;
		else
			return 2;
	}
}

//Excessive Terrain Closure Rate MODE2
int excessiveTerrainColsureRate(double radarAltimeter, double terrainClosureRate)
{	// Returns 0 if clear
	// Returns 1 for "TERRAIN! TERRAIN!"
	// Returns 2 for "PULL UP!"
	if (terrainClosureRate >= 1750)return 0;
	FlyingPoint currentLocation(radarAltimeter, 0, terrainClosureRate);
	FlyingPoint 
		clearLineP1(0, 0, 0),
		clearLineP2(4000, 0, 1500),
		clearLineP3(6000, 0, 1750);

	FlyingPoint
		terrainLineP1(0, 0, 2400),
		terrainLineP2(4100, 0, 1400),
		terrainLineP3(6000, 0, 1600);
	if (radarAltimeter > 2200) return 0;
	else if (radarAltimeter >= 1500)
	{
		if (pointLeftOfLine(currentLocation, clearLineP1, clearLineP2))
			return 0;
		else if (pointLeftOfLine(currentLocation, terrainLineP1, terrainLineP2))
			return 1;
	}
	else
	{
		if (pointLeftOfLine(currentLocation, clearLineP1, clearLineP2))
			return 0;
		else if (pointLeftOfLine(currentLocation, terrainLineP1, terrainLineP2))
			return 1;
	}		
}

//5- Terrain Avoidance DISPLAY