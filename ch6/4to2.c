/* Convert four spaces indentation to two spaces indentation. */

#include <stdio.h>

int main (int argc, char **argv)
{
  FILE *src, *dest;
  int ch;
  int cnt;

  src = fopen (argv[1], "r");
  dest = fopen (argv[2], "w");

  cnt = 0;
  while ((ch = fgetc (src)) != -1)
  {
    if (ch == ' ')
      ++cnt;
    else if (ch == '\t')
      /* Change tab to equal spaces. */
      cnt = (cnt / 8 + 1) * 8; 
    else
    {
      if (cnt > 1)
	cnt /= 2;

      while (cnt-- > 0)
	fputc (' ', dest);
      fputc(ch, dest);

      /* Cnt would be -1 if there are two successive nonblank
       * characters, so set it to 0. */
      cnt = 0;
    }
  }

  fclose (src);
  fclose (dest);

  return 0;
}
