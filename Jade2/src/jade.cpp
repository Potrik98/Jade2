#include "jade.h"
#include "boardutils.h"
#include "test.h"
#include "io.h"
#include "bitboard.h"
#include "hashkey.h"
#include "perft.h"

int sq120to64[120];
int sq64to120[64];
int getRank[120];
int getFile[120];

void initConversionArrays() {
	for (int i = 0; i < 120; i++) {
		getRank[i] = OFFBOARD;
		getFile[i] = OFFBOARD;
		sq120to64[i] = 65;
	}

	int sq64 = 0;
	for (int r = RANK_1; r <= RANK_8; r++) {
		for (int f = FILE_A; f <= FILE_H; f++) {
			int sq = FR2SQ(f, r);
			sq64to120[sq64] = sq;
			sq120to64[sq] = sq64++;
			getRank[sq] = r;
			getFile[sq] = f;
		}
	}
}

void initBitMasks() {
    for (int i = 0; i < 64; i++) {
        bitboard::setMask[i] = (1ULL << i);
        bitboard::clrMask[i] = ~bitboard::setMask[i];
    }
}

int main() {
#define DEBUG
#ifdef DEBUG
	initConversionArrays();
    initBitMasks();
    hashkey::init();
	test::testParseFen();
    test::testIsSquareAttacked();
    test::testGenerateAllMoves();
    test::testHashkeys();
    test::testPolyglotHashkeys();
    test::perftTests();
    test::testMoveExists();
    runPerftTests("perftsuite.epd");
#endif // DEBUG
    startInputLoop();
}
