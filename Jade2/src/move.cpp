#pragma once

#include "move.h"
#include "boardutils.h"
#include "piece.h"

bool isSquareAttacked(const int sq, const int side, const Board *Board) {
    // pawns
    if (side == WHITE) {
        if (Board->pieces[sq - 11] == wP || Board->pieces[sq - 9] == wP) {
            return true;
        }
    }
    else {
        if (Board->pieces[sq + 11] == bP || Board->pieces[sq + 9] == bP) {
            return true;
        }
    }

    // knights
    for (int i = 0; i < 8; i++) {
        int piece = Board->pieces[sq + piece::direction::dirKn[i]];
        if (piece != OFFBOARD && piece::isKnight[piece] && piece::getCol[piece] == side) {
            return true;
        }
    }

    // rooks, queens
    for (int i = 0; i < 4; i++) {
        int dir = piece::direction::dirRk[i];
        int t_sq = sq + dir;
        int piece = Board->pieces[t_sq];
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (piece::isRookQueen[piece] && piece::getCol[piece] == side) {
                    return true;
                }
                break;
            }
            t_sq += dir;
            piece = Board->pieces[t_sq];
        }
    }

    // bishops, queens
    for (int i = 0; i < 4; i++) {
        int dir = piece::direction::dirBi[i];
        int t_sq = sq + dir;
        int piece = Board->pieces[t_sq];
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (piece::isBishopQueen[piece] && piece::getCol[piece] == side) {
                    return true;
                }
                break;
            }
            t_sq += dir;
            piece = Board->pieces[t_sq];
        }
    }

    // kings
    for (int i = 0; i < 8; i++) {
        int piece = Board->pieces[sq + piece::direction::dirKi[i]];
        if (piece != OFFBOARD && piece::isKing[piece] && piece::getCol[piece] == side) {
            return true;
        }
    }

    return false;
}

static void addQuietMove(const Board *Board, const int move, MoveList *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void addCaptureMove(const Board *Board, const int move, MoveList *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void addEnPassantMove(const Board *Board, const int move, MoveList *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

static void addWhitePawnCapMove(const Board *Board, const int from, const int to, const int cap, MoveList *list) {
    if (getRank[from] == RANK_7) {
        addCaptureMove(Board, MOVE(from, to, cap, wQ, 0), list);
        addCaptureMove(Board, MOVE(from, to, cap, wR, 0), list);
        addCaptureMove(Board, MOVE(from, to, cap, wB, 0), list);
        addCaptureMove(Board, MOVE(from, to, cap, wN, 0), list);
    }
    else {
        addCaptureMove(Board, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

static void addWhitePawnMove(const Board *Board, const int from, const int to, MoveList *list) {
    if (getRank[from] == RANK_7) {
        addQuietMove(Board, MOVE(from, to, EMPTY, wQ, 0), list);
        addQuietMove(Board, MOVE(from, to, EMPTY, wR, 0), list);
        addQuietMove(Board, MOVE(from, to, EMPTY, wB, 0), list);
        addQuietMove(Board, MOVE(from, to, EMPTY, wN, 0), list);
    }
    else {
        addQuietMove(Board, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

static void addBlackPawnCapMove(const Board *Board, const int from, const int to, const int cap, MoveList *list) {
    if (getRank[from] == RANK_2) {
        addCaptureMove(Board, MOVE(from, to, cap, bQ, 0), list);
        addCaptureMove(Board, MOVE(from, to, cap, bR, 0), list);
        addCaptureMove(Board, MOVE(from, to, cap, bB, 0), list);
        addCaptureMove(Board, MOVE(from, to, cap, bN, 0), list);
    }
    else {
        addCaptureMove(Board, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

static void addBlackPawnMove(const Board *Board, const int from, const int to, MoveList *list) {
    if (getRank[from] == RANK_2) {
        addQuietMove(Board, MOVE(from, to, EMPTY, bQ, 0), list);
        addQuietMove(Board, MOVE(from, to, EMPTY, bR, 0), list);
        addQuietMove(Board, MOVE(from, to, EMPTY, bB, 0), list);
        addQuietMove(Board, MOVE(from, to, EMPTY, bN, 0), list);
    }
    else {
        addQuietMove(Board, MOVE(from, to, EMPTY, EMPTY, 0), list);
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
