#ifndef UTIL_GLSL_TOKENIZER_HPP
#define UTIL_GLSL_TOKENIZER_HPP

#include <vector> // For std::vector<T>
#include <string> // For std::string

#include <boost/any.hpp> // For boost::any

enum TokenType {
	T_NONE = 0,
	
	// ----- Types -----
	T_BOOL,   // Boolean type
	T_INT,   	 // Integer type
	T_UINT,   	 // Unsigned integer type
	T_FLOAT,  	 // Single precision type
	T_DOUBLE, 	 // Double precision type
	T_ARRAY,  	 // Array type (size)
	T_SAMPLER2D, // Texture sampler (texture)

	T_VEC, // N	    dimensional vector (type, n, {x, y, z, w})
	T_MAT, // N x M dimensional matrix (type, n, m  {contents...})

	// ----- Declarations -----
	T_UNIFORM, // Uniform parameter (name, type, default_value)
	T_IN,	   // In parameter (name, type)
	T_OUT,	   // Out parameter (name, type)

	T_LOCAL,   // Local variable (name, type)
	T_GLOBAL,  // Global variable (name) 

	T_STRUCT, // Struct definition (name, {member_name, member_type}....)
   	
	T_FUNCDECL, // Function definition (name, return_type, {arg_name, arg_type}...)

	// ----- Operations ----- 
	T_ADD,	// Addition (type, type)
	T_SUB,	// Subtraction (type, type)
	T_MULT,	// Multiplication (type, type)
	T_DIV,	// Division (type, type)

	T_FUNCCALL // Function call (func_name, {func_param_name, func_param_type}...
};

struct Token {
	TokenType type;
	std::vector<boost::any> values;
};

#endif
