#include "io.h"
#include "boardutils.h"

char charPiece[] = ".PNBRQKpnbrqkx";
char charSide[] = "wb-";
char charRank[] = "123456RANK_88";
char charFile[] = "abcdefgh";

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