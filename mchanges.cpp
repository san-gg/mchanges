// change return to ERROR token
#include "java/grammar.h"
#include "java/tokenizer.h"
#include "java/node.h"
extern JavaLang* jLang;
class WrrapperParser : public yy::parser {
	Tokenizer tokens;
public:
	void error(const std::string& err) {
		std::cout << "Caught Error while parsing...\n";
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
};

int main() {
	WrrapperParser parser;
	try {
		parser.setFile("java/testJava/p1.java");
		parser.parse();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "caught stream exception\n";
		std::cout << "  what() : " << e.what() << std::endl;
		exit(1);
	}
	catch (yy::parser::syntax_error& e) {
		std::cout << "caught syntax_error\n";
		std::cout << "  what() : " << e.what() << std::endl;
	}
	catch (std::regex_error& e) {
		std::cerr << "Regex Error : " << e.what() << std::endl;
	}
	catch (InvalidException& e) {
		std::cerr << "InvaildException : " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Something went wrong\n";
	}
	if (jLang != nullptr) {
		char temp;
		std::cout << "Finally you did it press enter for success...";
		std::cin >> temp;
		std::cout << "Class Name : ";
		if (jLang->getClassHeader()->getModifiers() != nullptr) {
			std::cout << *(jLang->getClassHeader()->getModifiers()->getScope()) << " ";
		}
		std::cout << jLang->getClassHeader()->getClassName() << std::endl;
		delete jLang;
	}
	parser.closeFile();
	return 0;
}