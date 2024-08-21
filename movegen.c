#define DEPTH 1
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "structs.h"
/*init values for pieces in decipawns*/
#define QUEEN 90
#define ROOK 50
#define PAWN 10
#define KNIGHT 30
#define BISHOP 32
#define KING 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
struct Square kqs[2]; /*keysquares (D & E 4 or D & E 5)*/
struct Square iqs[2]; /*important squares (C & F 4 or C & F 5)*/


int mValid(struct Move m, struct Piece* ps, const int player);

int wchecker(int i, struct Square* sqs, struct Piece* ps, struct Move m, int player, int opponent) {
    int p;
    for (int j = 0; j <= i; j++) {

        if (!(sqs[j].x == m.startX && sqs[j].y == m.startY)) {
            for (p = 0; p < 32; p++) {
                if (ps[p].xpos == sqs[j].x && ps[p].ypos == sqs[j].y && ps[p].captured == 0) {
                  //  printf("\n%d/%d\n", sqs[j].x, sqs[j].y);
                     if (!(ps[p].ypos == m.destY && ps[p].xpos == m.destX) || ((ps[p].xpos == m.destX && ps[p].ypos == m.destY) && ps[p].owner == player)) {
                            return 0;
                        }
                }
            }
        }
    }
    return 1;
}
int queenValid(struct Move m, struct Piece* ps, int pl, int opponent) {
    const int player = pl; /*somehow, the player variable is changing so it is now a const*/
    /*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner != player) {
        return 0;
    }
    int index = 0;

    if (m.startX == m.destX) { /*rook moves on Y axis*/

        if (m.startY < m.destY) {
            struct Square sqs[m.destY - m.startY];
            for (int i = m.startY + 1; i <= m.destY; i++) {
                sqs[index].x = m.startX;
                sqs[index].y = i;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
        else if (m.startY > m.destY) {
            struct Square sqs[m.startY - m.destY];
            for (int i = m.startY - 1; i >= m.destY; i--) {
                sqs[index].x = m.startX;
                sqs[index].y = i;;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
    }
    else if (m.startY == m.destY) { /*rook moves on x axis*/
        if (m.startX < m.destX) {
            struct Square sqs[m.destX - m.startX];
            for (int i = m.startX + 1; i <= m.destX; i++) {
                sqs[index].x = i;
                sqs[index].y = m.startY;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
        else if (m.startX > m.destX) {
    //        printf("test: %d", m.startX - m.destX);
            struct Square sqs[m.startX - m.destX];
            for (int i = m.startX - 1; i >= m.destX; i--) {
                sqs[index].x = i;
                sqs[index].y = m.startY;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
    }
    else if (abs(m.destX - m.startX) == abs(m.destY - m.startY)) { /*check if piece moves on a diagonal*/
        int i = 0; /*index for sqs array*/
        int s; /*integet holding temp. square*/
        if (m.destX > m.startX && m.destY > m.startY) { /*piece moves right up*/
            struct Square sqs[m.destX - m.startX]; /*allocate array of squares each holding an x and y value*/
            for (s = m.startX; i <= m.destX - m.startX; s++) {
                   sqs[i].x = s;
                   sqs[i].y =  m.startY + (s - m.startX);
                   i++;
            }
            return wchecker(i, sqs, ps, m, player, opponent);
        }
        else if (m.destX > m.startX && m.destY < m.startY) { /*piece moves right down*/
            struct Square sqs[m.destX - m.startX]; /*allocate array of squares each holding an x and y value*/
            for (s = m.startX; i <= m.destX - m.startX; s++) {
                   sqs[i].x = s;
                   sqs[i].y =  m.startY - (s - m.startX);
                   i++;
            }
            return wchecker(i, sqs, ps, m, player, opponent);
        }
        else if (m.destX < m.startX && m.destY > m.startY) { /*piece moves left up*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            for (s = m.startY; i <= m.destY - m.startY; s++) {
                sqs[i].y = s;
                sqs[i].x =  m.startX - (s - m.startY);
                i++;
            }
            return wchecker(i, sqs, ps, m, player, opponent);

        }
        else if (m.destX < m.startX && m.destY < m.startY) { /*piece moves left down*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            i = 0;

            for (s = m.startY; s >= m.destY; s--) {
                sqs[i].y = s;
                sqs[i].x = m.startX + (s - m.startY);
          //      printf("\nX: %d\n", sqs[i].x);
                i++;
            }
          return wchecker(i, sqs, ps, m, player, opponent);
      }
    }
    return 0;

}

int pawnValid(struct Move m, struct Piece* ps, int player, int opponent) {

    /*if piece is captured, controlled by opponent or doesnt move, abort*/
    if (m.startY == m.destY || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == opponent) {
        return 0;
    }
    if (player == 0) {
    if (m.destY == m.startY + 2 && m.destX == m.startX && ps[m.pieceID].moved == 0) {
        int i;
        for (i = 0; i < 32; i++) {
            if (ps[i].xpos == m.startX && i != m.pieceID) { /*make sure only other pieces are searched*/
                if(ps[i].ypos == m.startY + 1 || ps[i].ypos == m.destY) {
                    return 0; /*move invalid: blocked by other piece*/
                }

            }

        }
        return 1; /*move valid: pawn moves 2 squares forward*/
    }
    else if (m.destY == m.startY + 1 && m.destX == m.startX) {
        int i;
        for (i = 0; i < 32; i++) {
            if (ps[i].xpos == m.startX && i != m.pieceID && ps[i].ypos == m.destY && ps[i].captured == 0) {
                return 0; /*move invalid: blocked by other piece*/
            }

        }
        return 1; /*move valid: pawn moves 1 square forward*/
    }
    else if (m.destY == m.startY + 1 && m.startX != m.destX) {
        if (m.destX == m.startX + 1) { /*pawn captures right*/
            int i;
            for (i = 0; i < 32; i++) {
                if (ps[i].xpos == m.destX && ps[i].ypos == m.destY && ps[i].captured == 0 && ps[i].owner == opponent) {
                    return 1; /*capture piece on right*/
                }
            }
        }

        else if (m.destX == m.startX - 1) { /*pawn captures left*/
            int i;
            for (i = 0; i < 32; i++) {
                if (ps[i].xpos == m.destX && ps[i].ypos == m.destY && ps[i].captured == 0 && ps[i].owner == opponent) {
                    return 1; /*capture piece on left*/
                }
            }
        }
        else {
            return 0; /*No piece to capture: invalid move*/
        }
    }
    }
    else if (player == 1) {
        if (m.destY == m.startY - 2 && m.destX == m.startX && ps[m.pieceID].moved == 0) {
    int i;
    for (i = 0; i < 32; i++) {
        if (ps[i].xpos == m.startX && i != m.pieceID) { /*make sure only other pieces are searched*/
            if(ps[i].ypos == m.startY - 1 || ps[i].ypos == m.destY) {
                return 0; /*move invalid: blocked by other piece*/
            }
        }
    }
    return 1; /*move valid: pawn moves 2 squares forward*/
}
else if (m.destY == m.startY - 1 && m.destX == m.startX) {
    int i;
    for (i = 0; i < 32; i++) {
        if (ps[i].xpos == m.startX && i != m.pieceID && ps[i].ypos == m.destY && ps[i].captured == 0) {
            return 0; /*move invalid: blocked by other piece*/
        }
    }
    return 1; /*move valid: pawn moves 1 square forward*/
}
else if (m.destY == m.startY - 1 && m.startX != m.destX) {
    if (m.destX == m.startX + 1) { /*pawn captures right*/
        int i;
        for (i = 0; i < 32; i++) {
            if (ps[i].xpos == m.destX && ps[i].ypos == m.destY && ps[i].captured == 0 && ps[i].owner == opponent) {
                return 1; /*capture piece on right*/
            }
        }
    }
    else if (m.destX == m.startX - 1) { /*pawn captures left*/
        int i;
        for (i = 0; i < 32; i++) {
            if (ps[i].xpos == m.destX && ps[i].ypos == m.destY && ps[i].captured == 0 && ps[i].owner == opponent) {
                return 1; /*capture piece on left*/
            }
        }
    }
    else {
        return 0; /*No piece to capture: invalid move*/
    }
}

    }
    return 0; /*none of the cases aboove applied: invalid move*/
}


int knightValid(struct Move m, struct Piece* ps, int player, int opponent) {
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == opponent) {
       // printf("error here");
        return 0;
    }
    if ((m.destX == m.startX + 2 && m.destY == m.startY + 1) || /*two right, one up*/
    (m.destX == m.startX + 2 && m.destY == m.startY - 1) || /*two right, one down*/
    (m.destX == m.startX - 2 && m.destY == m.startY + 1) || /*two left, one up*/
    (m.destX == m.startX - 2 && m.destY == m.startY - 1) || /*two left, one down*/
    (m.destX == m.startX + 1 && m.destY == m.startY + 2) || /*one right, two up*/
    (m.destX == m.startX + 1 && m.destY == m.startY - 2) || /*one right, two down*/
    (m.destX == m.startX - 1 && m.destY == m.startY + 2) || /*one left, two up*/
    (m.destX == m.startX - 1 && m.destY == m.startY - 2)) { /*one left, two down*/

    int p;
    for (p = 0; p < 32; p++) {
        if (ps[p].xpos == m.destX && ps[p].ypos == m.destY && ps[p].owner == player) {
            return 0; /*square is blocked*/
        }
    }
    return 1;
    }
    return 0;
}



int kingValid(struct Move m, struct Piece* ps, int player, int opponent) {
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == opponent) {
     //   printf("error here");
        return 0;
    }

    if ((m.destX == m.startX + 1 && m.destY == m.startY) ||
    (m.destX == m.startX - 1 && m.destY == m.startY) ||
    (m.destX == m.startX && m.destY == m.startY + 1) ||
    (m.destX == m.startX && m.destY == m.startY - 1) ||
    (m.destX == m.startX + 1 && m.destY == m.startY + 1) ||
    (m.destX == m.startX + 1 && m.destY == m.startY - 1) ||
    (m.destX == m.startX - 1 && m.destY == m.startY + 1) ||
    (m.destX == m.startX - 1 && m.destY == m.startY - 1)) {

    for (int p = 0; p < 32; p++) {
        if (ps[p].xpos == m.destX && ps[p].ypos == m.destY && ps[p].owner == player && ps[p].captured == 0) {
            return 0;
        }

    }
    return 1;
}
if (m.startY == m.destY && ps[m.pieceID].moved == 0 && (ps[m.pieceID].ypos == 1 || ps[m.pieceID].ypos == 8)) {
    struct Piece rook;
    if (m.destX == m.startX + 2) {
        for (int p = 0; p < 32; p++) {
            if (ps[p].xpos == 8 && ps[p].ypos == ps[m.pieceID].ypos && ps[p].owner == player && ps[p].captured == 0) {
                rook = ps[p];
            }
        }
    }
    else if (m.destX == m.startX - 2) {
        for (int p = 0; p < 32; p++) {
            if (ps[p].xpos == 1 && ps[p].ypos == ps[m.pieceID].ypos && ps[p].owner == player && ps[p].captured == 0) {
                rook = ps[p];
            }
        }
    }
    if (rook.moved == 1) {
        return 0;
    }
    int ncastleway = 0;
    int castleway[5];
    int rookID;
    for (int r = 0; r < 32; r++) {
        if (ps[r].xpos == rook.xpos && ps[r].ypos == rook.ypos && ps[r].captured == 0 && ps[r].owner == player) {
            rookID = r;
        }
    }
    if (m.destX == m.startX + 2) {
        ncastleway = 4;
        for (int i = 0; i < 4; i++) {
            castleway[i] = 5 + i;
        }
    }
    else if (m.destX == m.startX - 2) {
        ncastleway = 5;
        for (int i = 0; i < 5; i++) {
            castleway[i] = 5 - i;
        }
    }

    if (ncastleway != 0) {
        for (int s = 0; s < ncastleway; s++) {
            //printf("\n\nS: %d\n\n", s);
            //printf("\nCAST: %d\n", castleway[s]);
        for (int p = 0; p < 32; p++) {
            if (ps[p].ypos == ps[m.pieceID].ypos && ps[p].xpos == castleway[s] && ps[p].captured == 0 && p != m.pieceID && p != rookID) {
                //printf("test: \n%d", ps[p].xpos == castleway[s]);
                return 0;
            }
            struct Move mv;
            mv.startX = ps[p].xpos;
            mv.startY = ps[p].ypos;
            mv.destX = castleway[s];
            mv.destY = 1;
            mv.pieceID = p;
            //printf("mv.startX = %d;\n", mv.startX);
            //printf("mv.startY = %d;\n", mv.startY);
            //printf("mv.destX = %d;\n", mv.destX);
            //printf("mv.destY = %d;\n", mv.destY);
            //printf("mv.pieceID = %d;\n", mv.pieceID);
            //printf("\n\nBEFOREID: %d\n\nDEST: %d%d\n----", mv.pieceID, mv.destX, mv.destY);
            if (mValid(mv, ps, 1) == 1) {
              //  printf("\nPiece no %d\n", mv.pieceID);
                return 0;
            }
          //  printf("\n\nAFTER\n\n");
        }
        }
    return 1;
    }
}
return 0;
}

int rookValid(struct Move m, struct Piece* ps, int player, int opponent) {
    /*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == opponent) {
       // printf("error here");
        return 0;
    }

    int index = 0;

    if (m.startX == m.destX) { /*rook moves on Y axis*/

        if (m.startY < m.destY) {
            struct Square sqs[m.destY - m.startY];
            for (int i = m.startY + 1; i <= m.destY; i++) {
                sqs[index].x = m.startX;
                sqs[index].y = i;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
        else if (m.startY > m.destY) {
            struct Square sqs[m.startY - m.destY];
            for (int i = m.startY - 1; i >= m.destY; i--) {
                sqs[index].x = m.startX;
                sqs[index].y = i;;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
    }
    else if (m.startY == m.destY) { /*rook moves on x axis*/
        if (m.startX < m.destX) {
            struct Square sqs[m.destX - m.startX];
            for (int i = m.startX + 1; i <= m.destX; i++) {
                sqs[index].x = i;
                sqs[index].y = m.startY;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
        else if (m.startX > m.destX) {
            struct Square sqs[m.startX - m.destX];
            for (int i = m.startX - 1; i >= m.destX; i--) {
                sqs[index].x = i;
                sqs[index].y = m.startY;
                index++;
            }
            return wchecker(index, sqs, ps, m, player, opponent);
        }
    }
    return 0;
}

int bishopValid(struct Move m, struct Piece* ps, const int pl, int op) {
    const int opponent = op;
    const int player = pl; /*somehow, the player variable is changing so it is now a const*/

/*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner != player) {
      //  printf("error here");
        return 0;
    }

    if (abs(m.destX - m.startX) == abs(m.destY - m.startY)) { /*check if piece moves on a diagonal*/
        int i = 0; /*index for sqs array*/
        int s; /*integet holding temp. square*/
        if (m.destX > m.startX && m.destY > m.startY) { /*piece moves right up*/
            struct Square sqs[m.destX - m.startX]; /*allocate array of squares each holding an x and y value*/
            for (s = m.startX; i <= m.destX - m.startX; s++) {
                   sqs[i].x = s;
                   sqs[i].y =  m.startY + (s - m.startX);
                   i++;
            }

            return wchecker(i, sqs, ps, m, player, opponent);
        }

        else if (m.destX > m.startX && m.destY < m.startY) { /*piece moves right down*/
            struct Square sqs[m.destX - m.startX]; /*allocate array of squares each holding an x and y value*/
            for (s = m.startX; i <= m.destX - m.startX; s++) {
                   sqs[i].x = s;
                   sqs[i].y =  m.startY - (s - m.startX);
                   i++;
            }
            return wchecker(i, sqs, ps, m, player, opponent);
        }
        else if (m.destX < m.startX && m.destY > m.startY) { /*piece moves left up*/
         //   printf("test");
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            for (s = m.startY; i <= m.destY - m.startY; s++) {
                sqs[i].y = s;
                //printf("\nY: %d\n", sqs[i].y);
                sqs[i].x =  m.startX - (s - m.startY);
                //printf("\nX: %d\n", sqs[i].x);
                //printf("INDEX: %d\n", i);
                i++;
            }
            for (int j = 0; j < i; j++) {
             //   printf("\n%d/%d\n", sqs[i].x, sqs[i].y);
            }
            return wchecker(i, sqs, ps, m, player, opponent);

        }
        else if (m.destX < m.startX && m.destY < m.startY) { /*piece moves left down*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            i = 0;

            for (s = m.startY; s >= m.destY; s--) {
                sqs[i].y = s;
                sqs[i].x = m.startX + (s - m.startY);
            //    printf("\nX: %d\n", sqs[i].x);
                i++;
            }
          return wchecker(i, sqs, ps, m, player, opponent);
      }
    }
    return 0;
}

int mValid(struct Move m, struct Piece* ps, const int pl) { /*function to validate moves*/
   const int player = pl;
   int oopponent;
   if (player == 0) {
        oopponent = 1;
   }
   else {
        oopponent = 0;
   }
   const int opponent = oopponent;
   struct Piece p = ps[m.pieceID];
   if (p.xpos != m.startX || p.ypos != m.startY) {
    printf("\n\nError: piece not initalized\n\n"); /*error handler*/
    return 0;
   }
   /*switch statement to select the right validation function dependend on the type of the piece*/
   switch (p.type) {
    case 0:
        return pawnValid(m, ps, player, opponent); /*validate pawn move*/

    case 1:
        return rookValid(m, ps, player, opponent); /*validate rook move*/

    case 2:
        return knightValid(m, ps, player, opponent);

    case 3:
        return bishopValid(m, ps, player, opponent);

    case 4:
        return queenValid(m, ps, player, opponent);

    case 5:
        return kingValid(m, ps, player, opponent);

    default:
        return 0;
    }

}

int getPieceValue(int type) {
    switch(type) {
        case PAWN:   return 100;    // 1 pawn = 100 centipawns
        case KNIGHT: return 300;    // 3 pawns = 300 centipawns
        case BISHOP: return 300;    // 3 pawns = 300 centipawns
        case ROOK:   return 500;    // 5 pawns = 500 centipawns
        case QUEEN:  return 900;    // 9 pawns = 900 centipawns
        case KING:   return 100000; // King is worth a very large number (e.g., 100,000 centipawns)
        default:     return 0;
    }
}
int materialCounter(struct Piece* ps, int player) {
    int playerMaterial = 0;
    int opponentMaterial = 0;
    for (int i = 0; i < 32; i++) {
        if (ps[i].owner == player && ps[i].captured == 0) {
            playerMaterial += getPieceValue(ps[i].type);
        }
        else if (ps[i].owner != player && ps[i].captured == 0) {
            opponentMaterial += getPieceValue(ps[i].type);
        }
    }
   // printf("\n%d", playerMaterial - opponentMaterial);
    return playerMaterial - opponentMaterial;
}


int eval(struct Piece* ps, int player) {
    int opponent = abs(player - 1); /*opponent gets declared (if player is 1, opponent is 0 and vice versa)*/

    int score; /*players score*/
    score = materialCounter(ps, player);
    return materialCounter(ps, player);


}


struct Piece* makeMove(struct Move mv, struct Piece* ps, int player) {
    ps[mv.pieceID].xpos = mv.destX;
    ps[mv.pieceID].ypos = mv.destY;
    ps[mv.pieceID].moved = 1;
    for (int p = 0; p < 32; p++) {
        if (ps[p].xpos == mv.destX && ps[p].ypos == mv.destY && ps[p].captured == 0 && ps[p].owner != player) { /*workaround fix*/
            ps[mv.pieceID].moved = 1;
            ps[p].captured = 1; /*piece gets captured*/
          //  printf("\nPiece no: %d/%d|%d/%d\n", mv.pieceID, p, mv.destX, mv.destY);
        }

    }
    if (ps[mv.pieceID].type == 5 && mv.destX == mv.startX + 2) {
        for (int p = 0; p < 32; p++) {
            if (ps[p].xpos == mv.startX + 3 && ps[p].ypos == mv.startY) {
                ps[p].xpos = ps[mv.pieceID].xpos + 1;
                ps[p].moved = 1;
            }
        }
    }
    else if (ps[mv.pieceID].type == 5 && mv.destX == mv.startX - 2) {
        for (int p = 0; p < 32; p++) {
            if (ps[p].xpos == mv.startX - 4 && ps[p].ypos == mv.startY && ps[p].owner == ps[mv.pieceID].owner) {
                ps[p].xpos = ps[mv.pieceID].xpos + 1;
                ps[p].moved = 1;
            }
        }
    }
    return ps;
}


struct Move* getMoves(struct Piece* ps, int player) {
     struct Move mv;
     struct Move* mvs = (struct Move*)calloc(218, sizeof(struct Move)); /*array of valid moves*/
     int nom = 0; /*number of moves*/
     for (int p = 0; p < 32; p++) {
        for (int x = 1; x < 9; x++) {
            for (int y = 1; y < 9; y++) {
                mv.startX = ps[p].xpos;
                mv.startY = ps[p].ypos;
                mv.destX = x;
                mv.destY = y;
                mv.pieceID = p;
             //   printf("\n%d", mv.pieceID);
                //printf("\nnom: %d\n", testmove(ps, mv, player));
                if (mValid(mv, ps, player) == 1/* && testmove(ps, mv, player) > MIN_ADVANTAGE*/) {
                    mvs[nom] = mv;
                    nom++;
              //      printf("test");

                }
                mvs[0].arlen = nom;
            }
        }
    }
    return mvs;
}
struct State* getstates(struct Piece* ps, const int pl, const int depth) { /*function for getting game "states" aka nodes of a specific position that will later be used in a tree for minimax*/
    const int player = pl;

    int op;
    if (player == 1) {
        op = 0;
    }
    else if (player == 0) {
        op = 1;
    }
    const int opponent = op;

    struct State* states = (struct State*)calloc(218, sizeof(struct State));
    struct Move* pmvs = getMoves(ps, player);
    for (int i = 0; i < pmvs[0].arlen; i++) {
        states[i].m = pmvs[i];
    //    memcpy(states[i].ps, ps, sizeof(*ps)); /*create a temporary instance of pieces for testing moves without affecting the main set*/
        for (int p = 0; p < 32; p++) {
            states[i].ps[p] = ps[p];
        }
        makeMove(states[i].m, states[i].ps, player);
        states[i].score = eval(states[i].ps, player);

    }

    return states;
}

struct Piece* loadPiecesFromFEN(const char* fen) {
    struct Piece* ps = (struct Piece*)calloc(32, sizeof(struct Piece));

    // Indexes for different piece types
    int white_pawn_index = 0;
    int white_other_index = 8;
    int black_pawn_index = 16;
    int black_other_index = 24;

    // Initialize all pieces as captured by default
    for (int i = 0; i < 32; i++) {
        ps[i].captured = 1;  // All pieces are considered captured at the start
    }

    // Extract the piece positions part of the FEN string (before the first space)
    const char* piece_positions = strtok(strdup(fen), " ");
    int xpos = 1;  // Files (columns)
    int ypos = 8;  // Ranks (rows)

    // Loop through the FEN string
    for (int i = 0; piece_positions[i] != '\0'; i++) {
        char c = piece_positions[i];

        if (c == '/') {
            // Move to the next rank
            ypos--;
            xpos = 1;
        } else if (c >= '1' && c <= '8') {
            // Skip empty squares
            xpos += c - '0';
        } else {
            // Initialize a piece
            struct Piece p;
            p.captured = 0;  // This piece is not captured
            p.moved = 0;     // Assume not moved yet
            p.xpos = xpos++;  // Set the x position (file)
            p.ypos = ypos;    // Set the y position (rank)

            // Determine the owner
            if (isupper(c)) {
                p.owner = 0;  // White piece
            } else {
                p.owner = 1;  // Black piece
            }

            // Determine the piece type
            switch (tolower(c)) {
                case 'p': p.type = PAWN; break;
                case 'r': p.type = ROOK; break;
                case 'n': p.type = KNIGHT; break;
                case 'b': p.type = BISHOP; break;
                case 'q': p.type = QUEEN; break;
                case 'k': p.type = KING; break;
            }

            // Place the piece in the correct part of the array (pawns vs other pieces)
            if (p.type == PAWN) {
                if (p.owner == 0) {
                    ps[white_pawn_index++] = p;  // White pawn
                } else {
                    ps[black_pawn_index++] = p;  // Black pawn
                }
            } else {
                if (p.owner == 0) {
                    ps[white_other_index++] = p;  // White non-pawn piece
                } else {
                    ps[black_other_index++] = p;  // Black non-pawn piece
                }
            }
        }
    }

    // Mark pawns as having moved if they are not on their initial ranks
    for (int i = 0; i < 32; i++) {
        if (ps[i].type == PAWN && ((ps[i].ypos != 2 && ps[i].owner == 0) || (ps[i].ypos != 7 && ps[i].owner == 1))) {
            ps[i].moved = 1;
        }
    }

    return ps;
}


int treeBuilder(struct State *rootNode, int player, int prune) {
    struct State* states = getstates(rootNode->ps, player, 0);
    int nstlen = 0; /*new stlen for pruned array*/
    rootNode->stlen = getMoves(rootNode->ps, player)[0].arlen;
    for (int i = 0; i < rootNode->stlen/* && states[i].score - states[i].bscore > MIN_ADVANTAGE*/; i++) {
        rootNode->children[i] = &states[i];
    }
    return 0;
}



int minimax(struct State* rootNode, struct Move *bestMove, int pl, int depth, int maximizing) {
    int opponent = 0;
    if (pl == 0) {
        opponent = 1;
    }
    if (depth == 0) {
        return rootNode->score;
    }
    if (maximizing == 1) {
        int max_eval = -INFINITY;
        for (int i = 0; i < rootNode->stlen; i++) {
            int evaluation = minimax(rootNode->children[i], bestMove, pl, depth - 1, 0);
            max_eval = MAX(max_eval, evaluation);
        }
        return max_eval;

    }
    else if (maximizing == 0) {
        int min_eval = INFINITY;
        for (int i = 0; i < rootNode->stlen; i++) {
            int evaluation = minimax(rootNode->children[i], bestMove, pl, depth - 1, 1);
            min_eval = MIN(min_eval, evaluation);
        }
        return min_eval;
    }



}

struct Move getBestMove(struct State* rootNode, int pl, int depth) {
    struct Move bestMove;
    int old_max_eval;
    for (int i = 0; i < rootNode->stlen; i++) {
        int max_eval = -INFINITY;
        for (int i = 0; i < rootNode->stlen; i++) {
            int evaluation = minimax(rootNode->children[i], &rootNode->m, pl, depth - 1, 0);
            old_max_eval = max_eval;
            max_eval = MAX(max_eval, evaluation);
            if (max_eval != old_max_eval) {
                bestMove = rootNode->children[i]->m;
            }
        }
        return bestMove;
    }
}

int buildFullTree(struct State *rootNode, const int pl, int depth) {
    int op = 0;

    if (pl == 0) {
            op = 1;
    }
    int prune = 0;
    rootNode->score = eval(rootNode->ps, 0);
    const int opponent = op;
    const int player = pl;


    if (depth > 0) {
        treeBuilder(rootNode, player, prune); /*the prune value is then given to the treeBuilder to ignore bad branches*/
        depth--;
        for (int node = 0; node < rootNode->stlen; node++) {
         //   treeBuilder(rootNode->children[node], opponent);
         buildFullTree(rootNode->children[node], opponent, depth); /*recursively call this function to continue building new branches*/

        }

    }
    return 0;
}

void cli() {
    system("clear");
    printf("\n Welcome to C-Chess!\n");
    const char *chess_knight_art =
        "  |\\_\n"
        " /  .\\_\n"
        "|   ___)\n"
        "|    \\\n"
        "|  =  |\n"
        " /_____\\\n"
        "[_______]\n";

    // Print the ASCII art
    printf("%s", chess_knight_art);
    char fen[256];
    printf("\nengine plays as white\n");
    printf("Enter FEN ->");
    fgets(fen, sizeof(fen), stdin);
        size_t length = strlen(fen);
        if (length > 0 && fen[length - 1] == '\n') {
        fen[length - 1] = '\0';
        }
        struct Piece* ps = loadPiecesFromFEN(fen); /*initalize pointer to pieces array allocated on heap*/
    for (int i = 0; i < 32; i++) {
        printf("type: %d\n", ps[i].captured);
    }
        printf("\neval: %d", eval(ps, 0));
        struct State rootNode;
        for (int i = 0; i < 32; i++) {
        rootNode.ps[i] = ps[i];
        }
        printf("\nthinking...");
        buildFullTree(&rootNode, 0, DEPTH);
        for (int i = 0; i < rootNode.stlen; i++) {
            if (rootNode.children[i]->m.startX == 1) {
                printf("\nX: %d Y; %d\n", rootNode.children[i]->score, rootNode.children[i]->m.destY);
            }
        }
        struct Move bestMove = getBestMove(&rootNode, 0, DEPTH);
        printf("\n%d%d\n%d%d\n", bestMove.startX, bestMove.startY, bestMove.destX, bestMove.destY);


        free(ps);
    }
