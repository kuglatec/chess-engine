#include "movegen.c"

int main() {
    struct Piece* ps = loadpieces(); /*initalize pointer to pieces array allocated on heap*/
    struct Move move;
    move.startX = 3;
    move.startY = 1;
    move.destX = 3;
    move.destY = 1;
    move.pieceID = 12;
    printf("%d", mValid(move, ps));
  //mValid(move, ps);
  for (int p = 0; p < 32; p++){
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
    move.startX = ps[p].xpos;
    move.startY = ps[p].ypos;
    move.destX = i;
    move.destY = j;
    move.pieceID = p;
    printf("%d", mValid(move, ps));
    }
  }
  }
    
    return 0;
}