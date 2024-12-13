# chess-engine

# features
- move generation and validation.
- evaluation based on material, piece development, center control.
- evaluation using minimax.
# roadmap
in the near future, i plan on implementing alpha beta pruning and the UCI protocol

# usage
compile main.c (no flags needed) and run it.
tested with gcc on gentoo 6.9.10

# dependencies
no additional dependencies aside from gnulibc, math, ctype, string, and stdio are needed (it compiles on any modern GNU/linux system)
# known issues
1. the engine sees stalemates as checkmates.
2. threefold repetition and en passant arent implemented.
let me know if you encounter any more issues via githubs issue feature or via email at contact@kuglatec.com

