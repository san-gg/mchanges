#pragma once
#ifndef __TOKENIZER_
#define __TOKENIZER_
#define TOKENS int
//#define _HAS_ITERATOR_DEBUGGING 0
//#define _REGEX_MAX_STACK_COUNT 10000000L
//#include <regex>
#include <boost/regex.hpp>
#include <fstream>
#include <vector>
#include "grammar.h"
#include "../smhasher/MurmurHash3.h"

constexpr auto MAIN_CONTENT_REGEX = "(\\/\\*|\\*\\/|\\/\\/)|\"(?:\\\\.|[^\\\\\"])*\"|'(?:\\\\.|[^\\\\'])*'|[{}(),<>=;@?\\[\\]]|[.]+|[\\w]+";
constexpr auto BODY_REGEX         = "(\\/\\*|\\*\\/|\\/\\/)|\"(?:\\\\.|[^\\\\\"])*\"|'(?:\\\\.|[^\\\\'])*'|[{}]|[^\\s{}\"'/*]+";

class JavaIgnoreGrammar {
	/*
		Assignment  : STRINGS '='
		StaticBlock : 'static' '{'
		Annotation  : '@' STRINGS
		Semicolon   : STRINGS ';'
		Interface   : STRINGS interface
		IIB         : '{'
		STRINGS     : Text | < | ,| > | { | } | ( | )
		enum        : STRING 'enum' STRING {
	*/
	int staticShift;
	int assignmentShift;
	int annotationShift;
	int semicolonShift;
	int iibShift;
	int interfaceShift;
	int enumShift;
	enum class grammar {
		NONE = 0,
		StaticBlock = 1,
		Assignment = 2,
		Annotation = 3,
		Semicolon = 4,
		Interface = 5,
		IIB = 6,
		Enum = 7
	} ignoreGrammar;

	void checkStatic(const std::string&);
	void checkAssignment(const std::string&);
	void checkAnnotation(const std::string&);
	void checkSemicolon(const std::string&);
	void checkInterface(const std::string&);
	void checkIIB(const std::string&, size_t);
	void checkEnum(const std::string&);
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
	bool isEnum();
	void resetEverything();
};

class Tokenizer {
private:
	std::string data;
	std::string javaFileName;
	boost::basic_regex<char> mainContent;
	boost::basic_regex<char> bodyContent;
	boost::basic_regex<char> skipContent;
	boost::sregex_iterator tokenItr;
	const boost::sregex_iterator endTokenItr;
	std::ifstream file;
	std::vector<std::string> token_buffer;
	JavaIgnoreGrammar ignoreGrammar;
	size_t token_index;
	bool doBodyRegex;
	bool innerClass;
	bool takeGenericType;
	bool checkClass;
	inline static size_t lineno = 0;

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
	void assignFileName(const char*);
	std::string concateTypes();
	uint64_t* get_yylex_body();
public:
	Tokenizer();
	void setFileName(const char*);
	TOKENS yylex(yy::parser::semantic_type*);
	void closeFile();
	void changeBodyRegex();
	bool classStatus();
	static size_t getLineNo();
	~Tokenizer();
};

#endif // !__TOKENIZER_
