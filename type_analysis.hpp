#ifndef HOLEYC_TYPE_ANALYSIS
#define HOLEYC_TYPE_ANALYSIS

#include "ast.hpp"
#include "symbol_table.hpp"
#include "types.hpp"

class NameAnalysis;

namespace holeyc{

// An instance of this class will be passed over the entire
// AST. Rather than attaching types to each node, the 
// TypeAnalysis class contains a map from each ASTNode to it's
// DataType. Thus, instead of attaching a type field to most nodes,
// one can instead map the node to it's type, or lookup the node
// in the map.
class TypeAnalysis {

private:
	//The private constructor here means that the type analysis
	// can only be created via the static build function
	// TypeAnalysis(){
	// 	hasError = false;
	// }

public:
  TypeAnalysis(){
    hasError = false;
  }
  static TypeAnalysis * build(NameAnalysis * astRoot);
	//static TypeAnalysis * build();

	//The type analysis has an instance variable to say whether
	// the analysis failed or not. Setting this variable is much
	// less of a pain than passing a boolean all the way up to the
	// root during the TypeAnalysis pass. 
	bool passed(){
		return !hasError;
	}

	void setCurrentFnType(const FnType * type){
		currentFnType = type;
	}

	const FnType * getCurrentFnType(){
		return currentFnType;
	}

	
	//Set the type of a node. Note that the function name is 
	// overloaded: this 2-argument nodeType puts a value into the
	// map with a given type. 
	void nodeType(const ASTNode * node, const DataType * type){
		nodeToType[node] = type;
	}

	//Gets the type of a node already placed in the map. Note
	// that this function name is overloaded: the 1-argument nodeType
	// gets the type of the given node out of the map.
	const DataType * nodeType(const ASTNode * node){
		const DataType * res = nodeToType[node];
		if (res == nullptr){
			const char * msg = "No type for node ";
			throw new InternalError(msg);
		}
		return nodeToType[node];
	}

	//The following functions all report and error and 
	// tell the object that the analysis has failed. 

	void badWriteFn(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Attempt to output a function");
	}

	void badWriteVoid(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Attempt to write void");
	}

	void badReadFn(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Attempt to read a function");
	}

	void badCallee(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Attempt to call a "
			"non-function");
	}
	void badArgCount(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Function call with wrong"
			" number of args");
	}
	void badArgMatch(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Type of actual does not match"
			" type of formal");
	}
	void badNoRet(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Missing return value");
	}
	void extraRetValue(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Return with a value in void"
			" function");
	}
	void badRetValue(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Bad return value");
	}
	void badMathOpd(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Arithmetic operator applied"
			" to invalid operand");
	}
	void badMathOpr(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Arithmetic operator applied"
			" to incompatible operands");
	}
	void badRelOpd(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Relational operator applied to"
			" non-numeric operand");
	}
	void badLogicOpd(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Logical operator applied to"
			" non-bool operand");
	}
	void badIfCond(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Non-bool expression used as"
			" an if condition");
	}
	void badWhileCond(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Non-bool expression used as"
			" a while condition");
	}
	void badEqOpd(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Invalid equality operand");
	}
	void badEqOpr(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Invalid equality operation");
	}
	void badAssignOpr(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Invalid assignment operation");
	}
	void badAssignOpd(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, 
			"Invalid assignment operand");
	}

	void badWritePtr(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Attempt to write a raw pointer");
	}

	
	void fnDeref(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col,
			"Attempt to dereference a function");
	}
	void badIndex(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, "Bad index type");
	}
	void badPtrBase(size_t line, size_t col){
		hasError = true;
		Report::fatal(line, col, "Attempt to index"
		  "a non-pointer type"
		);
	}
	void badRefOpd(size_t line, size_t col){
		Report::fatal(line, col, "Invalid ref operand");
	}

private:
	HashMap<const ASTNode *, const DataType *> nodeToType;
	const FnType * currentFnType;
	bool hasError;
public:
	ProgramNode * ast;
};

}
#endif
