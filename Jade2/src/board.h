#pragma once

#include "jade.h"

#include <string>

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK, OFFBOARD };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

enum { WHITE, BLACK, BOTH };

enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

typedef struct {
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey;
} S_UNDO;

typedef struct {
    U64 posKey;
    int move;
    int score;
    int depth;
    int flags;
} S_HASH_ENTRY;

class Board
{
public:
    Board();
    ~Board();
    void initHashTable();
    void clearHashTable();
    void clear();
    void reset();
    void parseFen(const std::string fen);

    int pieces[120];
    U64 pawns[3];

    int sideToMove;
    int enPasSquare;
    int fiftyMove;

    int ply;
    int plyTotal;

    int castlePerm;

    U64 posKey;

    int pieceNum[13];
    int pieceBig[2];
    int pieceMaj[2];
    int pieceMin[2];
    int material[2];

    int pieceSquareList[13][10];

    int pvArray[MAX_SEARCH_DEPTH];

    S_UNDO history[1024];
    S_HASH_ENTRY *hashTable;
private:
    void parseRank(const std::string format, const int rank);
    void parseSideToMove(const std::string fen, const int index);
    int parseCastlePermission(const std::string fen, const int index);
    int parseEnPassantSquare(const std::string fen, const int index);
    void parseMoveCounters(const std::string fen, const int index);
    void updatePieceLists();

    bool isHashTableInitialized;
};
