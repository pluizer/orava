#ifndef __orava_h_
#define __orava_h_

/**
   Simply pass valid Orava-code to this function to have it
   evaluated.
*/
extern void eval(char str[]);

/**
   Use this function to extend Orava.
   @param str	The name of the new command.
   @param func 	A valid Orava function to call when this command
   		is evaluated. Valid commands take no arguments 
		and return no value.
*/
extern void compile_builtin(char str[], void(*func)());

#endif // __orava_h_
