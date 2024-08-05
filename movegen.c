#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

int wchecker(int i, struct Square* sqs, struct Piece* ps, struct Move m) {
    int p;

    for (int j = 0; j <= i; j++) {
        if (sqs[j].x != m.startX && sqs[j].y != m.startY) {
            for (p = 0; p < 32; p++) {
               // printf("\n%d\n", p);
                if (ps[p].xpos == sqs[j].x && ps[p].ypos == sqs[j].y && ps[p].captured == 0) {
                     if ((ps[p].ypos != m.destY && ps[p].xpos != m.destX) || ((ps[p].xpos == m.destX && ps[p].ypos == m.destY) && ps[p].owner == 0)) {
                            return 0; 
                        }
                }
            }
        } 
    } 
    return 1;
}

int ywchecker(int i, int j, int *sqs, struct Piece* ps, struct Move m){ /*function for checking if the piece is blocked when moving on the y axis*/
    int p;
    for (i = 0; i <= j; i++) {
                if (sqs[i] != m.startY) {
                    for (p = 0; p < 32; p++) {
                        if (ps[p].captured == 0 && ps[p].xpos == m.startX) {
                                if (sqs[i] == ps[p].ypos && (ps[p].ypos != m.destY || (ps[p].ypos == m.destY && ps[p].owner == 0))) {
                                    return 0; /*some piece is blocking*/
                                }
                    
                }
            }}}
            return 1; /* no blockers*/
}

int xwchecker(int i, int j, int *sqs, struct Piece* ps, struct Move m){ /*function for checking if the piece is blocked when moving on the y axis*/
    int p;
    for (i = 0; i <= j; i++) {
                if (sqs[i] != m.startX) {
                    for (p = 0; p < 32; p++) {
                        if (ps[p].captured == 0 && ps[p].ypos == m.startY) {
                                if (sqs[i] == ps[p].xpos && (ps[p].xpos != m.destX || (ps[p].xpos == m.destX && ps[p].owner == 0))) {
                                    return 0; /*some piece is blocking*/
                                }
                    
                }
            }}}
            return 1; /* no blockers*/
}


int queenValid(struct Move m, struct Piece* ps) {
    /*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == 1) {
        return 0;
    }
    int dr; /*direction where the rook is moving (0=up, 1=down, 2=right, 3=left, 4=right-up, 5=right-down, 6=left-up, 7=left-down)*/
    if (m.startX == m.destX) { /*rook moves on Y axis*/
        if (m.startY < m.destY) {
            dr = 0; /*rook moves up*/
        }
        else if (m.startY > m.destY) {
            dr = 1; /*rook moves down*/
        }
    }
    else if (m.startY == m.destY) { /*rook moves on x axis*/
        if (m.startX < m.destX) {
            dr = 2; /*rook moves right*/
        } 
        else if (m.startX > m.destX) {
            dr = 3; /*rook moves left*/
        }    
    }
    int i;
    switch (dr) {
        case 0: {
            int sqs[m.destY - m.startY]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            for (i = m.startY; i < m.destY; i++) {
                sqs[j] = i;
                j++;
                printf("\nI: %d\n", i);
            }
            //printf("\nJ: %d\n", j);
            sqs[j] = m.destY;

            return ywchecker(i, j, sqs, ps, m);
        }
        case 1: {
            int sqs[m.startY - m.destY]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            for (i = m.startY; i > m.destY; i--) {
                sqs[j] = i;
                j++;
            }
            sqs[j] = m.destY;
            return ywchecker(i, j, sqs, ps, m);
        }

        case 2: {
            int sqs[m.destX - m.startX]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            int p;
            for (i = m.startX; i > m.destX; i--) {
                sqs[j] = i;
                j++;
            }
            sqs[j] = m.destX;
            return xwchecker(i, j, sqs, ps, m);
        }
        case 3: {
            int sqs[m.startX - m.destX]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            int p;
            for (i = m.startX; i > m.destX; i--) {
                sqs[j] = i;
                j++;
            }
            sqs[j] = m.destX;
            return xwchecker(i, j, sqs, ps, m);
        }

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
            return wchecker(i, sqs, ps, m);
        }
        else if (m.destX > m.startX && m.destY < m.startY) { /*piece moves right down*/
            struct Square sqs[m.destX - m.startX]; /*allocate array of squares each holding an x and y value*/ 
            for (s = m.startX; i <= m.destX - m.startX; s++) {
                   sqs[i].x = s;
                   sqs[i].y =  m.startY - (s - m.startX);
                   i++;
            }
            return wchecker(i, sqs, ps, m);
        }
        else if (m.destX < m.startX && m.destY > m.startY) { /*piece moves left up*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            for (s = m.startY; i <= m.destY - m.startY; s++) {
                sqs[i].y = s;
                sqs[i].x =  m.startX - (s - m.startY);
                i++;
            }
            return wchecker(i, sqs, ps, m);
        
        }
        else if (m.destX < m.startX && m.destY < m.startY) { /*piece moves left down*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            i = 0;

            for (s = m.startY; s >= m.destY; s--) {
                sqs[i].y = s;
                sqs[i].x = m.startX + (s - m.startY);
                printf("\nX: %d\n", sqs[i].x);
                i++;
            }
          return wchecker(i, sqs, ps, m); 
      }  
    } 
    return 0;
}

int pawnValid(struct Move m, struct Piece* ps) {
    
    /*if piece is captured, controlled by opponent or doesnt move, abort*/
    if (m.startY == m.destY || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == 1) {
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
                if (ps[i].xpos == m.destX && ps[i].ypos == m.destY && ps[i].captured == 0 && ps[i].owner == 1) {
                    return 1; /*capture piece on right*/
                }
            }  
        }

        else if (m.destX == m.startX - 1) { /*pawn captures left*/
            int i;
            for (i = 0; i < 32; i++) {
                if (ps[i].xpos == m.destX && ps[i].ypos == m.destY && ps[i].captured == 0 && ps[i].owner == 1) {
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


int knightValid(struct Move m, struct Piece* ps) {
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == 1) {
        printf("error here");
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
        if (ps[p].xpos == m.destX && ps[p].ypos == m.destY && ps[p].owner == 0) {
            return 0; /*square is blocked*/
        }
    }
    return 1;
    }
    return 0;
}



int kingValid(struct Move m, struct Piece* ps) {
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == 1) {
        printf("error here");
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
        if (ps[p].xpos == m.destX && ps[p].ypos == m.destY && ps[p].owner == 0 && ps[p].captured == 0) {
            return 0;
        }
        
    }
    return 1;
}
return 0;
}

int rookValid(struct Move m, struct Piece* ps) {
    /*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == 1) {
        printf("error here");
        return 0;
    }

    int dr; /*direction where the rook is moving (0=up, 1=down, 2=right, 3=left)*/

    if (m.startX == m.destX) { /*rook moves on Y axis*/
        if (m.startY < m.destY) {
            dr = 0; /*rook moves up*/
            printf("\nRook moved %d squares \n", m.destY - m.startY);
        }
        else if (m.startY > m.destY) {
            dr = 1; /*rook moves down*/
        }
    }
    else if (m.startY == m.destY) { /*rook moves on x axis*/
        if (m.startX < m.destX) {
            dr = 2; /*rook moves right*/
        } 
        else if (m.startX > m.destX) {
            dr = 3; /*rook moves left*/
        }    
    }
    int i;
    switch (dr) {
        case 0: {
            int sqs[m.destY - m.startY]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            for (i = m.startY; i < m.destY; i++) {
                sqs[j] = i;
                j++;
                printf("\nI: %d\n", i);
            }
            //printf("\nJ: %d\n", j);
            sqs[j] = m.destY;

            return ywchecker(i, j, sqs, ps, m);
        }
        case 1: {
            int sqs[m.startY - m.destY]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            for (i = m.startY; i > m.destY; i--) {
                sqs[j] = i;
                j++;
            }
            sqs[j] = m.destY;
            return ywchecker(i, j, sqs, ps, m);
        }

        case 2: {
            int sqs[m.destX - m.startX]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            int p;
            for (i = m.startX; i > m.destX; i--) {
                sqs[j] = i;
                j++;
            }
            sqs[j] = m.destX;
            return xwchecker(i, j, sqs, ps, m);
        }
        case 3: {
            int sqs[m.startX - m.destX]; /*initialite array of squares the piece is passing*/
            int j = 0; /*index to iterate the empty array*/
            int p;
            for (i = m.startX; i > m.destX; i--) {
                sqs[j] = i;
                j++;
            }
            sqs[j] = m.destX;
            return xwchecker(i, j, sqs, ps, m);
        }

    }
    printf("\n\nDIR:%d\n\n", dr);

    return 0;
}

int bishopValid(struct Move m, struct Piece* ps) {
/*if piece is captured, controlled by opponent or doesnt move, abort*/
    if ((m.startY == m.destY && m.startX == m.destX) || ps[m.pieceID].captured == 1 || ps[m.pieceID].owner == 1) {
        printf("error here");
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
            return wchecker(i, sqs, ps, m);
        }
        else if (m.destX > m.startX && m.destY < m.startY) { /*piece moves right down*/
            struct Square sqs[m.destX - m.startX]; /*allocate array of squares each holding an x and y value*/ 
            for (s = m.startX; i <= m.destX - m.startX; s++) {
                   sqs[i].x = s;
                   sqs[i].y =  m.startY - (s - m.startX);
                   i++;
            }
            return wchecker(i, sqs, ps, m);
        }
        else if (m.destX < m.startX && m.destY > m.startY) { /*piece moves left up*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            for (s = m.startY; i <= m.destY - m.startY; s++) {
                sqs[i].y = s;
                sqs[i].x =  m.startX - (s - m.startY);
                i++;
            }
            return wchecker(i, sqs, ps, m);
        
        }
        else if (m.destX < m.startX && m.destY < m.startY) { /*piece moves left down*/
            struct Square sqs[m.startX - m.destX]; /*allocate memory*/
            i = 0;

            for (s = m.startY; s >= m.destY; s--) {
                sqs[i].y = s;
                sqs[i].x = m.startX + (s - m.startY);
                printf("\nX: %d\n", sqs[i].x);
                i++;
            }
          return wchecker(i, sqs, ps, m); 
      }  
    } 
    return 0;
}

int mValid(struct Move m, struct Piece* ps, int player) { /*function to validate moves*/
   
   struct Piece p = ps[m.pieceID];
   if (p.xpos != m.startX || p.ypos != m.startY) {
    printf("\n\nError: piece not initalized\n\n"); /*error handler*/
    return 0;
   }
   /*switch statement to select the right validation function dependend on the type of the piece*/
   switch (p.type) {
    case 0:
        return pawnValid(m, ps); /*validate pawn move*/
    
    case 1:
        return rookValid(m, ps); /*validate rook move*/
    
    case 2:
        return knightValid(m, ps);
    
    case 3:
        return bishopValid(m, ps);
    
    case 4:
        return queenValid(m, ps);
    
    case 5:
        return kingValid(m, ps);
    
    default:
        return 0;
    }
    
}  



struct Piece* loadpieces() {
    struct Piece* ps = (struct Piece*)calloc(32, sizeof(struct Piece)); /*allocate memory for 32 pieces*/
    /*temporary workaround for setting the state of pieces hardcoded*/
   /*TODO: implement FEN and UCI*/    
    /*engine pawns*/

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
