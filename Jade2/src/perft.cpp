
#include "perft.h"
#include "move.h"

U64 perft(const int depth, Board *board) {
    if (depth == 0) {
        return 1ULL;
    }

    MoveList list[1];
    generateAllMoves(board, list);

    U64 sum = 0ULL;

    int MoveNum = 0;
    for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

        if (!makeMove(board, list->moves[MoveNum].move)) {
            continue;
        }
        U64 nodes = perft(depth - 1, board);

        sum += nodes;
        takeMove(board);
    }

    return sum;
}
