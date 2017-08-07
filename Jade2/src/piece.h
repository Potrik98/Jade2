#pragma once

namespace piece {
    const extern bool isBig[14];
    const extern bool isMaj[14];
    const extern bool isMin[14];

    const extern bool isKnight[14];
    const extern bool isRookQueen[14];
    const extern bool isBishopQueen[14];
    const extern bool isKing[14];
    const extern bool isPawn[14];

    const extern int getVal[13];
    const extern int getCol[14];

    const extern bool isSliding[13];

    namespace direction {
        const extern int dirKn[8];
        const extern int dirRk[4];
        const extern int dirBi[4];
        const extern int dirKi[8];

        const extern int dirLength[13];
        const extern int getDir[13][8];
    }
}