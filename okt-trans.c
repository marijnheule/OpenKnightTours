/* Encoding of the open knight tours problem         */
/* by Marijn Heule, last edited on February 27, 2021 */

/******************************************************
There are 168 different knight moves. These are
represented by the variables 1 to 168.

The encoding consists of two constraints. First, Apart
from the start and end squares, exactly two moves must
be in the tour. Second, for each pair of squares (i,j),
we have a Boolean variable expressing whether i is
before j in the tour. The second type of constraint
enforces this.
*******************************************************/

#include <stdio.h>
#include <stdlib.h>

#define SIZE	8
#define FIRST	1
#define LAST	8

int paths;

int lessThan (int a, int b) {
  int min, max;
  if (a < b) { min = a; max = b; }
  else       { min = b; max = a; }

  int res = 1 * paths + max - min;
  while (min) { res += SIZE * SIZE  - min--; }
  return res; }

int isPath (int a, int b) {
  if (a >= b) return 0;

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

  int cls = 2 * sqsize + 2 * paths * (sqsize - 2);
  int jumps, jump[8];
  for (i = 0; i < sqsize; i++) {
    jumps = 0;
    for (j = 0; j < sqsize; j++)
      if (path[i][j] || path[j][i]) jumps++;

    if ((i != first) && (i != last)) {
      cls += jumps + jumps * (jumps - 1) * (jumps - 2) / 6; }
    else { cls += jumps * (jumps - 1) / 2; } }

  printf ("p cnf %i %i\n", paths + sqsize * (sqsize - 1) / 2, cls);

  // The ExactlyTwo constraints
  for (i = 0; i < sqsize; i++) {
    jumps = 0;
    for (j = 0; j < sqsize; j++) {
      if (path[i][j]) jump[jumps++] = path[i][j];
      if (path[j][i]) jump[jumps++] = path[j][i]; }

    // encoding of AtMostTwo constraint using jumps * (jumps - 1) * (jumps - 2) / 6 clauses
    if ((i != first) && (i != last)) {
      for (j = 0; j < jumps; j++)
        for (k = j + 1; k < jumps; k++)
          for (l = k + 1; l < jumps; l++)
            printf("-%i -%i -%i 0\n", jump[j], jump[k], jump[l]); }
    else { // encoding of AtMostOne constraint iff i in {first, last}
      for (j = 0; j < jumps; j++)
        for (k = j + 1; k < jumps; k++)
          printf("-%i -%i 0\n", jump[j], jump[k]); }

    // encoding of AtLeastTwo constraint using jumps clauses (not for first and last)
    if ((i != first) && (i != last)) {
      for (j = 0; j < jumps; j++) {
        for (k = 0; k < jumps; k++)
          if (j != k) printf("%i ", jump[k]);
        printf("0\n"); } }
    else { // encoding of AtLeastOne constraint iff i in {first, last}
      for (j = 0; j < jumps; j++)
        printf("%i ", jump[j]);
      printf("0\n"); } }

  for (i = 0;         i < first;  i++) printf("-%i 0\n", lessThan(i, first));
  for (i = first + 1; i < sqsize; i++) printf("%i 0\n",  lessThan(i, first));
  for (i = 0;         i < last;   i++) printf("%i 0\n",  lessThan(i,  last));
  for (i = last + 1;  i < sqsize; i++) printf("-%i 0\n", lessThan(i,  last));

  // 2 * paths * (sqsize - 2) clauses encoding transitivity
  for (i = 0; i < sqsize; i++)
    for (j = i + 1; j < sqsize; j++)
      if (path[i][j])
        for (k = 0; k < sqsize; k++) {
          if ((k == i) || (k == j)) continue;
          if ((k >  i) && (k <  j)) {
            printf("-%i %i %i 0\n",   path[i][j], lessThan(i,k), lessThan(j,k));
            printf("-%i -%i -%i 0\n", path[i][j], lessThan(i,k), lessThan(j,k)); }
          else {
            printf("-%i -%i %i 0\n",  path[i][j], lessThan(i,k), lessThan(j,k));
            printf("-%i %i -%i 0\n",  path[i][j], lessThan(i,k), lessThan(j,k)); } }
}
