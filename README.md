# chess-engine

# features
move generation and validation
evaluation based on material, piece development and center control
evaluation based on minimax
function for finding the best move in any position defined by a FEN string

# roadmap

in the near future, i plan to implement alpha beta pruning and the UCI protocol

# usage
set a FEN string in main.c

compile with gcc main.c

# dependencies
no additional dependencies aside from gnulibc, math, type, string, and stdio are needed (it compiles on any modern GNU/linux system)
