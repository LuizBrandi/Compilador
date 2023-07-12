/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_INT = 258,
    TK_FLOAT = 259,
    TK_CHAR = 260,
    TK_TIPO_BOOL = 261,
    TK_TRUE = 262,
    TK_FALSE = 263,
    TK_STRING = 264,
    TK_MAIN = 265,
    TK_ID = 266,
    TK_TIPO_INT = 267,
    TK_TIPO_FLOAT = 268,
    TK_TIPO_CHAR = 269,
    TK_TIPO_STRING = 270,
    TK_CONVERT_FLOAT = 271,
    TK_CONVERT_INT = 272,
    TK_FIM = 273,
    TK_ERROR = 274,
    TK_GREATER_EQUAL = 275,
    TK_LESS_EQUAL = 276,
    TK_EQUAL_EQUAL = 277,
    TK_NOT_EQUAL = 278,
    TK_AND = 279,
    TK_OR = 280,
    TK_NOT = 281,
    TK_VIRGULA = 282,
    TK_PRINT = 283,
    TK_IF = 284,
    TK_ELSE = 285,
    TK_WHILE = 286
  };
#endif
/* Tokens.  */
#define TK_INT 258
#define TK_FLOAT 259
#define TK_CHAR 260
#define TK_TIPO_BOOL 261
#define TK_TRUE 262
#define TK_FALSE 263
#define TK_STRING 264
#define TK_MAIN 265
#define TK_ID 266
#define TK_TIPO_INT 267
#define TK_TIPO_FLOAT 268
#define TK_TIPO_CHAR 269
#define TK_TIPO_STRING 270
#define TK_CONVERT_FLOAT 271
#define TK_CONVERT_INT 272
#define TK_FIM 273
#define TK_ERROR 274
#define TK_GREATER_EQUAL 275
#define TK_LESS_EQUAL 276
#define TK_EQUAL_EQUAL 277
#define TK_NOT_EQUAL 278
#define TK_AND 279
#define TK_OR 280
#define TK_NOT 281
#define TK_VIRGULA 282
#define TK_PRINT 283
#define TK_IF 284
#define TK_ELSE 285
#define TK_WHILE 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
