#ifndef PLOTTER_H
#define PLOTTER_H

#include <vector>
#include "point.h"

using namespace std;

class Plotter{
public:
	void plot( const vector<Point>&, const Point& translate = Point(0,0), const Point& scale = Point(1,1) );
};

#endif