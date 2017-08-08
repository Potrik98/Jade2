
#include "perft.h"
#include "move.h"

#include <fstream>
#include <sstream>

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

void runPerftTests(const std::string file) {
    std::ifstream infile(file);
    std::string line;
    Board board;
    while (getline(infile, line)) {
        board.parseFen(line);
        printf("%s\n", line.c_str());
        size_t loc = line.find_first_of(';');
        std::string data = line.substr(loc + 1);
        std::istringstream is(data);
        int currentDepth = 0;
        std::string d;
        int nodes;
        while (is >> d >> nodes) {
            if (nodes > 15000000) continue;
            currentDepth++;
            U64 res = perft(currentDepth, &board);
            printf("d%d: %d nodes\n", currentDepth, nodes);
            if (res != nodes) throw std::runtime_error("Perft test failed!");
        }
    }
    printf("Perft tests complete\n");
}
