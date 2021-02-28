/* Encoding of the open knight tours problem         */
/* by Marijn Heule, last edited on February 27, 2021 */

/******************************************************
There are 168 different knight moves (modulo polarity).
This encoding uses the variables 1 to 336 to express
the moves.

The encoding consists of three constraints. First, when
using LFSR, none of the bit-vectors can be all-zero.
Although this constraint it redundant, it is useful in
practice. Second, apart from the last position, each
position must have exactly one successor. Third, apart
from the first position, each position musht have
exactly one predecessor. Finally, the successor is
encoded using linear-feedback shift register with
1 + x^5 + x^6.
*******************************************************/

#include <stdio.h>
#include <stdlib.h>

#define SIZE	8
#define FIRST	1
#define LAST	8

int paths;

int bit (int s, int b) {
  return paths + s * 6 + b + 1; }

int isPath (int a, int b) {
  int r = abs((a % SIZE) - (b % SIZE));
  int c = abs((a / SIZE) - (b / SIZE));

  if ((r == 1) && (c == 2)) return 1;
  if ((r == 2) && (c == 1)) return 1;
  return 0; }

int main (int argc, char** argv) {
  int i, j, k, l;
  int sqsize = SIZE * SIZE;

  int path[sqsize][sqsize];

  int first = FIRST - 1;
  int last  = LAST  - 1;

  if (argc > 1) first = atoi (argv[1]) - 1;
  if (argc > 2) last  = atoi (argv[2]) - 1;

  paths = 0;

  for (i = 0; i < sqsize; i++)
    for (j = 0; j < sqsize; j++) {
      if (isPath (i, j)) path[i][j] = ++paths;
      else               path[i][j] = 0; }

  int nCls = 0;

  int size;

  for (i = 0; i < sqsize; i++) {
    if (i == last) continue;
    size = 0;
    for (j = 0; j < sqsize; j++) {
      if (j == first) continue;
      if (path[i][j]) size++; }
    nCls += 1 + size * (size - 1) / 2; }

  for (i = 0; i < sqsize; i++) {
    if (i == first) continue;
    size = 0;
    for (j = 0; j < sqsize; j++) {
      if (j == last) continue;
      if (path[j][i]) size++; }
    nCls += 1 + size * (size - 1) / 2; }

  printf ("p cnf %i %i\n", paths + 6 * sqsize, nCls + sqsize + 12 + 14 * paths);

  // none of the bitvectors can be all-zero
  for (i = 0; i < sqsize; i++) {
    for (j = 0; j < 6; j++)
      printf ("%i ", bit (i, j));
    printf ("0\n"); }

  int jump[8];

  // Exactly one successor
  for (i = 0; i < sqsize; i++) {
    if (i == last) continue;
    size = 0;
    for (j = 0; j < sqsize; j++) {
      if (j == first) continue;
      if (path[i][j]) jump[size++] = path[i][j]; }

    for (j = 0; j < size; j++)
      printf ("%i ", jump[j]);
    printf ("0\n");

    for (j = 0; j < size; j++)
      for (k = j+1; k < size; k++)
        printf ("-%i -%i 0\n", jump[j], jump[k]); }

  // Exactly one predecessor
  for (i = 0; i < sqsize; i++) {
    if (i == first) continue;
    size = 0;
    for (j = 0; j < sqsize; j++) {
      if (j == last) continue;
      if (path[j][i]) jump[size++] = path[j][i]; }

    for (j = 0; j < size; j++)
      printf ("%i ", jump[j]);
    printf ("0\n");

    for (j = 0; j < size; j++)
      for (k = j+1; k < size; k++)
        printf ("-%i -%i 0\n", jump[j], jump[k]); }

  // initialize the beginning and the end positions
  printf ("%i 0\n", bit (first, 0));
  printf ("%i 0\n", bit (last,  0));
  for (i = 1; i < 6; i++) {
    printf ("-%i 0\n", bit (first, i));
    printf ("-%i 0\n", bit (last,  i)); }

  for (i = 0; i < sqsize; i++) {
    for (j = 0; j < sqsize; j++)
      if (path[i][j]) {
        printf("-%i %i -%i 0\n",      path[i][j], bit(j, 0), bit(i, 5));
        printf("-%i -%i %i 0\n",      path[i][j], bit(j, 0), bit(i, 5));
        printf("-%i %i -%i 0\n",      path[i][j], bit(j, 1), bit(i, 0));
        printf("-%i -%i %i 0\n",      path[i][j], bit(j, 1), bit(i, 0));
        printf("-%i %i -%i 0\n",      path[i][j], bit(j, 2), bit(i, 1));
        printf("-%i -%i %i 0\n",      path[i][j], bit(j, 2), bit(i, 1));
        printf("-%i %i -%i 0\n",      path[i][j], bit(j, 3), bit(i, 2));
        printf("-%i -%i %i 0\n",      path[i][j], bit(j, 3), bit(i, 2));
        printf("-%i %i -%i 0\n",      path[i][j], bit(j, 4), bit(i, 3));
        printf("-%i -%i %i 0\n",      path[i][j], bit(j, 4), bit(i, 3));
        printf("-%i -%i -%i -%i 0\n", path[i][j], bit(j, 5), bit(i, 4), bit(i, 5));
        printf("-%i %i %i -%i 0\n",   path[i][j], bit(j, 5), bit(i, 4), bit(i, 5));
        printf("-%i %i -%i %i 0\n",   path[i][j], bit(j, 5), bit(i, 4), bit(i, 5));
        printf("-%i -%i %i %i 0\n",   path[i][j], bit(j, 5), bit(i, 4), bit(i, 5));
      }
   }
}
