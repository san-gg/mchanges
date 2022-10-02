#include <iostream>
#include <string>
#include "tokenizer.h"
#include "node.h"
#include "grammar.h"

JavaIgnoreGrammar::JavaIgnoreGrammar(): staticShift(0), assignmentShift(0), annotationShift(0),
										semicolonShift(0), iibShift(0), interfaceShift(0),
										ignoreGrammar(JavaIgnoreGrammar::grammar::NONE) { }

void JavaIgnoreGrammar::clearState() {
	this->staticShift = 0;
	this->annotationShift = 0;
	this->assignmentShift = 0;
	this->semicolonShift = 0;
	this->interfaceShift = 0;
	this->iibShift = 0;
	this->ignoreGrammar = JavaIgnoreGrammar::grammar::NONE;
}

void JavaIgnoreGrammar::resetEverything() {
	clearState();
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
void JavaIgnoreGrammar::checkSemicolon(const std::string& token) {
	int cmp = token.compare(";");
	if (this->semicolonShift == 1 && cmp == 0)
		throw yy::parser::syntax_error("Invalid import statement");
	else if (this->semicolonShift == 0 && cmp == 0)
		this->semicolonShift = 1;
}
void JavaIgnoreGrammar::checkInterface(const std::string& token) {
	int cmp1 = token.compare("interface");
	int cmp2 = token.compare("{");
	if (this->interfaceShift == 1 && cmp1 == 0)
		throw yy::parser::syntax_error("Error at interface keyword (2 interface keyword found)");
	else if (this->interfaceShift == 0 && cmp1 == 0)
		this->interfaceShift = 1;
	else if (this->interfaceShift == 1 && cmp2 == 0)
		this->interfaceShift = 2;
}
void JavaIgnoreGrammar::checkIIB(const std::string& token, size_t tokenBlockSize) {
	int cmp = token.compare("{");
	if (this->iibShift == 0 && tokenBlockSize == 1 && cmp == 0)
		this->iibShift = 1;
}

int JavaIgnoreGrammar::checkGrammar(const std::string &token, size_t tokenBlockSize) {
	// Accept and shift tokens
	checkStatic(token);
	checkAssignment(token);
	checkAnnotation(token);
	checkSemicolon(token);
	checkInterface(token);
	checkIIB(token, tokenBlockSize);


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
	else if (this->semicolonShift == 1) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::Semicolon;
		return 1;
	}
	else if (this->interfaceShift == 2) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::Interface;
		return 1;
	}
	else if (this->iibShift == 1) {
		clearState();
		this->ignoreGrammar = JavaIgnoreGrammar::grammar::IIB;
		return 1;
	}

	// Check unaccepted grammar
	if (token.compare("(") == 0 ||
		token.compare(")") == 0 ||
		token.compare("}") == 0 ||
		(this->staticShift == 0 && token.compare("{") == 0) ||
		(this->interfaceShift == 0 && token.compare("{") == 0)
	)
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
bool JavaIgnoreGrammar::isSemicolon() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::Semicolon;
}
bool JavaIgnoreGrammar::isInterface() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::Interface;
}
bool JavaIgnoreGrammar::isIIB() {
	return this->ignoreGrammar == JavaIgnoreGrammar::grammar::IIB;
}

Tokenizer::Tokenizer() : token_index(0), endTokenItr(std::sregex_iterator()), lineno(0),
						doBodyRegex(false), innerClass(false) {
	this->mainContent.assign("(\\/\\*|\\*\\/|\\/\\/)|\"(?:\\\\.|[^\\\\\"])*\"|'(?:\\\\.|[^\\\\'])*'|[{}(),<>=;@\\[\\]?]|[\\w]+");
	this->bodyContent.assign("(\\/\\/|\\/\\*|\\*\\/)|\"(?:\\\\.|[^\\\\\"])*\"|'(?:\\\\.|[^\\\\'])*'|[{}]|[^\\s{}\"'/*]+|[*]+(?=\\*\\/)|[*]+");
	this->file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
}

void Tokenizer::setFileName(const char* fileName) {
	try { this->file.close(); }
	catch (...) {}
	this->lineno = 0;
	this->token_index = 0;
	this->doBodyRegex = false;
	this->innerClass = false;
	this->data.clear();
	this->token_buffer.clear();
	this->tokenItr = this->endTokenItr;
	this->file.open(fileName);
}

bool Tokenizer::getNewLineM() {
	try {
		if (std::getline(this->file, this->data)) {
			this->tokenItr = std::sregex_iterator(this->data.begin(), this->data.end(), this->mainContent);
			this->lineno += 1;
		}
	}
	catch (...) {
		return false;
	}
	return true;
}
bool Tokenizer::getNewLineB() {
	try {
		if (std::getline(this->file, this->data)) {
			this->tokenItr = std::sregex_iterator(this->data.begin(), this->data.end(), this->bodyContent);
			this->lineno += 1;
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

void Tokenizer::skipComments(const std::string& token, bool useBodyRegex = false) {
	if (token.compare("//") == 0) {
		this->tokenItr = this->endTokenItr;
	}
	else if (token.compare("/*") == 0) {
		while (true) {
			if (this->tokenItr != this->endTokenItr) {
				if (this->tokenItr->str(1).compare("*/") == 0) break;
				this->tokenItr++;
			}
			else if ((!useBodyRegex && !getNewLineM()) || (useBodyRegex && !getNewLineB()))
					throw yy::parser::syntax_error("Reached end of file while parsing multi comment");
		}
		this->tokenItr++;
	}
	else {
		throw InvalidException("Failed to parse comment... (Potentially bug in the code)");
	}
}


void Tokenizer::skipBlock() {
	int stack = 1;
	this->tokenItr++;
	while (stack > 0) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0) {
				skipComments(this->tokenItr->str(0));
				continue;
			}
			else if (this->tokenItr->str(0).compare("{") == 0) stack += 1;
			else if (this->tokenItr->str(0).compare("}") == 0) stack -= 1;
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
			else if (this->tokenItr->str(0).compare("{") == 0) skipBlock();
			else if (this->tokenItr->str(0).compare(";") == 0) break;
			else this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("Reached end of file while parsing assignment block");
	}
	this->tokenItr++;
}

bool Tokenizer::skipAnnotation() {
	int stack = 0;
	bool annotationWithParam = false;
	this->tokenItr++;
	while(true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0) {
				skipComments(this->tokenItr->str(1));
				continue;
			}
			else if (this->tokenItr->str(0).compare("(") == 0) {
				annotationWithParam = true;
				stack += 1;
			}
			else if (this->tokenItr->str(0).compare(")") == 0)
				stack -= 1;
			if (stack == 0) break;
			this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("Reached end of file while parsing annotation block");
	}
	if (annotationWithParam) this->tokenItr++;
	return annotationWithParam;
}

std::string Tokenizer::concateTypes() {
	int stack = 1;
	std::string typeStr;
	typeStr.append("<");
	this->tokenItr++;
	while (stack > 0) {
		if (this->tokenItr != this->endTokenItr) {
			int cmp1 = this->tokenItr->str(1).compare("");
			int cmp2 = this->tokenItr->str(2).compare("");
			int cmp3 = this->tokenItr->str(3).compare("");

			if (cmp1 != 0 || cmp2 != 0 || cmp3 != 0) throw yy::parser::syntax_error("");
			else if (this->tokenItr->str(0).compare("<") == 0) stack += 1;
			else if (this->tokenItr->str(0).compare(">") == 0) stack -= 1;

			typeStr.append(this->tokenItr->str(0));
			this->tokenItr++;
		}
		else if (!getNewLineM()) {
			throw yy::parser::syntax_error("Invaild token");
		}
	}
	return typeStr;
}

void Tokenizer::appendTypeArray() {
	const std::string& token = this->tokenItr->str(0);
	int tf = this->token_buffer.size() - 1;
	if (token.compare("[") == 0) {
		this->token_buffer[this->token_buffer.size() - 1].append("[]");
	}
	this->tokenItr++;
}

void Tokenizer::skipToToken(const char* token) {
	while (true) {
		if(this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0)
				skipComments(this->tokenItr->str(1));
			else if (this->tokenItr->str(0).compare(token) == 0)
				break;
			else this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("Reached end of file while parsing inner class");
	}
}

bool Tokenizer::addTokenBuffer() {
	if (this->data.length() == 0 && !getNewLineM())
		return false;

	while (true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0)
				skipComments(this->tokenItr->str(0));
			else if (this->tokenItr->str(0).compare("<") == 0) {
				std::string type = concateTypes();
				this->token_buffer[this->token_buffer.size() - 1].append(type);
				type.clear();
			}
			else if (this->tokenItr->str(0).compare("[") == 0 ||
					this->tokenItr->str(0).compare("]") == 0) {
				appendTypeArray();
			}
			else if (this->tokenItr->str(0).compare("class") == 0 && this->innerClass) {
				this->ignoreGrammar.resetEverything();
				this->token_buffer.clear();
				skipToToken("{");
				skipBlock();
			}
			else {
				this->token_buffer.push_back(this->tokenItr->str(0));
				int doIgnore = this->ignoreGrammar.checkGrammar(this->tokenItr->str(0), this->token_buffer.size());
				if (doIgnore == -1) break;
				else if (doIgnore == 1) {
					this->token_buffer.clear();
					if (this->ignoreGrammar.isStatic() || this->ignoreGrammar.isInterface()) skipBlock();
					else if (this->ignoreGrammar.isAssignment()) skipAssignment();
					else if (this->ignoreGrammar.isAnnotation()) skipAnnotation();
					else if (this->ignoreGrammar.isIIB()) skipBlock();
					else if (this->ignoreGrammar.isSemicolon()) this->tokenItr++;
				}
				else this->tokenItr++;
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
	else if (token.compare("final") == 0) tokenNo = yy::parser::token::yytokentype::FINAL;
	else if (token.compare("(") == 0) tokenNo = yy::parser::token::yytokentype::O_PARAM;
	else if (token.compare(")") == 0) tokenNo = yy::parser::token::yytokentype::C_PARAM;
	else if (token.compare("class") == 0) {
		tokenNo = yy::parser::token::yytokentype::T_CLASS;
		this->innerClass = true;
	}
	else {
		tokenNo = yy::parser::token::yytokentype::STRINGS;
		val->str = new std::string(token);
	}
	return tokenNo;
}

uint64_t* Tokenizer::get_yylex_body() {
	uint64_t* hash = new uint64_t[2];
	std::string body;
	int braceCount = 1;
	body.reserve(3072);
	hash[0] = 0;
	hash[1] = 0;
	this->doBodyRegex = false;
	// check abstract | opening block(brace)
	if (!checkBlockPresent()) {
		return hash;
	}

	if (this->tokenItr != this->endTokenItr) {
		this->data.erase(0, this->tokenItr->position());
		this->tokenItr = std::sregex_iterator(this->data.begin(), this->data.end(), this->bodyContent);
	}
	else if (!getNewLineB()) throw yy::parser::syntax_error("End of file reached");

	while (true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0)
				skipComments(this->tokenItr->str(1), true);
			else {
				body.append(this->tokenItr->str(0));
				if (this->tokenItr->str(0).compare("{") == 0) braceCount += 1;
				else if (this->tokenItr->str(0).compare("}") == 0) braceCount -= 1;
				this->tokenItr++;
			}
		}
		else if(!getNewLineB()) throw yy::parser::syntax_error("End of file reached");
		if (braceCount == 0) break;
	}
	MurmurHash3_x64_128(body.c_str(), body.length(), 1, hash);
	// convert to mainContentRegex
	if (this->tokenItr != this->endTokenItr) {
		this->data.erase(0, this->tokenItr->position());
		this->tokenItr = std::sregex_iterator(this->data.begin(), this->data.end(), this->mainContent);
	}
	else this->data.clear();
	return hash;
}

bool Tokenizer::checkBlockPresent() {
	bool returnVal = false;
	while (true) {
		if (this->tokenItr != this->endTokenItr) {
			if (this->tokenItr->str(1).compare("") != 0) {
				skipComments(this->tokenItr->str(0));
				continue;
			}
			else if (this->tokenItr->str(0).compare("{") == 0) {
				returnVal = true;
				break;
			}
			else if (this->tokenItr->str(0).compare(";") == 0)
				break;
			this->tokenItr++;
		}
		else if (!getNewLineM())
			throw yy::parser::syntax_error("End of file reached");
	}
	this->tokenItr++;
	return returnVal;
}

TOKENS Tokenizer::yylex(yy::parser::semantic_type* val) {
	if (this->doBodyRegex) {
		this->token_index = this->token_buffer.size();
		val->bodyHash = get_yylex_body();
		return yy::parser::token::yytokentype::BODY;
	}
	if (this->token_index == this->token_buffer.size()) {
		this->token_index = 0;
		this->token_buffer.clear();
		if (!addTokenBuffer()) return yy::parser::token::yytokentype::ERROR;
	}
	return get_yylex_token(val, this->token_buffer[this->token_index++]);
}

void Tokenizer::changeBodyRegex() {
	this->doBodyRegex = true;
}

void Tokenizer::closeFile() {
	try { this->file.close(); }
	catch (...) {}
}

int Tokenizer::lineNo() {
	return this->lineno;
}

bool Tokenizer::classStatus() {
	return this->innerClass;
}

Tokenizer::~Tokenizer() {
	try { this->file.close(); }
	catch (...) {}
	this->token_buffer.clear();
}