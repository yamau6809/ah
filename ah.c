#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define YES 1
#define NO  0
#define EOS '\0'

long seek = 0;	/* seek value */
int oflag = NO;	/* address in octal */
int vflag = NO;	/* be verbos: print even the same line */
int sflag = NO;	/* do not print file name */

unsigned char v[68];	/* output buffer */
unsigned char vp[68];	/* previous contents of the output buffer */
unsigned char s[19];	/* ascii portion of the output buffer */

void ah(FILE *infp, char *fname);
long atol(char *string);

void main(int argc, char **argv)
{
  FILE *infp, *fopen();
  unsigned char c;
  long atol();

  while (--argc && ((c = **++argv) == '+' || c == '-')) {
    if ((*argv)[0] == '+') {
      if ((*argv)[1] == EOS)
	if (--argc)
	  seek = atol(*++argv);
	else
	  fprintf(stderr, "missing offset");
      else
	seek = atol(&(*argv)[1]);
    }
    else {
      while (c = *++(*argv)) {
	switch (c) {
	case 'o':
	  oflag++;
	  break;
	case 'v':
	  vflag++;
	  break;
	case 's':
	  sflag++;
	  break;
	default:
	  fprintf(stderr, "bad flag\n");
	}
      }
    }
  }
  s[4] = s[9] = s[14] = ' ';
  if (!argc)
    ah(stdin, "STDIN");
  else
    for (; argc--; ++argv) {
      if (!(infp = fopen(*argv, "rb"))) {
	fprintf(stderr, "ah: can't open %s\n",
		*argv);
	continue;
      }
      ah(infp, *argv);
      fclose(infp);
    }
}
 
void ah(FILE *infp, char *fname)
{
  unsigned long i, a;
  short c;
  unsigned char *p = v;
  int again = NO;

  if (seek) {
    if (infp == stdin)
      fprintf(stderr, "can't seek STDIN\n");
    else if (0 > fseek(infp, seek, 0))
      fprintf(stderr, "unexpected EOF on %s\n", fname);
  }
  if (!sflag) {
    if (infp == stdin)
      printf("STDIN\n");
    else
      printf("%s\n", fname);
  }
  i = a = 0;
  if (oflag)
    sprintf(p, "%07o ", i + seek);
  else
    sprintf(p, "%07x ", i + seek);
  p += 8;
  while ((c = getc(infp)) != EOF) {
    i++;
    a++;
    sprintf(p, "%1x%1x", (c & 0xf0) >> 4, c & 0xf);
    p += 2;
    if (c >= 0x20 && c <= 0x7e)
      s[a - 1] = (unsigned char)c;
    else
      s[a - 1] = '.';
    if (i % 16 == 0) {
      sprintf(p, "  %.19s", s);
      if (!vflag && !strncmp(v + 8, vp + 8, 58)) {
	if (!again)
	  printf("*\n");
	again = YES;
      } else {
	printf("%.68s\n", v);
	again = NO;
      }
      strncpy(vp, v, 68);
      p = v;
      a = 0;
      if (oflag)
	sprintf(p, "%07o", i + seek);
      else
	sprintf(p, "%07x", i + seek);
      p += 7;
    }
    if (i % 4 == 0) {
      sprintf(p, " ");
      p++;
      if (i % 16 != 0) {
	sprintf(p, " ");
	p++;
	a++;
      }
    }
  }
  while (i % 16 != 0) {
    sprintf(p, "  ");
    p += 2;
    i++;
    if (i % 4 == 0) {
      sprintf(p, "  ");
      p += 2;
    }
  }
  printf("%.48s", v);
  p = s;
  while (a--)
    putchar(*p++);
  putchar('\n');
  if (!sflag)
    putchar('\n');
}

long atol(char *string)
{
  long val = 0;
  long weight = 1;
  long base = 10;
  char *p;
  char c;

  if (*string == '0') {
    string++;
    base = 8;
    if (*string == 'x' || *string == 'X') {
      string++;
      base = 16;
    }
  }

  for (p = string; *p; p++)
    ;
  for (p--; p >= string; p--) {
    if ((c = *p) > '9' && base == 10 || c > '7' && base == 8)
      return (0);
    if (isdigit(c))
      val += weight * (c - '0');
    else if ('a' <= c && c <= 'f')
      val += weight * (c - 'W');
    else if ('A' <= c && c <= 'F')
      val += weight * (c - '7');
    else
      return (0);
    weight *= base;
  }
  return (val);
}
