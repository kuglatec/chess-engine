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
    
    return 0;
}
