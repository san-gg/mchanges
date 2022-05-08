#include <iostream>
#include <string>
#include <stack>
#include "tokenizer.h"
#include "node.h"
#include "grammar.h"

JavaIgnoreGrammar::JavaIgnoreGrammar(): staticShift(0), assignmentShift(0), annotationShift(0), importShift(0),
										ignoreGrammar(JavaIgnoreGrammar::grammar::NONE) { }

void JavaIgnoreGrammar::clearState() {
	this->staticShift = 0;
	this->annotationShift = 0;
	this->assignmentShift = 0;
	this->importShift = 0;
	this->ignoreGrammar = JavaIgnoreGrammar::grammar::NONE;
}

void JavaIgnoreGrammar::checkStatic(const std::string& token) {
	int cmp1 = token.compare("static");
	int cmp2 = token.compare("{");
	if (this->staticShift == 1 && cmp1 == 0)
		throw yy::parser::syntax_error("Error at static keyword (2 static keyword found)");
	else if (this->staticShift == 0 && cmp1 == 0)
		this->staticShift = 1;
	else if (this->staticShift == 1 && cmp2 == 0)
		this->staticShift = 2;
}
void JavaIgnoreGrammar::checkAssignment(const std::string& token) {
	if (this->assignmentShift == 0 && token.compare("=") == 0)
		this->assignmentShift = 1;
}
void JavaIgnoreGrammar::checkAnnotation(const std::string& token) {
	int cmp = token.compare("@");
	if (this->annotationShift == 1 && cmp == 0)
		throw yy::parser::syntax_error("Invalid annotation");
	else if (this->annotationShift == 0 && cmp == 0)
		this->annotationShift = 1;
	else if (this->annotationShift == 1)
		this->annotationShift = 2;
}
void JavaIgnoreGrammar::checkImport(const std::string& token) {
	int cmp1 = token.compare("import");
	int cmp2 = token.compare(";");
	if (this->importShift == 1 && cmp1 == 0)
		throw yy::parser::syntax_error("Invalid import statement");
	else if (this->importShift == 0 && cmp1 == 0)
		this->importShift = 1;
	else if (this->importShift == 1 && cmp2 == 0)
		this->importShift = 2;
}

int JavaIgnoreGrammar::checkGrammar(const std::string &token) {
	// Accept and shift tokens
	checkStatic(token);
	checkAssignment(token);
	checkAnnotation(token);
	checkImport(token);


	// Check accepted grammar
	if (this->staticShift == 2) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::StaticBlock;
		return 1;
	}
	else if (this->annotationShift == 2) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::Annotation;
		return 1;
	}
	else if (this->assignmentShift == 1) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::Assignment;
		return 1;
	}
	else if (this->importShift == 2) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::Imports;
		return 1;
	}

	// Check unaccepted grammar
	if (token.compare("(") == 0 ||
		token.compare(")") == 0 ||
		token.compare(",") == 0 ||
		token.compare("}") == 0 ||
		(this->staticShift == 0 && token.compare("{") == 0))
	{
		clearState();
		return -1;
	}

	return 0;
}

bool JavaIgnoreGrammar::isStatic() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::StaticBlock;
}
bool JavaIgnoreGrammar::isAssignment() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::Assignment;
}
bool JavaIgnoreGrammar::isAnnotation() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::Annotation;
}
bool JavaIgnoreGrammar::isImport() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::Imports;
}

Tokenizer::Tokenizer() : token_index(0), endTokenItr(std::sregex_iterator()), lineno(0) {
	this->mainContent.assign("(\\/\\*|\\*\\/|\\/\\/)|(\".*\")|('.*')|([{}(),<>=;@]|[\\w]+)");
	this->bodyContent.assign("(\\/\\/|\\/\\*|\\*\\/)|(\".*\")|('.*')|([{}]|[\\S]*)");
	this->file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
}

void Tokenizer::setFileName(const char* fileName) {
	this->isComment = false;
	this->changeRegex = false;
	this->skipExpression = false;
	this->classToken = false;
	this->data.clear();
	this->nextToken.clear();
	this->token_buffer.clear();
	this->tokenItr = this->endTokenItr;
	file.open(fileName);
}

bool Tokenizer::getNewLineM() {
	if (std::getline(this->file, this->data)) {
		this->tokenItr = std::sregex_iterator(this->data.begin(), this->data.end(), this->mainContent);
		this->lineno += 1;
		return true;
	}
	return false;
}

void Tokenizer::skipComments(const std::string& token) {
	if (token.compare("//") == 0) {
		this->tokenItr = this->endTokenItr;
	}
	else if (token.compare("/*") == 0) {
		while (true) {
			if (this->tokenItr != this->endTokenItr) {
				if (this->tokenItr->str(1).compare("*/") == 0) break;
				this->tokenItr++;
			}
			else if (!getNewLineM())
					throw yy::parser::syntax_error("Reached end of file while parsing multi comment");
		}
		this->tokenItr++;
	}
	else {
		throw InvalidException("Failed to parse comment... (Potentially bug in the code)");
	}
}


void Tokenizer::skipBlock() {
	std::stack<char> st;
	st.push('{');
	this->tokenItr++;
	while (!st.empty()) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0) {
				skipComments(this->tokenItr->str(0));
				continue;
			}
			else if (this->tokenItr->str(4).compare("{") == 0)
				st.push('{');
			else if (this->tokenItr->str(4).compare("}") == 0)
				st.pop();
			this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("Reached end of file while parsing block");
	}
}

void Tokenizer::skipAssignment() {
	while (true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0) skipComments(this->tokenItr->str(0));
			else if (this->tokenItr->str(4).compare("{") == 0) skipBlock();
			else if (this->tokenItr->str(4).compare(";") == 0) break;
			else this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("Reached end of file while parsing assignment block");
	}
	this->tokenItr++;
}

bool Tokenizer::skipAnnotation() {
	std::stack<char> st;
	bool annotationWithParam = false;
	this->tokenItr++;
	while(true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0) {
				skipComments(this->tokenItr->str(1));
				continue;
			}
			else if (this->tokenItr->str(4).compare("(") == 0) {
				annotationWithParam = true;
				st.push('(');
			}
			else if (this->tokenItr->str(4).compare(")") == 0)
				st.pop();
			if (st.empty()) break;
			this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("Reached end of file while parsing annotation block");
	}
	if (annotationWithParam) this->tokenItr++;
	return annotationWithParam;
}

bool Tokenizer::addTokenBuffer() {
	if (this->data.length() == 0 && !getNewLineM())
		return false;

	while (true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0)
				skipComments(this->tokenItr->str(0));
			else {
				this->token_buffer.push_back(this->tokenItr->str(0));
				int doIgnore = this->ignoreGrammar.checkGrammar(this->tokenItr->str(0));
				if (doIgnore == -1) break;
				else if (doIgnore == 1) {
					this->token_buffer.clear();
					if (this->ignoreGrammar.isStatic()) skipBlock();
					else if (this->ignoreGrammar.isAssignment()) skipAssignment();
					else if (this->ignoreGrammar.isAnnotation()) skipAnnotation();
					else if (this->ignoreGrammar.isImport()) this->tokenItr++;
				}
				else
					this->tokenItr++;
			}
		}
		else if (!getNewLineM()) throw yy::parser::syntax_error("End of file reached");
	}
	this->tokenItr++;
	return true;
}

TOKENS Tokenizer::get_yylex_token(yy::parser::semantic_type* val, std::string& token) {
	int tokenNo = yy::parser::token::yytokentype::ERROR;
	if (token.compare("abstract") == 0) tokenNo = yy::parser::token::yytokentype::ABSTRACT;
	else if (token.compare(",") == 0) tokenNo = yy::parser::token::yytokentype::COMMA;
	else if (token.compare("}") == 0) tokenNo = yy::parser::token::yytokentype::C_BRACE;
	else if (token.compare("default") == 0) tokenNo = yy::parser::token::yytokentype::DEFAULT;
	else if (token.compare("native") == 0) tokenNo = yy::parser::token::yytokentype::NATIVE;
	else if (token.compare("{") == 0) tokenNo = yy::parser::token::yytokentype::O_BRACE;
	else if (token.compare("private") == 0) tokenNo = yy::parser::token::yytokentype::PRIVATE;
	else if (token.compare("protected") == 0) tokenNo = yy::parser::token::yytokentype::PROTECTED;
	else if (token.compare("public") == 0) tokenNo = yy::parser::token::yytokentype::PUBLIC;
	else if (token.compare("static") == 0) tokenNo = yy::parser::token::yytokentype::STATIC;
	else if (token.compare("strictfp") == 0) tokenNo = yy::parser::token::yytokentype::STRICTFP;
	else if (token.compare("synchronized") == 0) tokenNo = yy::parser::token::yytokentype::SYNCHRONIZED;
	else if (token.compare("class") == 0) tokenNo = yy::parser::token::yytokentype::T_CLASS;
	else {
		tokenNo = yy::parser::token::yytokentype::STRINGS;
		val->str = new std::string(token);
	}
	return tokenNo;
}


TOKENS Tokenizer::yylex(yy::parser::semantic_type* val) {
	if (this->token_index == this->token_buffer.size()) {
		this->token_index = 0;
		this->token_buffer.clear();
		if (!addTokenBuffer()) return yy::parser::token::yytokentype::ERROR;
	}
	TOKENS token = get_yylex_token(val, this->token_buffer[this->token_index++]);
	return token;
}

void Tokenizer::closeFile() {
	this->file.close();
}