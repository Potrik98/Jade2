#include "pvtable.h"
#include "board.h"
#include "move.h"

int getPvMove(const Board* board) {
    int index = board->posKey % HASH_ENTRIES;
    printf("Getting move at index %d\n", index);
    if (board->hashTable[index].posKey == board->posKey) {
        printf("Found move %d\n", board->hashTable[index].move);
        return board->hashTable[index].move;
    }
    printf("Found nothing :( Expected key %llX but found %llX\n", board->posKey, board->hashTable[index].posKey);
    return 0;
}

int getPvLine(Board* board, const int depth) {
    int move = getPvMove(board);
    int count = 0;

    while (move != 0 && count < depth) {
        if (moveExists(board, move)) {
            makeMove(board, move);
            board->pvArray[count++] = move;
        }
        else break;

        move = getPvMove(board);
    }

    while (board->ply > 0) {
        takeMove(board);
    }

    return count;
}

void storePvMove(Board* board, const int move) {
    int index = board->posKey % HASH_ENTRIES;
    printf("Storing move %d at index %d\n, current key: %llX", move, index, board->posKey);
    board->hashTable[index].move = move;
    board->hashTable[index].posKey = board->posKey;
}
