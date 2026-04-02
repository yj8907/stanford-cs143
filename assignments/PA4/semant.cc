

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"


extern int semant_debug;
extern char *curr_filename;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any 
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}



ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {

    /* Fill this in */
    class_table.enterscope();
    class_inheritance_check_table.enterscope();

    for (int i = classes->first(); classes->more(i); i=classes->next(i)) {
        class__class *cls_i = dynamic_cast<class__class*>(classes->nth(i));
        class_table.addid(cls_i->get_name(), cls_i);
        class_inheritance_check_table.addid(cls_i->get_name(), 0);
    }

    if (!check_dependency()) return;


}

bool ClassTable::check_dependency() {
    
    for (int i = classes->first(); classes->more(i); i=classes->next(i)) {
        class__class *cls_i = dynamic_cast<class__class*>(classes->nth(i));
        if ( *class_inheritance_check_table.lookup(cls_i->get_name()) == 0){
            bool isvalid  = check_parent(class_table.lookup(cls_i->get_name())->get_parent());
            if (!isvalid) return isvalid;
        }
        if ( *class_inheritance_check_table.lookup(cls_i->get_name()) == 1) return false;
    }
    return true;
}

bool ClassTable::check_parent(Symbol current_cls) {
    if ( *class_inheritance_check_table.lookup(current_cls) == 1) return false;

    if ( *class_inheritance_check_table.lookup(current_cls) == 0) {
        *class_inheritance_check_table.lookup(current_cls) = 1;
        bool isvalid = check_parent(class_table.lookup(current_cls)->get_parent());
        if (isvalid) *class_inheritance_check_table.lookup(current_cls) = 2;
        return isvalid;
    }
    return true;
}

void ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
   // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
	class_(Object, 
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class = 
	class_(IO, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);  

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    Class_ Int_class =
	class_(Int, 
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       
    Class_ Str_class =
	class_(Str, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat, 
								      single_Formals(formal(arg, Str)),
								      Str, 
								      no_expr()))),
			       single_Features(method(substr, 
						      append_Formals(single_Formals(formal(arg, Int)), 
								     single_Formals(formal(arg2, Int))),
						      Str, 
						      no_expr()))),
	       filename);
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{                                                             
    return semant_error(c->get_filename(),c);
}    

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 



/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */
static SymbolTable<Symbol, Entry> variable_types;

void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    // classes
    
    ClassTable *classtable = new ClassTable(classes);

    /* some semantic analysis code may go here */
    

    if (classtable->errors()) {
	cerr << "Compilation halted due to static semantic errors." << endl;
	exit(1);
    }
}


void program_class::typecheck() 
{

}


void program_class::symtab_visit_class(Class_ cls) 
{   
    class__class* cls_impl = dynamic_cast<class__class*>(cls);
    Features features = cls_impl->get_features();

    variable_types.enterscope();

    for (int i = features->first(); features->more(i); i=features->next(i)){
        if (method_class* m_cls = dynamic_cast<method_class*>(features->nth(i))){
            Formals fmls = m_cls->get_formals();
            for (int fi = fmls->first(); fmls->more(fi); fmls->next(fi)){
                formal_class* fml_cls = dynamic_cast<formal_class*>(fmls->nth(fi));
                idtable.add_string(fml_cls->get_type_decl()->get_string());
                variable_types.addid(fml_cls->get_name(), fml_cls->get_type_decl());
            }
        }
        if (attr_class* attr = dynamic_cast<attr_class*>(features->nth(i))) {
            variable_types.addid(attr->get_name(), attr->get_type_decl());
        }
    }

    for (int i = features->first(); features->more(i); i=features->next(i)){
        if (method_class* m_cls = dynamic_cast<method_class*>(features->nth(i))){
            symtab_visit_expr(m_cls->get_expr());
        }
        if (attr_class* attr = dynamic_cast<attr_class*>(features->nth(i))) {
            symtab_visit_expr(attr->get_init());
        }
    }

    variable_types.exitscope();

}

void program_class::symtab_visit_expr(Expression expr)
{
    variable_types.enterscope();
    if (assign_class* as = dynamic_cast<assign_class*>(expr)) {
        symtab_visit_expr(as->get_expr());
    }
    if (static_dispatch_class* static_dispatch = dynamic_cast<static_dispatch_class*>(expr)) {
        symtab_visit_expr(static_dispatch->get_expr());
        Expressions actual = static_dispatch->get_actual();
        for (int i = actual->first(); actual->more(i); i=actual->next(i)) {
            symtab_visit_expr(actual->nth(i));
        }
    }
    if (dispatch_class* dispatch = dynamic_cast<dispatch_class*>(expr)) {
        symtab_visit_expr(dispatch->get_expr());
        Expressions actual = dispatch->get_actual();
        for (int i = actual->first(); actual->more(i); i=actual->next(i)) {
            symtab_visit_expr(actual->nth(i));
        }
    }
    if (cond_class* cond = dynamic_cast<cond_class*>(expr)) {
        symtab_visit_expr(cond->get_pred());
        symtab_visit_expr(cond->get_then_expr());
        symtab_visit_expr(cond->get_else_expr());
    }
    if (loop_class* loop = dynamic_cast<loop_class*>(expr)) {
        symtab_visit_expr(loop->get_pred());
        symtab_visit_expr(loop->get_body());
    }
    if (block_class* block = dynamic_cast<block_class*>(expr)) {
        Expressions exprs = block->get_body();
        for (int i = exprs->first(); exprs->more(i); i=exprs->next(i)) {
            symtab_visit_expr(exprs->nth(i));
        }
    }
    if (let_class* let = dynamic_cast<let_class*>(expr)) {
        variable_types.addid(let->get_identifier(), let->get_type_decl());
        symtab_visit_expr(let->get_init());
        symtab_visit_expr(let->get_expr());
    }
    if (typcase_class* typcase = dynamic_cast<typcase_class*>(expr)) {
        symtab_visit_expr(typcase->get_expr());
        Cases cases = typcase->get_cases();
        for (int i = cases->first(); cases->more(i); i=cases->next(i)) {
            symtab_visit_case(cases->nth(i));
        }
    }
    if (new__class* newcls = dynamic_cast<new__class*>(expr)) {
        return;
    }
    if (isvoid_class* isvoid = dynamic_cast<isvoid_class*>(expr)) {
        symtab_visit_expr(isvoid->get_expr());
    }
    if (plus_class* plus = dynamic_cast<plus_class*>(expr)) {
        symtab_visit_expr(plus->get_expr1());
        symtab_visit_expr(plus->get_expr2());
    }
    if (sub_class* sub = dynamic_cast<sub_class*>(expr)) {
        symtab_visit_expr(sub->get_expr1());
        symtab_visit_expr(sub->get_expr2());
    }
    if (mul_class* mul = dynamic_cast<mul_class*>(expr)) {
        symtab_visit_expr(mul->get_expr1());
        symtab_visit_expr(mul->get_expr2());
    }
    if (divide_class* div = dynamic_cast<divide_class*>(expr)) {
        symtab_visit_expr(div->get_expr1());
        symtab_visit_expr(div->get_expr2());
    }
    if (neg_class* neg = dynamic_cast<neg_class*>(expr)) {
        symtab_visit_expr(neg->get_expr());
    }
    if (lt_class* lt = dynamic_cast<lt_class*>(expr)) {
        symtab_visit_expr(lt->get_expr1());
        symtab_visit_expr(lt->get_expr2());
    }
    if (leq_class* leq = dynamic_cast<leq_class*>(expr)) {
        symtab_visit_expr(leq->get_expr1());
        symtab_visit_expr(leq->get_expr2());
    }
    if (comp_class* leq = dynamic_cast<comp_class*>(expr)) {
        symtab_visit_expr(leq->get_expr());
    }
    variable_types.exitscope();
}



