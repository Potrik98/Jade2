#include "board.h"
#include "boardutils.h"

#include <algorithm>
#include <sstream>
#include <iostream>

Board::Board() {
	clear();
}

void Board::clear() {
	for (int i = 0; i < 120; i++) {
		pieces[i] = OFFBOARD;
	}

	for (int i = 0; i < 64; i++) {
		pieces[SQ120(i)] = EMPTY;
	}

	for (int i = 0; i < 2; i++) {
		pieceBig[i] = 0;
		pieceMaj[i] = 0;
		pieceMin[i] = 0;
		material[i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		pawns[i] = 0ULL;
	}

	for (int i = 0; i < 13; i++) {
		pieceNum[i] = 0;
	}

	sideToMove = BOTH;
	enPasSquare = NO_SQ;
	fiftyMove = 0;

	ply = 0;
	plyTotal = 0;

	castlePerm = 0;

	posKey = 0ULL;
}

int getPieceFromChar(const char c) {
	switch (c) {
	case 'p': return bP;
	case 'r': return bR;
	case 'n': return bN;
	case 'b': return bB;
	case 'k': return bK;
	case 'q': return bQ;
	case 'P': return wP;
	case 'R': return wR;
	case 'N': return wN;
	case 'B': return wB;
	case 'K': return wK;
	case 'Q': return wQ;
	default: return EMPTY;
	}
}

int getNumberOfBlankSquares(const char c) {
	return std::max(0, std::min(8, c - '0'));
}

void assertIndexInString(const int index, const std::string string) {
	if (index >= string.length()) {
		throw std::invalid_argument("Format too short!");
	}
}

void Board::parseRank(const std::string format, const int rank) {
	int index = 0;
	for (int file = FILE_A; file <= FILE_H;) {
		assertIndexInString(index, format);

		const char c = format[index];
		int piece = EMPTY;
		int blanks = 0;
		
		if ((piece = getPieceFromChar(c)) != EMPTY) {
			pieces[FR2SQ(file, rank)] = piece;
			file++;
		}
		else if ((blanks = getNumberOfBlankSquares(c)) != 0) {
			if (file + blanks - 1 > FILE_H) {
				std::ostringstream msg;
				msg << "Too many squares!";
				msg << " file:" << file << " rank:" << rank << " squares:" << blanks;
				throw std::invalid_argument(msg.str());
			}
			else {
				file += blanks;
			}
		} else {
			std::ostringstream msg;
			msg << "Unexpected character: " << c;
			msg << " format: " << format;
			msg << " index: " << index;
			msg << " file:" << file << " rank:" << rank;
			throw std::invalid_argument(msg.str());
		}
		index++;
	}
}

void Board::parseSideToMove(const std::string fen, const int index) {
	assertIndexInString(index, fen);
	switch (fen[index])
	{
	case 'w':
		sideToMove = WHITE;
		break;
	case 'b':
		sideToMove = BLACK;
		break;
	default:
		std::ostringstream msg;
		msg << "Unexpected character: " << fen[index];
		throw std::invalid_argument(msg.str());
	}
}

int Board::parseCastlePermission(const std::string fen, const int index) {
	for (int offset = 0; offset < 4; offset++) {
		assertIndexInString(index + offset, fen);

		switch (fen[index + offset])
		{
		case 'K':
			castlePerm |= WKCA;
			break;
		case 'Q':
			castlePerm |= WQCA;
			break;
		case 'k':
			castlePerm |= BKCA;
			break;
		case 'q':
			castlePerm |= BQCA;
			break;
		case ' ':
			return offset + 1;
		default:
			std::ostringstream msg;
			msg << "Unexpected character: " << fen[index];
			throw std::invalid_argument(msg.str());
		}
	}
	return 4;
}

int Board::parseEnPassantSquare(const std::string fen, int index) {
	assertIndexInString(index, fen);
	const char c = fen[index];
	if (c == '-') return 1;

	const int file = c - 'a';
	if (file > FILE_H || file < FILE_A) {
		std::ostringstream msg;
		msg << "Unexpected character: " << c;
		throw std::invalid_argument(msg.str());
	}

	assertIndexInString(index + 1, fen);
	const char d = fen[index + 1];
	const int rank = d - '1';
	if (rank > RANK_8 || rank < RANK_1) {
		std::ostringstream msg;
		msg << "Unexpected character: " << d;
		throw std::invalid_argument(msg.str());
	}

	enPasSquare = FR2SQ(file, rank);

	return 2;
}

void Board::parseMoveCounters(const std::string fen, const int index) {
	assertIndexInString(index + 2, fen);
	std::istringstream(fen.substr(index)) >> fiftyMove >> plyTotal;
	plyTotal *= 2;
	if (sideToMove == WHITE) plyTotal -= 1;
}

void Board::parseFen(const std::string fen) {
	clear();
	int index = 0;
	parseRank(fen.substr(index), RANK_8);
	
	for (int rank = RANK_7; rank >= RANK_1; rank--) {
		size_t loc = fen.find_first_of('/', index);
		if (loc == std::string::npos) {
			std::ostringstream msg;
			msg << "Missing ranks!";
			msg << " fen: " << fen;
			msg << " index: " << index;
			msg << " rank: " << rank;
			throw std::invalid_argument(msg.str());
		}
		index = loc + 1;
		parseRank(fen.substr(index), rank);
	}

	size_t loc = fen.find_first_of(' ', index);
	if (loc == std::string::npos) {
		std::ostringstream msg;
		msg << "Format too short!";
		throw std::invalid_argument(msg.str());
	}
	index = loc + 1;
	parseSideToMove(fen, index);

	index += 2;
	index += parseCastlePermission(fen, index);
	index++;

	index += parseEnPassantSquare(fen, index);
	index++;

	parseMoveCounters(fen, index);
}

void Board::reset() {

}