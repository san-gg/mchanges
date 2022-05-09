#pragma once
#ifndef __TOKENIZER_
#define __TOKENIZER_
#define TOKENS int
#include <regex>
#include <fstream>
#include <vector>
#include "grammar.h"

class InvalidException : public std::exception {
public:
	InvalidException() : exception() { }
	InvalidException(const char* str) : exception(str) { }
	char const* what() {
		return exception::what();
	}
};

class JavaIgnoreGrammar {
	/*
		Assignment  : STRINGS '='
		StaticBlock : 'static' '{'
		Annotation  : '@' STRINGS
		import		: import STRINGS ';'
		IIB			: '{'
		STRINGS		: Text
					| <
					| ,
					| >
		//~~Simple~~
	*/
	int staticShift;
	int assignmentShift;
	int annotationShift;
	int importShift;
	int iibShift;
	enum class grammar {
		NONE = 0,
		StaticBlock = 1,
		Assignment = 2,
		Annotation = 3,
		Imports = 4,
		IIB = 5
	} ignoreGrammar;

	void checkStatic(const std::string&);
	void checkAssignment(const std::string&);
	void checkAnnotation(const std::string&);
	void checkImport(const std::string&);
	void checkIIB(const std::string&, int);
	void clearState();
public:
	JavaIgnoreGrammar();
	int checkGrammar(const std::string&, int);
	bool isStatic();
	bool isAssignment();
	bool isAnnotation();
	bool isImport();
	bool isIIB();
};

class Tokenizer {
	std::string data;
	std::string nextToken;
	std::basic_regex<char> mainContent;
	std::basic_regex<char> bodyContent;
	std::basic_regex<char> skipContent;
	std::sregex_iterator tokenItr;
	const std::sregex_iterator endTokenItr;
	std::ifstream file;
	std::vector<std::string> token_buffer;
	JavaIgnoreGrammar ignoreGrammar;
	int token_index;
	size_t lineno;
	bool classToken = false;

	TOKENS get_yylex_token(yy::parser::semantic_type*, std::string&);
	bool addTokenBuffer();
	void skipComments(const std::string&);
	void skipBlock();
	void skipAssignment();
	bool skipAnnotation();
	bool getNewLineM();
public:
	bool isComment = false;
	bool changeRegex = false;
	bool skipExpression = false;
	Tokenizer();
	void setFileName(const char*);
	TOKENS yylex(yy::parser::semantic_type*);
	void closeFile();
};
#endif // !__TOKENIZER_
