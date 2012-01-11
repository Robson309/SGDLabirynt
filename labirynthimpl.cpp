#include "labirynthimpl.h"
#include <iostream>
#include "blocks\display.h"
#include "pathfinding\AStarSearch.h"
#include <vector>
#include <list>
#include "plotter.h"`

//do ogl¹dania Windows+w powrót alt+w

using namespace std;

LabirynthImpl::LabirynthImpl()
{
	btnFrame_ = new LabirynthTile( "data/frame.bmp" );
	btnFrame_->move( 8, 598 );
	btnWall_ = new LabirynthTile( "data/wall.bmp", 1000 ); //koszt przejœcia
	btnWall_->move( 10, 600 );
	btnStart_ = new LabirynthTile( "data/start.bmp" );
	btnStart_->move( 40, 600 );
	btnFinish_ = new LabirynthTile( "data/finish.bmp" );
	btnFinish_->move( 70, 600 );
	btnSearch_ = new Sprite( "data/search.bmp" );
	btnSearch_->move( 100, 600 );
	btnCurrent_ = btnWall_;
	start_ = NULL;
	finish_ = NULL;
	plotPath_ = false;
}


LabirynthImpl::~LabirynthImpl(){
	delete btnFrame_;
	delete btnWall_;
	delete btnStart_;
	delete btnFinish_;
	delete btnSearch_;
}


void LabirynthImpl::processLMB(){ //klikamy na lewy przycisk
	int gridx = Display().mouseX() / 20; //miejsce na ekranie
	int gridy = Display().mouseY() / 20;

	if ( !buttonPressed() ){ //je¿eli nie klikamy na button
		plotPath_ = false;
		path_.clear();		
		if ( !(gridx < 0 || gridx >= 30 ||
			gridy < 0 || gridy >= 30) ){
				if ( btnCurrent_ == btnStart_ ){ //je¿eli button na start
					removeKeyPoint( start_ ); //usuñ stary start
					delete grid_[gridy][gridx]; //skasuj czarn¹ przestrzeñ
					grid_[gridy][gridx] = btnCurrent_->clone(); //skasowane miejsce wskazuje na clone() - a to zwraca new LabirynthTile z t¹ sam¹ bitmap¹ i kosztem
					start_ = grid_[gridy][gridx]; //
				} else if ( btnCurrent_ == btnFinish_ ){
					removeKeyPoint( finish_ );
					delete grid_[gridy][gridx];
					grid_[gridy][gridx] = btnCurrent_->clone();
					finish_ = grid_[gridy][gridx];
				} else{
					delete grid_[gridy][gridx];
					grid_[gridy][gridx] = btnCurrent_->clone(); //je¿eli nie START lub FINISH to wstaw sciankê
				}
				grid_[gridy][gridx]->move( 20 * gridx, 20 * gridy ); //pozycjonowanie na ekranie
		}
	}
}

void LabirynthImpl::processRMB(){
	int gridx = Display().mouseX() / 20;
	int gridy = Display().mouseY() / 20;

	if ( !(gridx < 0 || gridx >= grid_.width() ||
		gridy < 0 || gridy >= grid_.height()) ){
			delete grid_[gridy][gridx];
			grid_[gridy][gridx] = new LabirynthTile(); //tworzy nowy pusty
	}
}

bool LabirynthImpl::buttonPressed(){
	if ( Display().mouseOver( *btnWall_ ) ){
		btnFrame_->move( 8, 598 );
		btnCurrent_ = btnWall_;
		return true;
	} else if ( Display().mouseOver( *btnStart_ ) ){
		btnFrame_->move( 38, 598 );
		btnCurrent_ = btnStart_;
		return true;
	} else if ( Display().mouseOver( *btnFinish_) ){
		btnFrame_->move( 68, 598 );
		btnCurrent_ = btnFinish_;
		return true;
	} else if ( Display().mouseOver( *btnSearch_) ){
		plotPath_ = true;
		buildGraph();
		Point start = getKeyPointCoords( start_ ),
			finish = getKeyPointCoords( finish_ );
		int cost;
		path_ = findPath( start, finish, &cost);

		cout << "Traversal cost: " << cost << endl;
		return true;
	}
	return false;
}

Point LabirynthImpl::getKeyPointCoords(LabirynthTile* keypoint){
	for (int i = 0; i != grid_.height(); ++i ){
		for (int j = 0; j != grid_.width(); ++j )
			if ( grid_[i][j] == keypoint ){
				return Point(j,i);
			}
	}
	//todo: replace with exception!
	return Point(-1,-1);
}

void LabirynthImpl::removeKeyPoint(LabirynthTile* keypoint){ //usuwanie 'starego' START i FINISH
	bool search = true;
	for (int i = 0; i != grid_.height() && search; ++i ){
		for (int j = 0; j != grid_.width() && search; ++j )
			if ( grid_[i][j] == keypoint ){
				delete grid_[i][j];
				grid_[i][j] = new LabirynthTile();
				search = false; //konczy przeszukiwanie grida
			}
	}
}

void LabirynthImpl::draw(){
	for (int i = 0; i != grid_.height(); ++i ){
		for (int j = 0; j != grid_.width(); ++j ){
			grid_[i][j]->draw();
		}
	}
	btnFrame_->draw();
	btnWall_->draw();
	btnStart_->draw();
	btnFinish_->draw();
	btnSearch_->draw();

	if ( plotPath_ ){
		Plotter().plot( path_, Point(10,10), Point(20,20) ); //przesuniêcie 10,10 (œrodek tile'a), skalowanie 20,20 (wymiary tile'a)
	}

}

void LabirynthImpl::buildGraph(){
	pathfindingGraph_.Clear();
	for (int i = 0; i != grid_.height(); ++i ){
		for (int j = 0; j != grid_.width(); ++j ){
			pathfindingGraph_.AddNode( GraphNode( i * grid_.width() + j ) );
		}
	}
	for ( int start = 0; start!= pathfindingGraph_.NumNodes(); ++start ){
		int x = start % grid_.width(), y = start / grid_.width(); //indeks wêz³a x=100%30=10; y=100/30=3
		Point neighbours[] = 
		{
			Point( x, y - 1 ),
			Point( x - 1, y ),								Point( x + 1, y ),
			Point( x, y + 1 ),
		};
		for ( int numNeighbour = 0; numNeighbour!= 4; ++numNeighbour){
			Point& p = neighbours[numNeighbour];
			if ( !(p.x < 0 || p.y < 0 || p.x >= grid_.width() || p.y >= grid_.height() ) ){//sprawdzam, czy s¹siad w zakresie
				int finish = grid_.width() * p.y + p.x; //indeks wêz³a odpowiadaj¹cego s¹siadowi
				int cost = grid_[p.y][p.x]->cost(); //koszt s¹siada
				pathfindingGraph_.AddEdge( start, GraphEdge( start, finish, (float)cost ) );
			}
		}
	}
}

vector<Point> LabirynthImpl::findPath(const Point& p1,const Point& p2, int* outCost){ //
	AStarSearch search( pathfindingGraph_,
		p1.y * grid_.width() + p1.x,
		p2.y * grid_.width() + p2.x );

	vector<Point> path;

	list<int> nodeList = search.PathToTarget(); //lista indeksów wêz³ów w grafie

	for ( std::list<int>::iterator it = nodeList.begin();
		it != nodeList.end(); ++it ) //iterujemy siê po liœcie indeksów
	{
		path.push_back( Point((*it) % grid_.width(), (*it) / grid_.width()) ); //*it dereferencja iteratora
	}

	for ( int i = 0; i != path.size(); ++i ){ // pokazuje œcie¿kê na konsoli
		cout << "(" << path[i].x << "," << path[i].y << ")" << endl;
	}

	if ( outCost )
	{
		*outCost = (int)search.CostToTarget();
	}
	return path;
}
