#include "io.h"
#include "boardutils.h"
#include "pvtable.h"
#include "move.h"
#include "search.h"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <thread>

char charPiece[] = ".PNBRQKpnbrqkx";
char charSide[] = "wb-";
char charRank[] = "123456RANK_88";
char charFile[] = "abcdefgh";

bool running = false;
bool isSearching = false;
Board currentBoard;
SearchInfo searchInfo[1];
std::thread *threadSearch;

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

void print(const std::string input) {
    printBoard(currentBoard);
}

void fen(const std::string line) {
    if (line.length() > 0) {
        try {
            const std::string fen = line.substr(1);
            currentBoard.parseFen(fen);
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Invalid FEN!" << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
}

void reset(const std::string line) {
    currentBoard.reset();
    currentBoard.initHashTable();
}

void startSearch(const std::string line) {
    searchInfo->stop = false;
    isSearching = true;
    std::istringstream(line.substr(1)) >> searchInfo->maxDepth;
    threadSearch = new std::thread(search, &currentBoard, searchInfo);
}

void stopSearch(const std::string line) {
    searchInfo->stop = true;
    if (isSearching) {
        threadSearch->join();
        delete(threadSearch);
        isSearching = false;
    }
}

void startInputLoop() {
    std::unordered_map<std::string, std::function<void(const std::string)>> commands;
    commands.emplace("quit", quit);
    commands.emplace("exit", quit);
    commands.emplace("print", print);
    commands.emplace("fen", fen);
    commands.emplace("position fen", fen);
    commands.emplace("reset", reset);
    commands.emplace("search", startSearch);
    commands.emplace("stop", stopSearch);

    running = true;
    while (running) {
        std::string line;
        getline(std::cin, line);

        for (auto it = commands.begin(); it != commands.end(); it++) {
            const std::string command = it->first;
            const auto func = it->second;
            if (!line.substr(0, command.length()).compare(command)) {
                func(line.substr(command.length()));
            }
        }
    }
}

std::string prMove(const int move) {
    int ff = getFile[FROMSQ(move)];
    int rf = getRank[FROMSQ(move)];
    int ft = getFile[TOSQ(move)];
    int rt = getRank[TOSQ(move)];

    int promoted = PROM(move);

    std::ostringstream result;

    result << ('a' + ff) << ('1' + rf) << ('a' + ft) << ('1' + rt);

    if (promoted) {
        result << charPiece[promoted];
    }

    return result.str();
}

void printPvLine(Board* board, int depth) {
    int count = getPvLine(board, depth);

    std::cout << " count: " << count;

    for (int i = 0; i < count; i++) {
        std::cout << " " << prMove(board->pvArray[i]);
    }
}
