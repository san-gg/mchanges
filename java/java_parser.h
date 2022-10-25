#pragma once
#ifndef _JAVA_PARSER_
#define _JAVA_PARSER_
#include <exception>
#include "tokenizer.h"

class ParserException : public std::exception {
public:
	ParserException(std::string str):exception(str.c_str()) { }
	char const* what() {
		return exception::what();
	}
};

class JavaParser : public yy::parser {
	Tokenizer tokens;
public:
	void error(const std::string& msg) {
		std::string str;
		if (!tokens.classStatus())
			str.append("No class definition found. Note: interface will be ignored");
		else str.append(msg);
		throw ParserException(str);
	}
	void setFile(const char* file) {
		tokens.setFileName(file);
	}
	void closeFile() {
		tokens.closeFile();
	}
	int yylex(yy::parser::semantic_type* value) {
		return tokens.yylex(value);
	}
	void yylexBody() {
		tokens.changeBodyRegex();
	}
};

#endif // !_JAVA_PARSER_

