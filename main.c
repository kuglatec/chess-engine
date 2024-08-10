#include "movegen.c"
int main() {
    setbuf(stdout, NULL); /*disable buffering for stdout*/
    setbuf(stdin, NULL); /*same for stdin*/
    struct Piece* ps = loadpieces(); /*initalize pointer to pieces array allocated on heap*/
    struct Move move;
    move.startX = 6;
    move.startY = 1;
    move.destX = 4;
    move.destY = 3;
    move.pieceID = 13;
    struct Move* mvs = getMoves(ps, 0);
    printf("\nARLEN: %d\n", mvs[0].arlen); 
  //mValid(move, ps);
   // printf("%d", mValid(move, ps, 0));

  //  printf("EVAL: %d\n", eval(ps, 0));
    
    return 0;
}
