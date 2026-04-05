#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"

#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

class ClassTable {
private:
  int semant_errors;
  void install_basic_classes();
  bool check_dependency() {}
  bool check_parent(Symbol) {}

  ostream& error_stream;
  SymbolTable<Symbol, class__class> class_table;
  SymbolTable<Symbol, int> class_inheritance_check_table;
  Classes classes;

public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  Classes get_classes();
  class__class* lookup_class(Symbol);

  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);
};

class Semant {
  private: 
    ClassTable *class_table;
    Class_ current_class;

    Program_class* program;
    SymbolTable<Symbol, Entry>* variables;
    SymbolTable<Symbol, Signature>* methods;

    void visit_class(Class_);
    void visit_expr(Expression);
    void visit_case(Case);

    void type_check_class(Class_);
    void type_check_method(method_class*);
    void type_check_attr(attr_class*);

    void type_check_static_dispatch(static_dispatch_class*);
    void type_check_dispatch(dispatch_class*);
    void type_check_cond(cond_class*);
    void type_check_loop(loop_class*);
    void type_check_block(block_class*);
    void type_check_let(let_class*);
    
    void type_check_typcase(typcase_class*);
    void type_check_newcls(new__class*);
    void type_check_isvoid(isvoid_class*);
    void type_check_arithmetic(Expression);
    void type_check_compare(Expression);

    bool conform(Symbol derived, Symbol base);

  public:
    Semant(ClassTable* ct, Program_class* ps) :class_table(ct), program(ps) {};
    void visit();
    void type_check();
};

#endif

