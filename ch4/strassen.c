#include <stdio.h>


void strassen(void *a, int a_d, int a_row, int a_col,
	      void *b, int b_d, int b_row, int b_col,
	      void *c, int c_d)
{
    int wid = c_d / 2;
    int (*A)[a_d], (*B)[b_d], (*C)[c_d];
    int S1[wid][wid], S2[wid][wid], S3[wid][wid], S4[wid][wid], S5[wid][wid],
	S6[wid][wid], S7[wid][wid], S8[wid][wid], S9[wid][wid], S10[wid][wid];
    int P1[wid][wid], P2[wid][wid], P3[wid][wid],
	P4[wid][wid], P5[wid][wid], P6[wid][wid], P7[wid][wid];
    int i, j;

    A = a;
    B = b;
    C = c;

    if (c_d == 1)
    {
	C[0][0] = A[a_row][a_col] * B[b_row][b_col];
	return;
    }

    for (i = 0; i < wid; ++i)
	for (j = 0; j < wid; ++j)
	{
	    S1[i][j] = B[b_row + i][b_col + wid + j] - B[b_row + wid + i][b_col + wid + j];
	    S2[i][j] = A[a_row + i][a_col + j] + A[a_row + i][a_col + wid + j];
	    S3[i][j] = A[a_row + wid + i][a_col + j] + A[a_row + wid + i][a_col + wid + j];
	    S4[i][j] = B[b_row + wid + i][b_col + j] - B[b_row + i][b_col + j];
	    S5[i][j] = A[a_row + i][a_col + j] + A[a_row + wid + i][a_col + wid + j];
	    S6[i][j] = B[b_row + i][b_col + j] + B[b_row + wid + i][b_col + wid + j];
	    S7[i][j] = A[a_row + i][a_col + wid + j] - A[a_row + wid + i][a_col + wid + j];
	    S8[i][j] = B[b_row + wid + i][b_col + j] + B[b_row + wid + i][b_col + wid + j];
	    S9[i][j] = A[a_row + i][a_col + j] - A[a_row + wid + i][a_col + j];
	    S10[i][j] = B[b_row + i][b_col + j] + B[b_row + i][b_col + wid + j];
	}

    strassen(a, a_d, a_row, a_col, S1, wid, 0, 0, P1, wid);
    strassen(S2, wid, 0, 0, b, b_d, b_row + wid, b_col + wid, P2, wid);
    strassen(S3, wid, 0, 0, b, b_d, b_row, b_col, P3, wid);
    strassen(a, a_d, a_row + wid, a_col + wid, S4, wid, 0, 0, P4, wid);
    strassen(S5, wid, 0, 0, S6, wid, 0, 0, P5, wid);
    strassen(S7, wid, 0, 0, S8, wid, 0, 0, P6, wid);
    strassen(S9, wid, 0, 0, S10, wid, 0, 0, P7, wid);

    for (i = 0; i < wid; ++i)
	for (j = 0; j < wid; ++j)
	{
	    C[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
	    C[i][wid + j] = P1[i][j] + P2[i][j];
	    C[wid + i][j] = P3[i][j] + P4[i][j];
	    C[wid + i][wid + j] = P5[i][j] + P1[i][j] - P3[i][j] - P7[i][j];
	}

}

main()
{
    int a[4][4] = { 1, -1, 1, 0,
	            -1, 1, 0, 1,
		    1, -1, 0, 1,
		    0, 1, -1, 1
                  };
    int b[4][4] = { 0, 1, 1, 1,
	            1, 1, 1, 0,
		    1, 0, 1, 1,
		    1, 1, 0, 1
		  };
    int c[4][4];
    int i, j;

    strassen(a, 4, 0, 0, b, 4, 0, 0, c, 4);
    for (i = 0; i < 4; ++i)
    {
	for (j = 0; j < 4; ++j)
	    printf("%d ", c[i][j]);
	printf("\n");
    }
}

