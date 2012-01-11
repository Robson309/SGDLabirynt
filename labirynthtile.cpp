#include "labirynthtile.h"

LabirynthTile::LabirynthTile():
Sprite(CImg<unsigned char>(20, 20, 1, 3, 0)), cost_( 1 ){

}

LabirynthTile::LabirynthTile(const string& s,const int& c):
Sprite( s ), cost_( c ){
}

LabirynthTile* LabirynthTile::clone(){
	return new LabirynthTile(file_,cost_);
}