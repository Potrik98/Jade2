#pragma once

#include "move.h"
#include "boardutils.h"
#include "piece.h"
#include "hashkey.h"
#include "bitboard.h"

const int castlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

bool isSquareAttacked(const int sq, const int side, const Board* board) {
    // pawns
    if (side == WHITE) {
        if (board->pieces[sq - 11] == wP || board->pieces[sq - 9] == wP) {
            return true;
        }
    }
    else {
        if (board->pieces[sq + 11] == bP || board->pieces[sq + 9] == bP) {
            return true;
        }
    }

    // knights
    for (int i = 0; i < 8; i++) {
        int piece = board->pieces[sq + piece::direction::dirKn[i]];
        if (piece != OFFBOARD && piece::isKnight[piece] && piece::getCol[piece] == side) {
            return true;
        }
    }

    // rooks, queens
    for (int i = 0; i < 4; i++) {
        int dir = piece::direction::dirRk[i];
        int t_sq = sq + dir;
        int piece = board->pieces[t_sq];
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (piece::isRookQueen[piece] && piece::getCol[piece] == side) {
                    return true;
                }
                break;
            }
            t_sq += dir;
            piece = board->pieces[t_sq];
        }
    }

    // bishops, queens
    for (int i = 0; i < 4; i++) {
        int dir = piece::direction::dirBi[i];
        int t_sq = sq + dir;
        int piece = board->pieces[t_sq];
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (piece::isBishopQueen[piece] && piece::getCol[piece] == side) {
                    return true;
                }
                break;
            }
            t_sq += dir;
            piece = board->pieces[t_sq];
        }
    }

    // kings
    for (int i = 0; i < 8; i++) {
        int piece = board->pieces[sq + piece::direction::dirKi[i]];
        if (piece != OFFBOARD && piece::isKing[piece] && piece::getCol[piece] == side) {
            return true;
        }
    }

    return false;
}

static void addQuietMove(const Board* board, const int move, MoveList *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void addCaptureMove(const Board* board, const int move, MoveList *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void addEnPassantMove(const Board* board, const int move, MoveList *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void addWhitePawnCapMove(const Board* board, const int from, const int to, const int cap, MoveList *list) {
    if (getRank[from] == RANK_7) {
        addCaptureMove(board, MOVE(from, to, cap, wQ, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, wR, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, wB, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, wN, 0), list);
    }
    else {
        addCaptureMove(board, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

static void addWhitePawnMove(const Board* board, const int from, const int to, MoveList *list) {
    if (getRank[from] == RANK_7) {
        addQuietMove(board, MOVE(from, to, EMPTY, wQ, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, wR, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, wB, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, wN, 0), list);
    }
    else {
        addQuietMove(board, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

static void addBlackPawnCapMove(const Board* board, const int from, const int to, const int cap, MoveList *list) {
    if (getRank[from] == RANK_2) {
        addCaptureMove(board, MOVE(from, to, cap, bQ, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, bR, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, bB, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, bN, 0), list);
    }
    else {
        addCaptureMove(board, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

static void addBlackPawnMove(const Board* board, const int from, const int to, MoveList *list) {
    if (getRank[from] == RANK_2) {
        addQuietMove(board, MOVE(from, to, EMPTY, bQ, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, bR, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, bB, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, bN, 0), list);
    }
    else {
        addQuietMove(board, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void generateAllMoves(const Board* board, MoveList *list) {
    list->count = 0;

    int piece = EMPTY;
    int side = board->sideToMove;
    int other = 0;
    int sq = 0;
    int to_sq = 0;
    int pieceNum = 0;
    int dir = 0;
    int i = 0;
    int piecei = 0;

    if (side == WHITE) {
        other = BLACK;
        for (pieceNum = 0; pieceNum < board->pieceNum[wP]; ++pieceNum) {
            sq = board->pieceSquareList[wP][pieceNum];

            if (board->pieces[sq + 10] == EMPTY) {
                addWhitePawnMove(board, sq, sq + 10, list);
                if (getRank[sq] == RANK_2 && board->pieces[sq + 20] == EMPTY) {
                    addQuietMove(board, MOVE(sq, (sq + 20), EMPTY, EMPTY, MFLAG_PS), list);
                }
            }

            if (!OFFBOARD(sq + 9) && piece::getCol[board->pieces[sq + 9]] == BLACK) {
                addWhitePawnCapMove(board, sq, sq + 9, board->pieces[sq + 9], list);
            }
            if (!OFFBOARD(sq + 11) && piece::getCol[board->pieces[sq + 11]] == BLACK) {
                addWhitePawnCapMove(board, sq, sq + 11, board->pieces[sq + 11], list);
            }

            if (board->enPasSquare != NO_SQ && sq + 9 == board->enPasSquare) {
                addEnPassantMove(board, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAG_EP), list);
            }
            if (board->enPasSquare != NO_SQ && sq + 11 == board->enPasSquare) {
                addEnPassantMove(board, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAG_EP), list);
            }
        }

        if (board->castlePerm & WKCA) {
            if (board->pieces[F1] == EMPTY && board->pieces[G1] == EMPTY) {
                if (!isSquareAttacked(E1, BLACK, board) && !isSquareAttacked(F1, BLACK, board)) {
                    addQuietMove(board, MOVE(E1, G1, EMPTY, EMPTY, MFLAG_CA), list);
                }
            }
        }

        if (board->castlePerm & WQCA) {
            if (board->pieces[D1] == EMPTY && board->pieces[C1] == EMPTY && board->pieces[B1] == EMPTY) {
                if (!isSquareAttacked(E1, BLACK, board) && !isSquareAttacked(D1, BLACK, board)) {
                    addQuietMove(board, MOVE(E1, C1, EMPTY, EMPTY, MFLAG_CA), list);
                }
            }
        }
        piece = wN;
    }
    else {
        other = WHITE;
        for (pieceNum = 0; pieceNum < board->pieceNum[bP]; ++pieceNum) {
            sq = board->pieceSquareList[bP][pieceNum];

            if (board->pieces[sq - 10] == EMPTY) {
                addBlackPawnMove(board, sq, sq - 10, list);
                if (getRank[sq] == RANK_7 && board->pieces[sq - 20] == EMPTY) {
                    addQuietMove(board, MOVE(sq, (sq - 20), EMPTY, EMPTY, MFLAG_PS), list);
                }
            }

            if (!OFFBOARD(sq - 9) && piece::getCol[board->pieces[sq - 9]] == WHITE) {
                addBlackPawnCapMove(board, sq, sq - 9, board->pieces[sq - 9], list);
            }

            if (!OFFBOARD(sq - 11) && piece::getCol[board->pieces[sq - 11]] == WHITE) {
                addBlackPawnCapMove(board, sq, sq - 11, board->pieces[sq - 11], list);
            }

            if (board->enPasSquare != NO_SQ && sq - 9 == board->enPasSquare) {
                addEnPassantMove(board, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAG_EP), list);
            }
            if (board->enPasSquare != NO_SQ && sq - 11 == board->enPasSquare) {
                addEnPassantMove(board, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAG_EP), list);
            }
        }

        // castling
        if (board->castlePerm &  BKCA) {
            if (board->pieces[F8] == EMPTY && board->pieces[G8] == EMPTY) {
                if (!isSquareAttacked(E8, WHITE, board) && !isSquareAttacked(F8, WHITE, board)) {
                    addQuietMove(board, MOVE(E8, G8, EMPTY, EMPTY, MFLAG_CA), list);
                }
            }
        }

        if (board->castlePerm &  BQCA) {
            if (board->pieces[D8] == EMPTY && board->pieces[C8] == EMPTY && board->pieces[B8] == EMPTY) {
                if (!isSquareAttacked(E8, WHITE, board) && !isSquareAttacked(D8, WHITE, board)) {
                    addQuietMove(board, MOVE(E8, C8, EMPTY, EMPTY, MFLAG_CA), list);
                }
            }
        }
        piece = bN;
    }

    while ((piece > wP && piece <= wK) || (piece > bP && piece <= bK)) {
        if (piece::isSliding[piece]) {
            for (int n = 0; n < board->pieceNum[piece]; n++) {
                sq = board->pieceSquareList[piece][n];

                for (int i = 0; i < piece::direction::dirLength[piece]; i++) {
                    to_sq = sq + piece::direction::getDir[piece][i];
                    while (board->pieces[to_sq] != OFFBOARD) {
                        if (board->pieces[to_sq] == EMPTY) addQuietMove(board, MOVE(sq, to_sq, 0, 0, 0), list);
                        else {
                            if (piece::getCol[board->pieces[to_sq]] == other) addCaptureMove(board, MOVE(sq, to_sq, board->pieces[to_sq], 0, 0), list);
                            break;
                        }

                        to_sq += piece::direction::getDir[piece][i];
                    }
                }
            }
        }
        else {
            for (int n = 0; n < board->pieceNum[piece]; n++) {
                sq = board->pieceSquareList[piece][n];

                for (int i = 0; i < piece::direction::dirLength[piece]; i++) {
                    to_sq = sq + piece::direction::getDir[piece][i];
                    if (board->pieces[to_sq] == EMPTY) addQuietMove(board, MOVE(sq, to_sq, 0, 0, 0), list);
                    else if (piece::getCol[board->pieces[to_sq]] == other) addCaptureMove(board, MOVE(sq, to_sq, board->pieces[to_sq], 0, 0), list);
                }
            }
        }

        piece++;
    }
}

static void clearPiece(const int sq, Board *board) {
    int piece = board->pieces[sq];

    int col = piece::getCol[piece];
    int index = 0;
    int t_pieceNum = -1;

    HASH_PCE(piece, sq);

    board->pieces[sq] = EMPTY;
    board->material[col] -= piece::getVal[piece];

    if (piece::isBig[piece]) {
        board->pieceBig[col]--;
        if (piece::isMaj[piece]) {
            board->pieceMaj[col]--;
        }
        else {
            board->pieceMin[col]--;
        }
    }
    else {
        CLRBIT(board->pawns[col], SQ64(sq));
        CLRBIT(board->pawns[BOTH], SQ64(sq));
    }

    for (index = 0; index < board->pieceNum[piece]; ++index) {
        if (board->pieceSquareList[piece][index] == sq) {
            t_pieceNum = index;
            break;
        }
    }

    board->pieceNum[piece]--;
    board->pieceSquareList[piece][t_pieceNum] = board->pieceSquareList[piece][board->pieceNum[piece]];
}


static void addPiece(const int sq, Board *board, const int piece) {
    int col = piece::getCol[piece];

    HASH_PCE(piece, sq);

    board->pieces[sq] = piece;

    if (piece::isBig[piece]) {
        board->pieceBig[col]++;
        if (piece::isMaj[piece]) {
            board->pieceMaj[col]++;
        }
        else {
            board->pieceMin[col]++;
        }
    }
    else {
        SETBIT(board->pawns[col], SQ64(sq));
        SETBIT(board->pawns[BOTH], SQ64(sq));
    }

    board->material[col] += piece::getVal[piece];
    board->pieceSquareList[piece][board->pieceNum[piece]++] = sq;

}

static void movePiece(const int from, const int to, Board *board) {
    int index = 0;
    int piece = board->pieces[from];
    int col = piece::getCol[piece];

    HASH_PCE(piece, from);
    board->pieces[from] = EMPTY;

    HASH_PCE(piece, to);
    board->pieces[to] = piece;

    if (!piece::isBig[piece]) {
        CLRBIT(board->pawns[col], SQ64(from));
        CLRBIT(board->pawns[BOTH], SQ64(from));
        SETBIT(board->pawns[col], SQ64(to));
        SETBIT(board->pawns[BOTH], SQ64(to));
    }

    for (index = 0; index < board->pieceNum[piece]; ++index) {
        if (board->pieceSquareList[piece][index] == from) {
            board->pieceSquareList[piece][index] = to;
            break;
        }
    }
}

bool makeMove(Board *board, const int move) {
    int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = board->sideToMove;

    board->history[board->plyTotal].posKey = board->posKey;

    if (move & MFLAG_EP) {
        if (side == WHITE) {
            clearPiece(to - 10, board);
        }
        else {
            clearPiece(to + 10, board);
        }
    }
    else if (move & MFLAG_CA) {
        switch (to) {
        case C1:
            movePiece(A1, D1, board);
            break;
        case C8:
            movePiece(A8, D8, board);
            break;
        case G1:
            movePiece(H1, F1, board);
            break;
        case G8:
            movePiece(H8, F8, board);
            break;
        }
    }

    HASH_CA;

    board->history[board->plyTotal].move = move;
    board->history[board->plyTotal].fiftyMove = board->fiftyMove;
    board->history[board->plyTotal].enPas = board->enPasSquare;
    board->history[board->plyTotal].castlePerm = board->castlePerm;

    board->castlePerm &= castlePerm[from];
    board->castlePerm &= castlePerm[to];
    board->enPasSquare = NO_SQ;

    HASH_CA;

    int captured = CAP(move);
    board->fiftyMove++;

    if (captured != EMPTY) {
        clearPiece(to, board);
        board->fiftyMove = 0;
    }

    board->plyTotal++;
    board->ply++;

    if (piece::isPawn[board->pieces[from]]) {
        board->fiftyMove = 0;
        if (move & MFLAG_PS) {
            if (side == WHITE) {
                board->enPasSquare = from + 10;
            }
            else {
                board->enPasSquare = from - 10;
            }
        }
    }

    movePiece(from, to, board);

    int prPce = PROM(move);
    if (prPce != EMPTY) {
        clearPiece(to, board);
        addPiece(to, board, prPce);
    }

    board->sideToMove ^= 1;
    HASH_SIDE;

    if ((board->sideToMove == BLACK && isSquareAttacked(board->pieceSquareList[wK][0], BLACK, board)) || (board->sideToMove == WHITE && isSquareAttacked(board->pieceSquareList[bK][0], WHITE, board))) {
        takeMove(board);
        return false;
    }

    return true;
}

void takeMove(Board *board) {
    board->plyTotal--;
    board->ply--;

    int move = board->history[board->plyTotal].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);

    HASH_CA;

    board->castlePerm = board->history[board->plyTotal].castlePerm;
    board->fiftyMove = board->history[board->plyTotal].fiftyMove;
    board->enPasSquare = board->history[board->plyTotal].enPas;

    HASH_CA;

    board->sideToMove ^= 1;
    HASH_SIDE;

    if (MFLAG_EP & move) {
        if (board->sideToMove == WHITE) {
            addPiece(to - 10, board, bP);
        }
        else {
            addPiece(to + 10, board, wP);
        }
    }
    else if (MFLAG_CA & move) {
        switch (to) {
        case C1: movePiece(D1, A1, board); break;
        case C8: movePiece(D8, A8, board); break;
        case G1: movePiece(F1, H1, board); break;
        case G8: movePiece(F8, H8, board); break;
        }
    }

    movePiece(to, from, board);

    int captured = CAP(move);
    if (captured != EMPTY) {
        addPiece(to, board, captured);
    }

    if (PROM(move) != EMPTY) {
        clearPiece(from, board);
        addPiece(from, board, (piece::getCol[PROM(move)] == WHITE ? wP : bP));
    }
}