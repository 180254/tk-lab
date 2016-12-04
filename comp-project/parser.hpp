/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
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
    T_PROGRAM = 258,
    T_VAR = 259,
    T_ARRAY = 260,
    T_ARRAY_RANGE = 261,
    T_OF = 262,
    T_INTEGER = 263,
    T_REAL = 264,
    T_FUNCTION = 265,
    T_PROCEDURE = 266,
    T_BEGIN = 267,
    T_END = 268,
    T_IF = 269,
    T_THEN = 270,
    T_ELSE = 271,
    T_WHILE = 272,
    T_DO = 273,
    T_NOT = 274,
    T_DIV = 275,
    T_MOD = 276,
    T_AND = 277,
    T_OR = 278,
    T_ID = 279,
    T_NUM = 280,
    T_EQ = 281,
    T_NE = 282,
    T_LE = 283,
    T_GE = 284,
    T_LO = 285,
    T_GR = 286,
    T_ASSIGN = 287,
    UMINUS = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 54 "parser.y" /* yacc.c:1909  */

    std::string *str;
    std::vector<std::string*> *vstr;
    Type type;
    Mem *mem;
    

#line 96 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
