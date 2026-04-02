//
// The following include files must come first.

#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include <iostream>
#include "tree.h"
#include "cool.h"
#include "stringtab.h"
#define yylineno curr_lineno;
extern int yylineno;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
typedef Program_class *Program;
class Class__class;
typedef Class__class *Class_;
class Feature_class;
typedef Feature_class *Feature;
class Formal_class;
typedef Formal_class *Formal;
class Expression_class;
typedef Expression_class *Expression;
class Case_class;
typedef Case_class *Case;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef list_node<Feature> Features_class;
typedef Features_class *Features;
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;

#define Program_EXTRAS                          \
virtual void semant() = 0;			\
virtual void dump_with_types(ostream&, int) = 0; 



#define program_EXTRAS                          \
void semant();     				\
void typecheck();				\
void dump_with_types(ostream&, int);         \
void symtab_visit_class(Class_);    		\
void symtab_visit_expr(Expression);			\
void symtab_visit_case(Case);


#define Class__EXTRAS                   \
virtual Symbol get_filename() = 0;      \
virtual void dump_with_types(ostream&,int) = 0;  


#define class__EXTRAS                                 \
Symbol get_filename() { return filename; }             \
void dump_with_types(ostream&,int);                    \
virtual Symbol get_name() = 0; 			\
virtual Symbol get_parent() = 0;		\
virtual Features get_features() = 0;
Class_ parent_class;


#define Feature_EXTRAS                                        \
virtual void dump_with_types(ostream&,int) = 0; 


#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    


#define method_EXTRAS 									\
Formals get_formals();									\
Expression get_expr();

#define attr_EXTRAS 									\
Symbol get_name();									\
Symbol get_type_decl();							\
Expression get_init();


#define Formal_EXTRAS                              \
virtual void dump_with_types(ostream&,int) = 0;


#define formal_EXTRAS                           \
void dump_with_types(ostream&,int);				\
Symbol get_name();								\
Symbol get_type_decl(); 	


#define Case_EXTRAS                             \
virtual void dump_with_types(ostream& ,int) = 0;


#define branch_EXTRAS                                   \
void dump_with_types(ostream& ,int);


#define Expression_EXTRAS                    \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
void check_type(); 							\
Expression_class() { type = (Symbol) NULL; }

#define Expression_SHARED_EXTRAS           \
void dump_with_types(ostream&,int); 		\
virtual Expression get_expr() = 0;

#define static_dispatch_EXTRAS 				\
Expressions get_actual();

#define dispatch_EXTRAS 				\
Expressions get_actual();

#define cond_EXTRAS 				\
Expression get_pred();				\
Expression get_then_expr();				\
Expression get_else_expr();

#define loop_EXTRAS 				\
Expression get_pred();				\
Expression get_body();				

#define block_EXTRAS 				\
Expressions get_body();

#define let_EXTRAS 				\
Symbol get_identifier();		\
Symbol get_type_decl();			\
Expression get_init();			\
Expression get_body();

#define typcase_EXTRAS 				\
Expression get_expr();			\
Cases get_cases();

#define branch_EXTRAS 				\
Symbol get_identifier();		\
Symbol get_type_decl();			\
Expression get_init();			\
Expression get_body();

#define isvoid_EXTRAS 				\
Expression get_expr();		

#define plus_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();

#define sub_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();

#define mul_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();

#define divide_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();

#define lt_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();

#define eq_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();

#define leq_EXTRAS 				\
Expression get_expr1();			\
Expression get_expr2();


#endif
