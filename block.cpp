#include "block.h"

//From im, grabs the vertical strip of pixels whose upper
//left corner is at position (column,0), and whose width is width.
void Block::build(PNG & im, int column, int width){
  data.reserve(im.height());
  for(int i = 0; i < im.height(); i++){
    vector<HSLAPixel> v;
    v.reserve(width);
    //need to create new row vector, else segfault
    //data.push_back(v); pushing back here simply pushes empty vector v onto data
    for(int j = 0; j < width; j++){
      HSLAPixel * p = im.getPixel(column + j, i);
      v.push_back(* p);
      // data[i][j] = * p; does not exist yet
    }
    data.push_back(v); //else v disappears as it is a local variable
  }
}

//Draws the current block at position (column,0) in im
void Block::render(PNG & im, int column) const {
  for(int i = 0; i < height(); i++){
    for(int j = 0; j < width(); j++){
      HSLAPixel * p = im.getPixel(column + j, i);
      * p = data[i][j];
    }
  }
}

void Block::greyscale(){
  for(int i = 0; i < height(); i++){
    for(int j = 0; j < width(); j++){
      data[i][j].s = 0;
    
      
    }
  }
}

int Block::width() const {
  return data.size() > 0 ? data[0].size() : 0; // ternary, involving 3 operands
}

int Block::height() const {
  return data.size();
}
