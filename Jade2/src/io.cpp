#include "io.h"
#include "boardutils.h"

#include <iostream>
#include <unordered_map>
#include <functional>

char charPiece[] = ".PNBRQKpnbrqkx";
char charSide[] = "wb-";
char charRank[] = "123456RANK_88";
char charFile[] = "abcdefgh";

bool running = false;
Board currentBoard;

void printBoard(const Board board) {
    int sq, file, rank, piece;

    printf("\ngame board:\n\n");

    for (rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d  ", rank + 1);
        for (file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank);
            piece = board.pieces[sq];
            printf("%3c", charPiece[piece]);
        }
        printf("\n");
    }

    printf("\n   ");
    for (file = FILE_A; file <= FILE_H; file++) {
        printf("%3c", 'a' + file);
    }
    printf("\n\n");
    printf("side: %c\n", charSide[board.sideToMove]);
    printf("en passant: %d\n", board.enPasSquare);
    printf("castle: %c%c%c%c\n",
        board.castlePerm & WKCA ? 'K' : '-',
        board.castlePerm & WQCA ? 'Q' : '-',
        board.castlePerm & BKCA ? 'k' : '-',
        board.castlePerm & BQCA ? 'q' : '-'
    );
    printf("position key: %llX\n", board.posKey);
}

void quit(std::string input) {
    running = false;
}

void print(std::string input) {
    printBoard(currentBoard);
}

void startInputLoop() {
    std::unordered_map<std::string, std::function<void(std::string)>> commands;
    commands.emplace("quit", quit);
    commands.emplace("exit", quit);
    commands.emplace("print", print);

    running = true;
    while (running) {
        std::string line;
        getline(std::cin, line);

        for (auto it = commands.begin(); it != commands.end(); it++) {
            std::string command = it->first;
            auto func = it->second;
            if (!line.substr(0, command.length()).compare(command)) {
                func(line.substr(command.length()));
            }
        }
    }
}