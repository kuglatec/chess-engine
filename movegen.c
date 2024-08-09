#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
/*init values for pieces in decipawns*/
#define QUEEN 90 
#define ROOK 50
#define PAWN 10
#define KNIGHT 30
#define BISHOP 32
#define KING 10000

struct Square kqs[2]; /*keysquares (D & E 4 or D & E 5)*/
struct Square iqs[2]; /*important squares (C & F 4 or C & F 5)*/


int mValid(struct Move m, struct Piece* ps, int player);

int wchecker(int i, struct Square* sqs, struct Piece* ps, struct Move m, int player, int opponent) {
    int p;
    for (int j = 0; j <= i; j++) {
     //   printf("\n%d/%d\n", sqs[j].x, sqs[j].y);
        if (!(sqs[j].x == m.startX && sqs[j].y == m.startY)) {
            for (p = 0; p < 32; p++) {
                if (ps[p].xpos == sqs[j].x && ps[p].ypos == sqs[j].y && ps[p].captured == 0) {
                     if (!(ps[p].ypos == m.destY && ps[p].xpos == m.destX) || ((ps[p].xpos == m.destX && ps[p].ypos == m.destY) && ps[p].owner == player)) {
                            return 0; 
                        }
                }
            }
        } 
    } 
    return 1;
}
int queenValid(struct Move m, struct Piece* ps, int player, int opponent) {
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
                printf("\nPiece no %d\n", mv.pieceID);
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
            printf("test: %d", m.startX - m.destX);
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

int bishopValid(struct Move m, struct Piece* ps, int player, int opponent) {
/*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == opponent) {
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
            //    printf("\nX: %d\n", sqs[i].x);
                i++;
            }
          return wchecker(i, sqs, ps, m, player, opponent); 
      }  
    } 
    return 0;
}

int mValid(struct Move m, struct Piece* ps, int player) { /*function to validate moves*/
   int opponent = abs(player - 1); /*if player is 1: opponent is 0 and vice versa*/
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


int materialCounter(struct Piece* tps) {
    int tmpscore = 0;
    for (int p = 0; p < 16; p++) {
        if (tps[p].captured == 0) {
            switch (tps[p].type) {
                case 0:
                    tmpscore += PAWN;
                    break;
                case 1:
                    tmpscore += ROOK;
                    break;
                case 2:
                    tmpscore += KNIGHT;
                    break; 
                case 3:
                    tmpscore += BISHOP;
                    break; 
                case 4:
                    tmpscore += QUEEN;
                    break; 
                case 5:
                    tmpscore += KING;
                    break; 
                default:
                    break;
            }
        }
    }
    return tmpscore;
}


int eval(struct Piece* ps, int player) {
    int opponent = abs(player - 1); /*opponent gets declared (if player is 1, opponent is 0 and vice versa)*/
    int score; /*players score*/
    int opscore; /*opponents score*/
    int tmpscore = 0; /*temporary score buffer*/
    struct Piece* pps = (struct Piece*)calloc(16, sizeof(struct Piece)); /*player pieces*/
    struct Piece* ops = (struct Piece*)calloc(16, sizeof(struct Piece)); /*opponent pieces*/
    int npps = 0;
    int nops = 0;
    for (int p = 0; p < 32; p++)
        if (ps[p].owner == player) {
            pps[npps] = ps[p];
            npps++;
        }
        else if (ps[p].owner == opponent) {
            ops[nops] = ps[p];
            nops++;
        }
    
    int pmoves = 0; /*number of possible moves*/
    struct Move mv;
    

    score = materialCounter(pps);
    for (int p = 0; p < 32; p++) {
        for (int x = 1; x < 9; x++) {
            for (int y = 1; y < 9; y++) {
                mv.startX = ps[p].xpos;
                mv.startY = ps[p].ypos;
                mv.destX = x;
                mv.destY = y;
                mv.pieceID = p;
                if (mValid(mv, ps, player) == 1) {
                    score = score + 1;
                    printf("\n\nPIECEID: %d\n\n", p);
                }
            }
        }
    }
    
    opscore = materialCounter(ops);
    score = score - opscore;
    return score;
    

}


struct Piece* loadpieces() {
    struct Piece* ps = (struct Piece*)calloc(32, sizeof(struct Piece)); /*allocate memory for 32 pieces*/
    /*temporary workaround for setting the state of pieces hardcoded*/
   /*TODO: implement FEN and UCI*/    

    // White pieces
// White pieces
// Pawns
for (int i = 0; i < 8; i++) {
    ps[i].type = 0; // Pawn
    ps[i].owner = 0; // White
    ps[i].captured = 0;
    ps[i].moved = 0;
    ps[i].xpos = i + 1;
    ps[i].ypos = 2;
}

// Rooks
ps[8].type = 1; // Rook
ps[8].owner = 0; // White
ps[8].captured = 0;
ps[8].moved = 0;
ps[8].xpos = 1;
ps[8].ypos = 1;

ps[9].type = 1; // Rook
ps[9].owner = 0; // White
ps[9].captured = 0;
ps[9].moved = 0;
ps[9].xpos = 8;
ps[9].ypos = 1;

// Knights
ps[10].type = 2; // Knight
ps[10].owner = 0; // White
ps[10].captured = 0;
ps[10].moved = 0;
ps[10].xpos = 2;
ps[10].ypos = 1;

ps[11].type = 2; // Knight
ps[11].owner = 0; // White
ps[11].captured = 0;
ps[11].moved = 0;
ps[11].xpos = 7;
ps[11].ypos = 1;

// Bishops
ps[12].type = 3; // Bishop
ps[12].owner = 0; // White
ps[12].captured = 0;
ps[12].moved = 0;
ps[12].xpos = 3;
ps[12].ypos = 1;

ps[13].type = 3; // Bishop
ps[13].owner = 0; // White
ps[13].captured = 0;
ps[13].moved = 0;
ps[13].xpos = 6;
ps[13].ypos = 1;

// Queens
ps[14].type = 4; // Queen
ps[14].owner = 0; // White
ps[14].captured = 0;
ps[14].moved = 0;
ps[14].xpos = 4;
ps[14].ypos = 1;

// Kings
ps[15].type = 5; // King
ps[15].owner = 0; // White
ps[15].captured = 0;
ps[15].moved = 0;
ps[15].xpos = 5;
ps[15].ypos = 1;

// Black pieces
// Pawns
for (int i = 16; i < 24; i++) {
    ps[i].type = 0; // Pawn
    ps[i].owner = 1; // Black
    ps[i].captured = 0;
    ps[i].moved = 0;
    ps[i].xpos = (i - 16) + 1;
    ps[i].ypos = 7;
}

// Rooks
ps[24].type = 1; // Rook
ps[24].owner = 1; // Black
ps[24].captured = 0;
ps[24].moved = 0;
ps[24].xpos = 1;
ps[24].ypos = 8;

ps[25].type = 1; // Rook
ps[25].owner = 1; // Black
ps[25].captured = 0;
ps[25].moved = 0;
ps[25].xpos = 8;
ps[25].ypos = 8;

// Knights
ps[26].type = 2; // Knight
ps[26].owner = 1; // Black
ps[26].captured = 0;
ps[26].moved = 0;
ps[26].xpos = 2;
ps[26].ypos = 8;

ps[27].type = 2; // Knight
ps[27].owner = 1; // Black
ps[27].captured = 0;
ps[27].moved = 0;
ps[27].xpos = 7;
ps[27].ypos = 8;

// Bishops
ps[28].type = 3; // Bishop
ps[28].owner = 1; // Black
ps[28].captured = 0;
ps[28].moved = 0;
ps[28].xpos = 3;
ps[28].ypos = 8;

ps[29].type = 3; // Bishop
ps[29].owner = 1; // Black
ps[29].captured = 0;
ps[29].moved = 0;
ps[29].xpos = 6;
ps[29].ypos = 8;

// Queens
ps[30].type = 4; // Queen
ps[30].owner = 1; // Black
ps[30].captured = 0;
ps[30].moved = 0;
ps[30].xpos = 4;
ps[30].ypos = 8;

// Kings
ps[31].type = 5; // King
ps[31].owner = 1; // Black
ps[31].captured = 0;
ps[31].moved = 0;
ps[31].xpos = 5;
ps[31].ypos = 8;

                    
    return ps;
 }
