struct Move{
   int pieceID; /*index of the piece in the ps array*/
   int startX; /*initial position at the X axis*/
   int startY; /*inital postion at the Y axis*/
   int destX; /*destination at X axis*/
   int destY; /*destination at Y axis*/
   int arlen; /*length identifier for arrays*/
};

struct Piece{
    int type; // 0 = pawn, 1 = rook, 2 = knight, 3 = bishop, 4 = queen, 5 = king
    int owner; //the owner of the piece 0 = engine, 1 = opponent
    int captured; //the piece has been captured
    int moved; //the piece has moved in the game (important for pawns or castling)
    int xpos; //position on the x axis
    int ypos; //position on the y axis
};

struct Square {
    int x;
    int y;
};

struct State { /*struct for nodes*/
    int stlen; /*length of the returned states array*/
    struct Piece ps[32];
    struct Move m;
    int score;
    struct State* children[256]; /*initialize pointer array to other children of type State*/
};
