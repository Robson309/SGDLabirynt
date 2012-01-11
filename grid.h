#ifndef GRID_H
#define GRID_H

#include <vector>

using namespace std;

template<class C>
class Grid{
public:
	Grid(const int& w = 30,const int& h = 30):
	  width_(w), height_(h){
		  grid_.resize(height_);
		  for (int i = 0; i != height_; ++i ){
			  grid_[i].resize(width_);
			  for (int j = 0; j != width_; ++j ){
				  grid_[i][j] = new C;
			  }
		  }
	  }

	  ~Grid(){
		  for (int i = 0; i != height_; ++i ){
			  for (int j = 0; j != width_; ++j )
				  delete grid_[i][j];
		  }
	  }
	  
	  vector<C*>& operator[](const int& i){ //przeci¹¿enie operatora indeksowania
		  return grid_[i]; //zwraca wiersz macierzy
	  }

	  int width() const { return width_; }

	  int height() const { return height_; }

private:
	vector<vector<C*> > grid_;
	int width_, height_;
};


#endif
