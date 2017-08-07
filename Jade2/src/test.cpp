#include "test.h"
#include "board.h"
#include "move.h"
#include "boardutils.h"
#include "hashkey.h"

namespace test {
    void testParseFen() {
        Board board;
        board.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        if (board.pieces[A1] != wR) throw std::runtime_error("Test failed!");
        if (board.pieces[E1] != wK) throw std::runtime_error("Test failed!");
        if (board.pieces[E8] != bK) throw std::runtime_error("Test failed!");
        if (board.pieces[A4] != EMPTY) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
        if (board.sideToMove != WHITE) throw std::runtime_error("Test failed!");
        if (board.enPasSquare != C6) throw std::runtime_error("Test failed!");
        if (board.fiftyMove != 0) throw std::runtime_error("Test failed!");
        if (board.plyTotal != 3) throw std::runtime_error("Test failed!");
        if (board.castlePerm != 15) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
        if (board.sideToMove != BLACK) throw std::runtime_error("Test failed!");
        if (board.enPasSquare != NO_SQ) throw std::runtime_error("Test failed!");
        if (board.fiftyMove != 1) throw std::runtime_error("Test failed!");
        if (board.plyTotal != 4) throw std::runtime_error("Test failed!");

        try {
            board.parseFen("invalid-fen");
            throw std::runtime_error("Test failed!");
        }
        catch (const std::invalid_argument& e) {

        }
    }

    void testIsSquareAttacked() {
        Board board;
        board.pieces[D4] = wQ;
        if (!isSquareAttacked(H4, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(A4, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(D8, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(D1, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(A1, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(G1, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(A7, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(H8, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(H4, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(A4, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(D8, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(D1, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(A1, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(G1, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(A7, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(H8, BLACK, &board)) throw std::runtime_error("Test failed!");
        board.pieces[D4] = bQ;
        if (isSquareAttacked(H4, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(A4, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(D8, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (isSquareAttacked(D1, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(H4, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(A4, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(D8, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(D1, BLACK, &board)) throw std::runtime_error("Test failed!");
        board.pieces[D4] = wN;
        if (!isSquareAttacked(E6, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(E2, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(C6, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(C2, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(F5, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(F3, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(B5, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(B3, WHITE, &board)) throw std::runtime_error("Test failed!");
        board.pieces[D4] = wP;
        if (!isSquareAttacked(E5, WHITE, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(C5, WHITE, &board)) throw std::runtime_error("Test failed!");
        board.pieces[D4] = bP;
        if (!isSquareAttacked(E3, BLACK, &board)) throw std::runtime_error("Test failed!");
        if (!isSquareAttacked(C3, BLACK, &board)) throw std::runtime_error("Test failed!");
    }

    void testGenerateAllMoves() {
        Board board;
        MoveList moveList[1];

        board.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        generateAllMoves(&board, moveList);
        if (moveList->count != 20) throw std::runtime_error("Test failed");

        board.parseFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        generateAllMoves(&board, moveList);
        if (moveList->count != 48) throw std::runtime_error("Test failed");

        board.parseFen("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
        generateAllMoves(&board, moveList);
        if (moveList->count != 24 + 1 // add one here because the move generator doesn't consider if a move is legal
            ) throw std::runtime_error("Test failed");
    }

    void testHashkeys() {
        Board board;
        board.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        if (board.posKey != 0x463b96181691fc9cULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
        if (board.posKey != 0x4d0dde8ef7cc031fULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2");
        if (board.posKey != 0x1bcf67975d7d9f11ULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");
        if (board.posKey != 0x662fafb965db29d4ULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
        if (board.posKey != 0x22a48b5a8e47ff78ULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 3");
        if (board.posKey != 0x652a607ca3f242c1ULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbq1bnr/ppp1pkpp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR w - - 0 4");
        if (board.posKey != 0x00fdd303c946bdd9ULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/p1pppppp/8/8/PpP4P/8/1P1PPPP1/RNBQKBNR b KQkq c3 0 3");
        if (board.posKey != 0x3c8123ea7b067637ULL) throw std::runtime_error("Test failed!");

        board.parseFen("rnbqkbnr/p1pppppp/8/8/P6P/R1p5/1P1PPPP1/1NBQKBNR b Kkq - 0 4");
        if (board.posKey != 0x5c3f9b829b279560ULL) throw std::runtime_error("Test failed!");
    }
}
