#include "search.h"
#include "board.h"
#include "io.h"
#include "evaluate.h"
#include "move.h"
#include "pvtable.h"

#include <iostream>
#include <thread>

bool isRepetition(const Board *board) {
    for (int i = board->plyTotal - 2; i >= board->plyTotal - board->fiftyMove; i -= 2) {
        if (board->posKey == board->history[i].posKey) return true;
    }
    return false;
}

void sortNextMove(int num, MoveList *list) {
    int best = 0;
    int index = num;

    for (int i = num; i < list->count; i++) {
        if (list->moves[i].score > best) {
            best = list->moves[i].score;
            index = i;
        }
    }

    Move tmp = list->moves[num];
    list->moves[num] = list->moves[index];
    list->moves[index] = tmp;
}

static int alphaBeta(int alpha, int beta, int depth, Board *board, SearchInfo *info) {
    if (info->stop) return 0;
    if (depth <= 0) return evaluate(board);

    info->nodes++;

    if ((isRepetition(board) || board->fiftyMove >= 100) && board->ply) return 0;
    if (board->ply > MAX_SEARCH_DEPTH - 1) {
        std::cout << "REACHED MAX DEPTH!";
        return evaluate(board);
    }

    bool check = ((board->sideToMove == WHITE) && isSquareAttacked(board->pieceSquareList[wK][0], BLACK, board))
        || ((board->sideToMove == BLACK) && isSquareAttacked(board->pieceSquareList[bK][0], WHITE, board));
    if (check) depth++;

    int score = -INF;
   
    MoveList list[1];
    generateAllMoves(board, list);

    int m = 0;
    int legal = 0;
    int alphaOld = alpha;
    int bestmove = 0;

    int bestscore = -INF;

    score = -INF;

    for (m = 0; m < list->count; ++m) {
        sortNextMove(m, list);
        if (makeMove(board, list->moves[m].move)) {
            legal++;
            score = -alphaBeta(-beta, -alpha, depth - 1, board, info);
            takeMove(board);


            if (score > bestscore) {
                bestscore = score;
                bestmove = list->moves[m].move;
                if (score > alpha) {
                    if (score >= beta) {
                        if (legal == 1) {
                            info->fhf++;
                        }
                        info->fh++;

                        storePvMove(board, bestmove);
                        return beta;
                    }
                    alpha = score;
                }
            }
        }
    }

    if (legal == 0) {
        if (check) {
            return -INF + board->ply;
        }
        else {
            return 0;
        }
    }

    return alpha;
}

void clearInfo(Board *board, SearchInfo *info) {
    info->fh = 0;
    info->fhf = 0;
    info->nodes = 0;
}

void clearSearch(Board *board, SearchInfo *info) {
    board->ply = 0;

    clearInfo(board, info);
}

void search(Board *board, SearchInfo *info) {
    clearSearch(board, info);

    int move = 0;
    int score = -INF;

    for (int d = 1; d <= info->maxDepth; d++) {
        score = alphaBeta(-INF, INF, d, board, info);
        int count = getPvLine(board, d);
        if (info->stop) break;
        double r = info->fhf * 100.0 / info->fh;

        std::cout << "D" << d << ": [";

        if (score > INF - MAX_SEARCH_DEPTH) {
            score = (INF - score + 1) / 2;
            if (abs(score) > MAX_SEARCH_DEPTH) break;
            std::cout << "MATE IN ";
        }
        else if (score < -INF + MAX_SEARCH_DEPTH) {
            score = (-INF - score - 1) / 2;
            if (abs(score) > MAX_SEARCH_DEPTH) break;
            std::cout << "MATE IN ";
        }

        std::cout << score << "] (" << info->nodes << " nodes, " << r << "% order) ";
        printPvLine(board, d);
        std::cout << std::endl;

        move = board->pvArray[0];
        clearInfo(board, info);
    }
}
