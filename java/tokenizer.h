#pragma once
#ifndef __TOKENIZER_
#define __TOKENIZER_
#define TOKENS int
#define _HAS_ITERATOR_DEBUGGING 0
#define _REGEX_MAX_STACK_COUNT 10000000L
#include <regex>
#include <fstream>
#include <vector>
#include "grammar.h"
#include "../smhasher/MurmurHash3.h"

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
		Semicolon   : STRINGS ';'
		Interface   : STRINGS interface
		IIB         : '{'
		STRINGS     : Text | < | ,| > | { | } | ( | )
		//~~Simple~~
	*/
	int staticShift;
	int assignmentShift;
	int annotationShift;
	int semicolonShift;
	int iibShift;
	int interfaceShift;
	enum class grammar {
		NONE = 0,
		StaticBlock = 1,
		Assignment = 2,
		Annotation = 3,
		Semicolon = 4,
		Interface = 5,
		IIB = 6
	} ignoreGrammar;

	void checkStatic(const std::string&);
	void checkAssignment(const std::string&);
	void checkAnnotation(const std::string&);
	void checkSemicolon(const std::string&);
	void checkInterface(const std::string&);
	void checkIIB(const std::string&, size_t);
	void clearState();
public:
	JavaIgnoreGrammar();
	int checkGrammar(const std::string&, size_t);
	bool isStatic();
	bool isAssignment();
	bool isAnnotation();
	bool isSemicolon();
	bool isInterface();
	bool isIIB();
	void resetEverything();
};

class Tokenizer {
	std::string data;
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
	bool doBodyRegex;
	bool innerClass;

	TOKENS get_yylex_token(yy::parser::semantic_type*, std::string&);
	bool addTokenBuffer();
	void skipComments(const std::string&, bool);
	void skipBlock();
	void skipAssignment();
	void skipToToken(const char*);
	void appendTypeArray();
	bool skipAnnotation();
	bool getNewLineM();
	bool getNewLineB();
	bool checkBlockPresent();
	std::string concateTypes();
	uint64_t* get_yylex_body();
public:
	Tokenizer();
	void setFileName(const char*);
	TOKENS yylex(yy::parser::semantic_type*);
	void closeFile();
	void changeBodyRegex();
	bool classStatus();
	int lineNo();
	~Tokenizer();
};
#endif // !__TOKENIZER_
