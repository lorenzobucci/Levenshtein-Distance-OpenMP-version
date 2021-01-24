#include <iostream>
#include "LevenshteinDistance.h"

using namespace std;

unsigned short sequentialAlgorithm(const string &str1, const string &str2, unsigned short **d) {

    unsigned short cost;

    for (int i1 = 0; i1 <= str1.length(); i1++)
        d[i1][0] = i1;
    for (int i2 = 0; i2 <= str2.length(); i2++)
        d[0][i2] = i2;

    for (int i1 = 1; i1 <= str1.length(); i1++) {
        for (int i2 = 1; i2 <= str2.length(); i2++) {
            if (str1[i1 - 1] == str2[i2 - 1])
                cost = 0;
            else
                cost = 1;
            d[i1][i2] = min(d[i1 - 1][i2] + 1, min(d[i1][i2 - 1] + 1, d[i1 - 1][i2 - 1] + cost));
        }
    }

    return d[str1.length()][str2.length()];
}

unsigned short parallelAlgorithm(const string &str1, const string &str2, const unsigned short &subMatrixSize, unsigned short **d) {

    unsigned short str1Len = str1.length();
    unsigned short str2Len = str2.length();

    #pragma omp parallel default(none) shared(d, str1, str2, str1Len, str2Len)
    {
        #pragma omp for nowait
        for (int i1 = 0; i1 <= str1Len; i1++)
            d[i1][0] = i1;

        #pragma omp for
        for (int i2 = 0; i2 <= str2Len; i2++)
            d[0][i2] = i2;
    }

    #pragma omp parallel default(none) shared(subMatrixSize, str1, str2, str1Len, str2Len, d)
    {
        for (int diag = 1; diag <= ((str1Len / subMatrixSize) + (str2Len / subMatrixSize) - 1); diag++) {
            int start_col = max(1, (diag - (str1Len / subMatrixSize) + 1));
            int count = min(diag, min(((str2Len / subMatrixSize) - start_col + 1), (str1Len / subMatrixSize)));

            #pragma omp for
            for (int diagElement = 0; diagElement < count; diagElement++) {
                int subMatrixColId = start_col + diagElement;
                int subMatrixRowId = min((str1Len / subMatrixSize), diag) - diagElement;
                int startCol = (subMatrixColId * subMatrixSize) - subMatrixSize + 1;
                int startRow = ((subMatrixRowId * subMatrixSize) - subMatrixSize + 1);
                int endCol = startCol + subMatrixSize;
                int endRow = startRow + subMatrixSize;

                if (str2Len - endCol < subMatrixSize)
                    endCol = str2Len;
                if (str1Len - endRow < subMatrixSize)
                    endRow = str1Len;

                for (int i = startRow; i <= endRow; i++) {
                    for (int j = startCol; j <= endCol; j++) {
                        int cost;
                        if (str1[i - 1] == str2[j - 1])
                            cost = 0;
                        else
                            cost = 1;
                        d[i][j] = min(d[i - 1][j] + 1, min(d[i][j - 1] + 1, d[i - 1][j - 1] + cost));
                    }
                }
            }
        }
    }

    return d[str1Len][str2Len];
}