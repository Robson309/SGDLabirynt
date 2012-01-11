#ifndef LABIRYNTH_TILE_H
#define LABIRYNTH_TILE_H

#include "blocks\sprite.h"

using namespace std;

class LabirynthTile: public Sprite{
public:
	LabirynthTile();
	LabirynthTile(const string&,const int& c = 1); //c=1 (default) czarna przestrzeñ ma koszt jeden
	LabirynthTile* clone(); //wskaŸnik na clone()
	int cost() const { return cost_; }
private:
	int cost_;
};


#endif
