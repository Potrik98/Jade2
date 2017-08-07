#include "test.h"
#include "board.h"
#include "boardutils.h"

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
}