/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 *
 *
 * Generate brute force patterns for use with oclHashcat.
 *
 * Prickle-Prickle, the 34th day of Chaos in the YOLD 3178
 *
 * Albert Veli
 */

#include <stdio.h>

#define NUM_CHARSETS 3
#define PAT_LEN 8

/* l = lower case
 * u = upper case
 * d = digits
 * s = special characters
 */
char charsets[NUM_CHARSETS] = { 'l', 'u', 'd'};

int pattern[PAT_LEN];

void init_pattern(void)
{
   int i;

   /* init pattern */
   for (i = 0; i < PAT_LEN; i++) {
      pattern[i] = 0;
   }
}

/* Generate next pattern */
int inc_pattern(void)
{
   int i;

   for (i = 0; i < PAT_LEN; i++) {
      if (pattern[i] < NUM_CHARSETS - 1) {
         pattern[i]++;
         return 0;
      } else {
         if (i == 7) {
            /* patterns exhausted */
            break;
         }
         pattern[i] = 0;
      }
   }

   return 1;
}

/* At least one of each char in pattern? */
int pattern_pass_filter(void)
{
   int has_char[NUM_CHARSETS];
   int i;

   for (i = 0; i < NUM_CHARSETS; i++) {
      has_char[i] = 0;
   }

   for (i = 0; i < PAT_LEN; i++) {
      has_char[pattern[i]] = 1;
   }

   for (i = 0; i < NUM_CHARSETS; i++) {
      if (has_char[i] == 0) {
         return 0;
      }
   }

   return 1;
}

void print_pattern(void)
{
   int i;

   if (pattern_pass_filter()) {
      for (i = 0; i < PAT_LEN; i++) {
         printf("?%c", charsets[pattern[i]]);
      }
      printf("\n");
   }
}

int main(void)
{
   init_pattern();

   do {
      print_pattern();
   } while (inc_pattern() == 0);

   return 0;
}

