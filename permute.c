/* All Rights Reversed - No Rights Reserved */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* TODO: Refactor this to read utf8chars from file instead. */

/* List of commonly seen byte combinations.
 * Most common UTF-8 special characters are 2-bytes.
 * Feel free to add more chars here.
 */
char *utf8chars[] = {
   /* Beginning of most popular char list, posted
    * to john-contest mailing list by Royce.
    */
   "\x21",
   "\x22",
   "\x23",
   "\x24",
   "\x25",
   "\x26",
   "\x27",
   "\x28",
   "\x29",
   "\x2b",
   "\x2d",
   "\x2e",
   "\x3f",
   "\x5f",
   "\xc2\xa3",
   "\xc2\xa4",
   "\xc2\xa7",
   "\xc2\xb1",
   "\xc2\xb3",
   "\xc2\xb9",
   "\xc2\xba",
   "\xc2\xbf",
   "\xc3\x82",
   "\xc3\x83",
   "\xc3\x84",
   "\xc3\x90",
   "\xc3\x91",
   "\xc3\x94",
   "\xc3\x97",
   "\xc3\x98",
   "\xc3\x99",
   "\xc3\x9f",
   "\xc3\xa0",
   "\xc3\xa1",
   "\xc3\xa2",
   "\xc3\xa3",
   "\xc3\xa4",
   "\xc3\xa5",
   "\xc3\xa6",
   "\xc3\xa7",
   "\xc3\xa8",
   "\xc3\xa9",
   "\xc3\xaa",
   "\xc3\xab",
   "\xc3\xac",
   "\xc3\xad",
   "\xc3\xae",
   "\xc3\xaf",
   "\xc3\xb0",
   "\xc3\xb1",
   "\xc3\xb2",
   "\xc3\xb3",
   "\xc3\xb4",
   "\xc3\xb5",
   "\xc3\xb6",
   "\xc3\xb8",
   "\xc3\xb9",
   "\xc3\xba",
   "\xc3\xbc",
   "\xc3\xbd",
   "\xc3\xbf",
   "\xc5\x93",
   "\xc5\xa1",
   "\xc5\xbe",
   "\xd0\x91",
   "\xd0\x92",
   "\xd0\x93",
   "\xd0\x94",
   "\xd0\x97",
   "\xd0\x98",
   "\xd0\x9a",
   "\xd0\x9b",
   "\xd0\x9c",
   "\xd0\x9d",
   "\xd0\x9e",
   "\xd0\x9f",
   "\xd0\xad",
   "\xd0\xaf",
   "\xd0\xb0",
   "\xd0\xb1",
   "\xd0\xb2",
   "\xd0\xb3",
   "\xd0\xb4",
   "\xd0\xb5",
   "\xd0\xb6",
   "\xd0\xb7",
   "\xd0\xb8",
   "\xd0\xb9",
   "\xd0\xba",
   "\xd0\xbb",
   "\xd0\xbc",
   "\xd0\xbd",
   "\xd0\xbe",
   "\xd0\xbf",
   "\xd1\x80",
   "\xd1\x81",
   "\xd1\x82",
   "\xd1\x83",
   "\xd1\x85",
   "\xd1\x86",
   "\xd1\x87",
   "\xd1\x88",
   "\xd1\x8b",
   "\xd1\x8c",
   "\xd1\x8d",
   "\xd1\x8e",
   "\xd1\x8f",
   "\xd8\xa3",
   "\xd8\xa5",
   "\xd8\xa6",
   "\xd8\xa7",
   "\xd8\xa8",
   "\xd8\xa9",
   "\xd8\xaa",
   "\xd8\xab",
   "\xd8\xac",
   "\xd8\xad",
   "\xd8\xae",
   "\xd8\xaf",
   "\xd8\xb1",
   "\xd8\xb2",
   "\xd8\xb3",
   "\xd8\xb4",
   "\xd8\xb5",
   "\xd8\xb6",
   "\xd8\xb7",
   "\xd8\xb8",
   "\xd8\xb9",
   "\xd8\xba",
   "\xd9\x81",
   "\xd9\x82",
   "\xd9\x83",
   "\xd9\x84",
   "\xd9\x85",
   "\xd9\x86",
   "\xd9\x87",
   "\xd9\x88",
   "\xd9\x89",
   "\xd9\x8a",
   "\xd9\xa0",
   "\xd9\xa1",
   "\xd9\xa2",
   "\xd9\xa3",
   "\xd9\xa4",
   "\xd9\xa5",
   "\xd9\xa6",
   "\xd9\xa7",
   "\xd9\xa8",
   "\xd9\xa9",
   "\xd9\xab",
   "\xdb\xb1",
   "\xdb\xb3",
   "\xdb\xb5",
   NULL
};

/* Check return value of write, and retry if not everything written */
/* TODO: Use printf instead, it takes care of buffering, retries etc. */
static inline void write_stdout(char *str, int len)
{
   char *strp = str;
   int left = len;
   int n;
   while (left > 0) {
      n = write(1, strp, left);
      if (n > 0) {
         left -= n;
         strp += n;
      }
   }
}

/* Insert utf8chars at all positions in str */
void print_permuted(char *str, int len)
{
   int i;
   char *utfp = utf8chars[0];
   int utfc = 0;

   if (len > 0) {
      while (utfp) {
         for (i = 0; i <= len; i++) {
            /* Before part */
            write_stdout(str, i);
            /* The unicode 2-byte character */
            write_stdout(utfp, strlen(utfp));
            /* After part */
            if (i < len) {
               write_stdout(&str[i], len - i);
            }
            write_stdout("\n", 1);
         }
         utfc++;
         utfp = utf8chars[utfc];
      }
   }
}

int main(int argc, char *argv[])
{
   FILE *fp;
   char line[256];

   if (argc < 2) {
      fprintf(stderr, "Usage: %s <wordlist>\n", argv[0]);
      return 1;
   }

   fp = fopen(argv[1], "r");
   if (!fp) {
      perror(argv[1]);
      return 1;
   }

   /* For each line, insert utf-8 from list at all positions */
   while (fgets(line, 255, fp)) {
      char *end = strchr(line, '\n');
      if (end) {
         print_permuted(line, end - line);
      }
   }

   fclose(fp);

   return 0;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
