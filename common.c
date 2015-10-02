#include <math.h>
#include <stdio.h>

#include "common.h"

/**
   Prints a string.
*/
void output(char str[])
{
    printf("%s", str);
}

/**
   Returns the length of a string.
*/
unsigned _strlen(char str[])
{
    int i=0;
    while (str[i]!='\0') i++;
    return i;
}

/**
   Copies a string.
*/
void _strcpy(char dest[], char src[])
{
    int i=0;
    while(src[i]!='\0') {
	dest[i] = src[i];
	i++;
    }
    dest[i]='\0';
}

/**
   Checks if two string a equal.
*/
bool _streql(char str_a[], char str_b[])
{
    int i=0;
    do {
	if (str_a[i] != str_b[i]) return false;
	i++;
    } while (str_a[i]!='\0');
    return (str_b[i]=='\0');
}

/**
   Returns true if a character is a "whitespace character".
*/
bool iswhite(char c)
{
    return (c==' ')||(c=='\t')||(c=='\n');
}

/**
   Splits strings seperated by white spaces.
*/
unsigned split(char dest[][NAME_SIZE], char* src)
{
    int i=0, j=0, k=0;
    char buff[16];
    /* Remove extra white-spaces */
    while (iswhite(src[i])) i++;
    while (src[i]!='\0') {
	if (!iswhite(src[i])) {
	    buff[k]=src[i];
	    k++; i++;
	} else {
	    buff[k] = '\0';
	    _strcpy(dest[j], buff);
	    j++; k=0;
	    /* Remove extra white-spaces */
	    while (iswhite(src[i])) i++;
	}
    }
    if (k) {
	buff[k] = '\0';
	_strcpy(dest[j], buff);
	j++;
    }
    return j;
}

/**
   Rase base to the power of exp
*/
int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

/**
   Returns if string represents a number.
*/
bool is_int(char str[])
{
    int i = 0;
    while(str[i] != '\0') {
	if (str[i] != '-' && (str[i] < 48 || str[i] > 57)) {
	    return false;
	}
	i++;
    }
    return !(str[i-1] == '-');
}

/**
   Converts an integer to a string.
*/
void to_str(char str[], int v)
{
    int i, l;
    if (v == 0) l = 1;
    else l = (int) floor(log10(v))+1;
	
    for (i=0; i<l; i++) {
	int d = v / (int)pow(10, i) % 10;
	str[l-i-1] = (char) d+48;
    }
    str[l] = '\0';
}

/**
   Converts a string to an integer.
*/
int to_int(char str[])
{
    int i = 0, c = 0, r = 0, d, m = 0;;
    int l = _strlen(str);
    for (i=l-1; i>=0; i--) {
	if (str[i] == '-') m=!m;
	if (str[i] >= 48 && str[i] <= 57) {
	    d = str[i]-48;
	    r+=d*ipow(10, c);
	    c++;
	}
    }
    return m ? -r : r;
}
