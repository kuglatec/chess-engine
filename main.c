#include "movegen.c"
int main() {
    setbuf(stdout, NULL); /*disable buffering for stdout*/
    setbuf(stdin, NULL); /*same for stdin*/
    struct Piece* ps = loadpieces(); /*initalize pointer to pieces array allocated on heap*/
    struct Move move;
    move.startX = 1;
    move.startY = 2;
    move.destX = 1;
    move.destY = 3;
    move.pieceID = 0;
  //  struct Move* mvs = getMoves(ps, 0);
    struct State* states = getstates(ps, 0, 0);
 //   printf("\nL:%d\n", states[0].m.destY);
   // printf("\n%d\n", states[0].score);
   
    printf("\n%d\n", eval(states[0].ps, 0));
    for (int p = 0; p < 32; p++) {
      printf("\nX:%d\nY:%d\n", states[0].ps[p].xpos, states[0].ps[p].ypos);
   }
    //printf("\nARLEN: %d\n", mvs[0].arlen); 
  //mValid(move, ps);
   // makeMove(move, ps, 0);
 //   printf("\n%d/%d", ps[0].xpos, ps[0].ypos);
   // printf("%d", mValid(move, ps, 0));

  //  printf("EVAL: %d\n", eval(ps, 0));
    
    return 0;
}
