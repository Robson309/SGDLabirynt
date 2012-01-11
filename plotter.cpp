#include "plotter.h"
#include "blocks\display.h"

void Plotter::plot( const vector<Point>& points, const Point& translate, const Point& scale ){
	const unsigned char color[] = { 255,255,255 }; //bia³a linia
	for ( int i = 0; i != points.size() - 1; ++i ){ // -1, bo do przedostatniego
		Display().draw_line(
			translate.x + points[i].x * scale.x,
			translate.y + points[i].y * scale.y,
			translate.x + points[i+1].x * scale.x,
			translate.y + points[i+1].y * scale.y,
			color );
	}
}