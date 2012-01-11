#ifndef LABIRYNTH_IMPL_H
#define LABIRYNTH_IMPL_H

#include "grid.h"
#include "point.h"
#include "labirynthtile.h"
#include "pathfinding/SparseGraph.h"

using namespace cimg_library;
using namespace std;

class LabirynthImpl{
public:
	LabirynthImpl();
	~LabirynthImpl();
	void processLMB();
	void processRMB();
	bool buttonPressed();
	void draw();
	Point getKeyPointCoords(LabirynthTile*); //koordynaty START i  FINISH, p1 i p2
	void removeKeyPoint(LabirynthTile*); //usuwanie START i  FINISH - mo¿e byæ tylko 1 start i 1 finish
	void buildGraph(); //dodawanie wêz³ów od 0 do 889 (bo 30x30) i krawêdzi do ka¿dego wêz³a
	vector<Point> findPath(const Point&,const Point&, int* outCost = NULL); //szukamy œcie¿ki od p1 do p2

private:
	Grid<LabirynthTile> grid_; //siatka tile'ów
	LabirynthTile *btnFrame_, *btnWall_, *btnStart_, *btnFinish_, *btnCurrent_; //buttony
	Sprite* btnSearch_; 
	LabirynthTile *start_, *finish_; //start i koniec œcie¿ki
	SparseGraph		pathfindingGraph_; //graf do szukania œcie¿ki - Matt Buckland
	bool	plotPath_; 
	vector<Point> path_; //œcie¿ka, wyœwietlana na konsoli
};

#endif

