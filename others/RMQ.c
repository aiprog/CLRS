/*
 * NOTE: In this implementation, RMQ tables are used to
 * store positions of minima, NOT values.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

typedef struct _RMQInfo
{
  int *log2Table;
  int bit;
  int *RMQTable;
} RMQInfo;

typedef struct _RMQ1Info
{
  int blkLen;
  RMQInfo **seqInfo;
  RMQInfo **blkInfo;
  int *minBlk;
  RMQInfo *minInfo;
} RMQ1Info;

RMQInfo *newRMQInfo (int *arr, int sz)
{
  RMQInfo *info = malloc (sizeof (RMQInfo));
  info->log2Table = malloc (sizeof (int) * (sz + 1));

  // Initialize the table for ⌈log2(n)⌉ operations
  int value = 1, bit = 1;
  for (int i = 1; i <= sz; ++i)
  {
    if (i == value << 1)
    {
      ++bit;
      value <<= 1;
    }

    info->log2Table[i] = bit;
  }

  info->bit = info->log2Table[sz];
  info->RMQTable = malloc (sizeof (int) * sz * info->bit);

  bit = info->bit;
  int *table = info->RMQTable;

  // table[i][0] = i
  for (int i = 0; i < sz; ++i)
    table[i * bit] = i;

  // table[j][i] = arr[table[j][i - 1] if
  // arr[table[j][i - 1]] <= arr[table[j + 2 ^ (i - 1)][i - 1]]
  // Otherwise, table[j][i] = table[j + 2 ^ (i - 1)][i - 1]
  for (int i = 1; i < bit; ++i)
  {
    for (int j = 0; j < sz; ++j)
    {
      if (j + (1 << i) - 1 >= sz)
        continue;

      int leftMin = table[j * bit + i - 1];
      int rightMin = table[(j + (1 << (i - 1))) * bit + i - 1];

      if (arr[leftMin] <= arr[rightMin])
        table[j * bit + i] = leftMin;
      else
        table[j * bit + i] = rightMin;
    }
  }

  return info;
}

void freeRMQInfo (RMQInfo *info)
{
  free (info->log2Table);
  free (info->RMQTable);
  free (info);
}

int RMQ (RMQInfo *info, int *arr, int beg, int end)
{
  int bit = info->bit;
  int *table = info->RMQTable;

  int queryLen = end - beg + 1;
  int queryBit = info->log2Table[queryLen];

  int leftMin = table[beg * bit + queryBit - 1];
  int rightMin = table[(end - (1 << (queryBit - 1)) + 1) * bit + queryBit - 1];

  if (arr[leftMin] <= arr[rightMin])
    return leftMin;
  else
    return rightMin;
}

RMQ1Info *newRMQ1Info (int *arr, int sz)
{
  RMQ1Info *info = malloc (sizeof (RMQ1Info));
  info->blkLen = log2 (sz) / 2;

  int blkLen = info->blkLen;
  int blkNum = sz / blkLen;
  if (sz % blkLen)
    ++blkNum;

  int seqNum = (1 << (blkLen - 1)) + 1;
  info->seqInfo = malloc (sizeof (RMQInfo *) * seqNum);
  RMQInfo **seqInfo = info->seqInfo;

  int *seq = malloc (sizeof (int) * blkLen);
  seq[0] = 0;

  // Enumerate all ±1 sequences and construct corresponding RMQ tables
  for (int i = 0; i < seqNum - 1; ++i)
  {
    int tmp = i << (32 - (blkLen - 1));
    for (int j = 1; j < blkLen; ++j)
    {
      if (tmp < 0)
        seq[j] = seq[j - 1] + 1;
      else
        seq[j] = seq[j - 1] - 1;

      tmp <<= 1;
    }

    seqInfo[i] = newRMQInfo (seq, blkLen);
  }
  free (seq);

  // Since the last block may well has a shorter length,
  // compute its RMQ table individually
  int offset = (blkNum - 1) * blkLen;
  seqInfo[seqNum - 1] = newRMQInfo (arr + offset, (sz - 1) % blkLen + 1);

  info->blkInfo = malloc (sizeof (RMQInfo *) * blkNum);
  RMQInfo **blkInfo = info->blkInfo;

  // For each block, compute its ±1 sequence and
  // find the corresponding RMQ Table
  for (int i = 0; i < blkNum - 1; ++i)
  {
    int beg = blkLen * i;
    int pos = 0;

    for (int j = 1; j < blkLen; ++j)
    {
      pos *= 2;
      if (arr[beg + j] > arr[beg + j - 1])
        ++pos;
    }

    blkInfo[i] = seqInfo[pos];
  }
  blkInfo[blkNum - 1] = seqInfo[seqNum - 1];  // The last block

  info->minBlk = malloc (sizeof (int) * blkNum);
  int *minBlk = info->minBlk;

  // Find the minima of all blocks and comstruct a RMQ table for them
  for (int i = 0; i < blkNum; ++i)
  {
    int beg = i * blkLen;
    if (i == blkNum - 1)  // The last block
      minBlk[i] = arr[beg + RMQ (blkInfo[i], arr + beg, 0, (sz - 1) % blkLen)];
    else
      minBlk[i] = arr[beg + RMQ (blkInfo[i], arr + beg, 0, blkLen - 1)];
  }
  info->minInfo = newRMQInfo (minBlk, blkNum);

  return info;
}

void freeRMQ1Info (RMQ1Info *info)
{
  for (int i = 0; i < (1 << (info->blkLen - 1)) + 1; ++i)
    freeRMQInfo (info->seqInfo[i]);
  freeRMQInfo (info->minInfo);

  free (info->seqInfo);
  free (info->blkInfo);
  free (info->minBlk);
  free (info);
}

int RMQ1 (RMQ1Info *info, int *arr, int beg, int end)
{
  int blkLen = info->blkLen;
  RMQInfo **blkInfo = info->blkInfo;

  int begBlk = beg / blkLen;
  int endBlk = end / blkLen;
  int begOffset = begBlk * blkLen;
  int endOffset = endBlk * blkLen;

  // Queries inside one block
  if (begBlk == endBlk)
  {
    int bOffset = beg % blkLen;
    int eOffset = end % blkLen;

    return begOffset + RMQ (blkInfo[begBlk], arr + begOffset, bOffset, eOffset);
  }

  // Minima for the first and last blocks
  int begMin = RMQ (blkInfo[begBlk], arr + begOffset, beg % blkLen,
                    blkLen - 1);
  int endMin = RMQ (blkInfo[endBlk], arr + endOffset, 0, end % blkLen);

  int min = begOffset + begMin;
  if (arr[endOffset + endMin] < arr[min])
    min = endOffset + endMin;

  int *minBlk = info->minBlk;
  RMQInfo *minInfo = info->minInfo;

  // Find the minimum of middle blocks, if exists
  if (endBlk - begBlk > 1)
  {
    int midBlk = RMQ (minInfo, minBlk, begBlk + 1, endBlk - 1);
    int midOffset = midBlk * blkLen;
    int midMin = RMQ (blkInfo[midBlk], arr + midOffset, 0, blkLen - 1);

    if (arr[midOffset + midMin] < arr[min])
      min = midOffset + midMin;
  }

  return min;
}

#define SZ 100000

void RMQTest ()
{
  int arr[SZ];
  arr[0] = 0;

  srand ((unsigned)time (NULL));
  for (int i = 1; i < SZ; ++i)
    arr[i] = rand ();

  RMQInfo *info = newRMQInfo (arr, SZ);
  int i;

  for (i = 1; i < SZ; ++i)
  {
    int beg = rand () % SZ;
    int end = beg + rand () % (SZ - beg);

    int min = arr[beg];
    for (int j = beg + 1; j <= end; ++j)
      if (arr[j] < min)
        min = arr[j];

    if (arr[RMQ (info, arr, beg, end)] != min)
      break;
  }

  if (i != SZ)
    printf ("RMQTest fails\n");
  else
    printf ("RMQTest succeeds\n");
}

void RMQ1Test ()
{
  int arr[SZ];
  arr[0] = 0;

  srand ((unsigned)time (NULL));
  for (int i = 1; i < SZ; ++i)
  {
    if (rand () % 2)
      arr[i] = arr[i - 1] + 1;
    else
      arr[i] = arr[i - 1] - 1;
  }

  RMQ1Info *info = newRMQ1Info (arr, SZ);
  int i;

  for (i = 1; i < SZ; ++i)
  {
    int beg = rand () % SZ;
    int end = beg + rand () % (SZ - beg);

    int min = arr[beg];
    for (int j = beg + 1; j <= end; ++j)
      if (arr[j] < min)
        min = arr[j];

    if (arr[RMQ1 (info, arr, beg, end)] != min)
      break;
  }

  if (i != SZ)
    printf ("RMQ1Test fails\n");
  else
    printf ("RMQ1Test succeeds\n");
}

int main ()
{
  RMQTest ();
  RMQ1Test ();

  return 0;
}