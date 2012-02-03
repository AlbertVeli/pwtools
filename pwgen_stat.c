/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 *
 *
 * Build oclHashcat mask patters from input file of words.
 *
 * For instance:
 *
 * Generate pwgen.txt with something like:
 *
 *  pwgen -1cn 8 1000000 > pwgen.txt
 *
 *  ./pwgen_stat | sort +1nr -2 > pwgen.stat
 *
 * Now take out first column from pwgen.stat and print out the whole cmdline:
 *
 *  cat pwgen.stat | awk '{printf "./cudaHashcat-plus32.bin --gpu-watchdog=89 -a 3 example.hash %s\n", $1}'
 *
 * Since the output was sorted on column 2 above the oclHashcat
 * lines will be run in order with the most common mask first.
 *
 *
 * Prickle-Prickle, the 34th day of Chaos in the YOLD 3178
 *
 * Albert Veli
 */

#include <stdio.h>
#include <string.h>

#define MAX 100000
#define PWLEN 8 /* All passwords in list must be of this len. */

struct pattern {
   char str[PWLEN];
   int num;
};

struct pattern patterns[MAX];
int n_pat;

void add_pat(char *str)
{
   int i;
   char pat[PWLEN];

   for (i = 0; i < PWLEN; i++) {
      if (str[i] >= '0' && str[i] <= '9') {
         pat[i] = 'd';
      } else if (str[i] >= 'a' && str[i] <= 'z') {
         pat[i] = 'l';
      } else if (str[i] >= 'A' && str[i] <= 'Z') {
         pat[i] = 'u';
      } else {
         /* TODO: Support special characters. */
         fprintf(stderr, "unexpected charset in %s\n", str);
         return;
      }
   }

   for (i = 0; i < n_pat; i++) {
      if (memcmp(pat, patterns[i].str, PWLEN) == 0) {
         /* found */
         patterns[i].num++;
         return;
      }
   }
   /* not found */
   if (i < MAX) {
      memcpy(patterns[i].str, pat, PWLEN);
      n_pat++;
   }
}

void print_pats(void)
{
   int i, j;

   for (i = 0; i < n_pat; i++) {
      for (j = 0; j < PWLEN; j++) {
         printf("?%c", patterns[i].str[j]);
      }
      printf(" %d\n", patterns[i].num + 1);
   }
}

int main(void)
{
   FILE *fp;
   char line[16];

   /* TODO: take input from argv instead. */
   fp = fopen("pwgen.txt", "r");
   if (!fp) {
      fprintf(stderr, "unable to open pwgen.txt for reading\n");
      perror("pwgen.txt");
      return 1;
   }

   n_pat = 0;
   memset(patterns, 0, sizeof(patterns));

   while (fgets(line, sizeof(line), fp) && n_pat < MAX) {
      add_pat(line);
   }

   fclose(fp);

   printf("n_pat = %d\n\n", n_pat);

   print_pats();

   return 0;
}

