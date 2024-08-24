# chess-engine

# features
move generation and validation
evaluation based on material, piece development and center control
evaluation based on minimax
function for finding the best move in any position defined by a FEN string

# roadmap

in the near future, i plan to implement alpha beta pruning and the UCI protocol

# usage
compile main.c (no flags needed) and run it
tested with gcc on gentoo 6.9.10

# dependencies
no additional dependencies aside from gnulibc, math, ctype, string, and stdio are needed (it compiles on any modern GNU/linux system)

# hardware requirements
the system the engine is running on should have at least 4 gb of ram available when running on depth 4. increasing the depth above 4 is not recommended due to heavy ram usage (20gb+)
