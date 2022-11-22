/* main.c

   This file written 2015 by F Lundevall and David Broman

   Latest update 2015-09-15 by David Broman

   For copyright and licensing, see file COPYING */

#include <stddef.h>  /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>  /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */

void saveinfo(char s[], void *a, size_t ns);
void showinfo(void);
void u32init(void);

int gv;     /* Global variable. */
int in = 3; /* Global variable, initialized to 4711. */

void fun(int param)
{
  param++;
  saveword("AF1: param", &param);
  gv = param; /* Change the value of a global variable. */
}

/* This is the main function */
int main()
{
  /* Local variables. */
  int m;
  int *p; /* Declare p as pointer, so that p can hold an address. */
  char cs[9] = "Bonjour!";
  char *cp = cs; /* Declare cp as pointer, initialise cp to point to cs */

  /* Do some calculation. */
  gv = 4;
  m = gv + in;

  /*
  m= gv+in=7
  *p= NULL
  *cp='B'
  gv= 4
  in=3
  */

  // savebyte( char s[], void * a )
  // saveword( char s[], void * a )
  /* Check the addresses and values of some variables and stuff */
  saveword("AM1: gv", &gv); // gv=4
  saveword("AM2: in", &in); // in= 3
  saveword("AM3: fun", &fun);
  saveword("AM4: main", &main);

  p = &m;

  /*
  m=7
  *p=address of m
  *cp='B'
  gv= 4
  in=3
  */

  /* Check p and m */
  saveword("AM5: p", &p);
  saveword("AM6: m", &m);

  /* Change *p */

  *p = *p + 1;
  /*
  m=7
  *p=address of 1 after m
  *cp='B'
  gv= 4
  in=3
  */

  /* Check p and m */
  saveword("AM7: p", &p);
  saveword("AM8: m", &m);

  p = (int *)cp; /* Casting a char pointer to an integer pointer */

  /*
  m=7
  *p=int of 'B'=0x42
  *cp='B'
  gv= 4
  in=3
  */

  saveword("AM9: p", &p);

  savebyte("AM10: cs[0]", &cs[0]);
  savebyte("AM11: cs[1]", &cs[1]);
  savebyte("AM12: cs[2]", &cs[2]);
  savebyte("AM13: cs[3]", &cs[3]);

  *p = 0x1234abcd; /* It starts to get interesting... */

  /*
  m=7
  *p=0x1234abcd
  *cp='B'
  gv= 4
  in=3
  */

  savebyte("AM14: cs[0]", &cs[0]);
  savebyte("AM15: cs[1]", &cs[1]);
  savebyte("AM16: cs[2]", &cs[2]);
  savebyte("AM17: cs[3]", &cs[3]);

  fun(m);
  /*
  param++;
  saveword("AF1: param", &param);
  gv = param; // Change the value of a global variable.
  */

  /*
  m=7
  *p=0x1234abcd
  *cp='B'
  gv= m+1=8
  in=3
  */

  /* Re-check the addresses and values of m and gv */
  saveword("AM18: m", &m);   // m=7
  saveword("AM19: gv", &gv); // gv=8

  showinfo();
}
