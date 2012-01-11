#ifndef POINT_H
#define POINT_H

struct Point{
	Point():x(0),y(0){}
	Point(const int& x_,const int& y_):
		x(x_),y(y_){}
	int x,y;
};

#endif