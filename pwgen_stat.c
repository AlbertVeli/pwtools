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
/* TODO: set pwlen from cmdline, default to 8 */
int pwlen = 8; /* Only look at passwords this len */
#define MAXLEN 11 /* For allocations of arrays */

struct pattern {
   char str[MAXLEN];
   int num;
};

struct pattern patterns[MAX];
int n_pat;

void add_pat(char *str)
{
   int i;
   char pat[MAXLEN];

   for (i = 0; i < pwlen; i++) {
      if (str[i] >= '0' && str[i] <= '9') {
         pat[i] = 'd';
      } else if (str[i] >= 'a' && str[i] <= 'z') {
         pat[i] = 'l';
      } else if (str[i] >= 'A' && str[i] <= 'Z') {
         pat[i] = 'u';
      } else if (
         (str[i] >= 33 && str[i] <= 47) || /* !"#$%&'()*+,-./ */
         (str[i] >= 58 && str[i] <= 64) || /* :;<=>?@         */
         (str[i] >= 91 && str[i] <= 96) || /* [\]^_`          */
         (str[i] >= 123 && str[i] <= 126)  /* {|}~            */
         ) {
         pat[i] = 's';
      } else if (str[i] == '\n' || str[i] == 0) {
         /* str shorter than pwlen, skip this word */
         return;
      } else {
         /* Unknown character */
         #if 0
         fprintf(stderr, "unexpected charset in %s (%c)\n", str, str[i]);
         #endif
         return;
      }
   }

   /* Check that str is not longer than pwlen */
   if (str[i] != '\n' && str[i] != 0) {
      /* Neither newline nor 0, skip this word */
      return;
   }

   for (i = 0; i < n_pat; i++) {
      if (memcmp(pat, patterns[i].str, pwlen) == 0) {
         /* found */
         patterns[i].num++;
         return;
      }
   }
   /* not found */
   if (i < MAX) {
      memcpy(patterns[i].str, pat, pwlen);
      n_pat++;
   }
}

void print_pats(void)
{
   int i, j;

   for (i = 0; i < n_pat; i++) {
      for (j = 0; j < pwlen; j++) {
         printf("?%c", patterns[i].str[j]);
      }
      printf(" %d\n", patterns[i].num + 1);
   }
}

void print_usage(char *progname)
{
   printf("Usage: %s <filename>\n\n", progname);
   printf("Example:\n");
   printf("%s pwgen.txt | sort +1nr -2 | awk '{printf \"./cudaHashcat-plus32.bin --gpu-watchdog=89 -a 3 example.hash %%s\\n\", $1}'\n\n", progname);
}

int main(int argc, char *argv[])
{
   FILE *fp;
   char line[16];

   if (argc < 2) {
      print_usage(argv[0]);
      return 1;
   }

   fp = fopen(argv[1], "r");
   if (!fp) {
      fprintf(stderr, "\nError: unable to open \"%s\" for reading\n", argv[1]);
      perror(argv[1]);
      fprintf(stderr, "\n");
      print_usage(argv[0]);
      return 1;
   }

   n_pat = 0;
   memset(patterns, 0, sizeof(patterns));

   while (fgets(line, sizeof(line), fp) && n_pat < MAX) {
      add_pat(line);
   }

   fclose(fp);

   print_pats();

   return 0;
}
