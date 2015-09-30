/**
Copyright (c) 2014 Richard van Roy (pluizer)
Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
**/

#ifndef __common_h_
#define __common_h_

/**
   Prints a string.
*/
extern void output(char str[]);

/**
   Returns the length of a string.
*/
extern unsigned _strlen(char str[]);

/**
   Copies a string.
*/
extern void _strcpy(char dest[], char src[]);

/**
   Checks if two string a equal.
*/
extern bool _streql(char str_a[], char str_b[]);

/**
   Returns true if a character is a "whitespace character".
*/
extern bool iswhite(char c);

/**
   Splits strings seperated by white spaces.
*/
extern unsigned split(char dest[][NAME_SIZE], char* src);

/**
   Rase base to the power of exp
*/
extern int ipow(int base, int exp);

/**
   Returns if string represents a number.
*/
extern bool is_int(char str[]);

/**
   Converts an integer to a string.
*/
extern void to_str(char str[], int v);

/**
   Converts a string to an integer.
*/
extern int to_int(char str[]);

#endif __common_h_
