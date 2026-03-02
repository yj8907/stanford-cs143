/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;
int left_bracket_count = 0;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */
%}

%x COMMENT STR

/*
 * Define names for regular expressions here.
 */
INTEGER  [0-9]+
CLASS (?i:CLASS)
ELSE (?i:ELSE)
FI (?i:FI)
IN (?i:IN)
INHERITS (?i:INHERITS)
LET (?i:LET)
LOOP (?i:LOOP)
POOL (?i:POOL)
THEN (?i:THEN)
WHILE (?i:WHILE)
CASE (?i:CASE)
ESAC (?i:ESAC)
OF (?i:OF)
NEW (?i:NEW)
ISVOID (?i:ISVOID)
NOT (?i:NOT)
TRUE (true)
FALSE (false)
SELF_IDENTIFIER (?i:self)
SELF_TYPE_IDENTIFIER (?i:SELF_TYPE) 

OBJECT_IDENTIFIER [a-z][a-zA-Z0-9_]*
TYPE_IDENTIFIER [A-Z][a-zA-Z0-9_]*
COMMENTS (--.*\n)

DARROW =>
WHITESPACE [ \n\f\r\t\v]+

%%

 /*
  *  Nested comments
  */

  /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */
{INTEGER} {
cool_yylval.symbol = inttable.add_string(yytext);
return INT_CONST;
}

{CLASS} {
cool_yylval.symbol = stringtable.add_string(yytext);
return CLASS;
}

{ELSE} {
cool_yylval.symbol = stringtable.add_string(yytext);
return ELSE;
}

{FI} {
cool_yylval.symbol = stringtable.add_string(yytext);
return FI;
}

{IN} {
cool_yylval.symbol = stringtable.add_string(yytext);
return IN;
}

{INHERITS} {
cool_yylval.symbol = stringtable.add_string(yytext);
return INHERITS;
}

{LET} {
cool_yylval.symbol = stringtable.add_string(yytext);
return LET;
}

{LOOP} {
cool_yylval.symbol = stringtable.add_string(yytext);
return LOOP;
}

{POOL} {
cool_yylval.symbol = stringtable.add_string(yytext);
return POOL;
}

{THEN} {
cool_yylval.symbol = stringtable.add_string(yytext);
return THEN;
}

{WHILE} {
cool_yylval.symbol = stringtable.add_string(yytext);
return WHILE;
}

{CASE} {
cool_yylval.symbol = stringtable.add_string(yytext);
return CASE;
}

{ESAC} {
cool_yylval.symbol = stringtable.add_string(yytext);
return ESAC;
}

{OF} {
cool_yylval.symbol = stringtable.add_string(yytext);
return OF;
}

{NEW} {
cool_yylval.symbol = stringtable.add_string(yytext);
return NEW;
}

{ISVOID} {
cool_yylval.symbol = stringtable.add_string(yytext);
return ISVOID;
}

{NOT} {
cool_yylval.symbol = stringtable.add_string(yytext);
return NOT;
}

{TRUE} {
cool_yylval.symbol = stringtable.add_string(yytext);
return BOOL_CONST;
}

{FALSE} {
cool_yylval.symbol = stringtable.add_string(yytext);
return BOOL_CONST;
}

"<-" {
cool_yylval.symbol = stringtable.add_string(yytext);
return ASSIGN;
}

"<=" {
cool_yylval.symbol = stringtable.add_string(yytext);
return LE;
}

 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */
 
\" { 
  string_buf_ptr = string_buf;
  BEGIN(STR);
  }
<STR>\\[btnf] { 
  for (int i=0; i<yyleng; i++){
      *string_buf_ptr++ = yytext[i];
    }
}
<STR>\\[^btnf] {
    *string_buf_ptr++ = yytext[1];
  }
<STR>\\[\n] {
  for (int i=0; i<yyleng; i++){
      *string_buf_ptr++ = yytext[i];
    }
}
<STR>[^\"]+ {
    for (int i=0; i<yyleng; i++){
      *string_buf_ptr++ = yytext[i];
    }
  }
<STR>\" {
  *string_buf_ptr = '\0';
  cool_yylval.symbol = stringtable.add_string(string_buf);
  BEGIN(INITIAL);
  return STR_CONST;
}

 /*
  *  The multiple-character operators.
  */
{OBJECT_IDENTIFIER} {
  cool_yylval.symbol = inttable.add_string(yytext);
  return OBJECTID;
}
{TYPE_IDENTIFIER} {
  cool_yylval.symbol = inttable.add_string(yytext);
  return TYPEID;
}
{DARROW}		{ return DARROW; }
{COMMENTS};

{WHITESPACE} {
  if (yytext[0] == '\n') curr_lineno++;
} 

  /* comments 
  */

"(*" {
  left_bracket_count += 1;
  BEGIN(COMMENT);
}
<COMMENT>"(*" {
  left_bracket_count += 1;
}
<COMMENT>"*)" {
  left_bracket_count -= 1;
  if (left_bracket_count == 0) BEGIN(INITIAL);
}
<COMMENT>.|\n {
}

. {
cool_yylval.symbol = stringtable.add_string(yytext);
return STR_CONST;
}

%%


