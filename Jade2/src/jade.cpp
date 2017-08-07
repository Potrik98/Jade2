#include "jade.h"
#include "boardutils.h"
#include "test.h"

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

int main() {
	initConversionArrays();

	test::testParseFen();
}
