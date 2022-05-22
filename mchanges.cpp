#include <iostream>
#include "java/grammar.h"
#include "java/tokenizer.h"
#include "java/node.h"
#include "java/javafiles.h"
#include "arguments.h"

extern JavaLang* jLang;
class WrrapperParser : public yy::parser {
	Tokenizer tokens;
public:
	void error(const std::string& err) {
		if (!tokens.classStatus())
			std::cout << "No class definition found. Note: interface will be ignored\n";
		else
			std::cerr << err << " : line no : " << this->tokens.lineNo() << std::endl;
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

bool fill_exp(std::string& list, std::set<std::string>& expfile) {
	std::ifstream file;
	std::string tmp;
	file.open(list);
	if (!file.good()) return false;
	while (std::getline(file, tmp))
		expfile.insert(tmp);
	return true;
}

void printCSV(JavaLang* lang1, JavaLang* lang2, std::ofstream& csv) {
	std::string content;
	std::string className = lang1->getClassHeader()->getClassName();
	const std::set<JavaFunction>* functions1 = lang1->getFunctionList()->getFunctionList();
	const std::set<JavaFunction>* functions2 = lang2->getFunctionList()->getFunctionList();
	auto endFunc = functions1->end();
	for (const JavaFunction& jf2 : *functions2) {
		auto jf1 = functions1->find(jf2);
		if ((jf1 != endFunc) && ((*jf1).cmpBodyHash(jf2) != 0)) {
			content.append("\"class    ");
			content.append(className);
			content.append("\",\"");
			content.append(jf2.getReturnType());
			content.append("    ");
			content.append(jf2.getName());
			content.append(" (");

			bool flag = false;
			const std::vector<std::string>& param = jf2.getParamList();
			for (std::string tmp : param) {
				flag = true;
				content.append(tmp);
				content.append(", ");
			}
			if(flag) content[content.length() - 1] = ')';
			content.append("\"\n");
			csv << content;
			content.erase();
		}
	}
}

int main(int argc, char** argv) {
	std::ofstream csv;
	if (argc <= 1) {
		std::cout << "Invalid argument. Check --help for more info.\n";
		return 1;
	}
	csv.open("m.out.csv", std::ios_base::out | std::ios_base::trunc);
	if (!csv.is_open()) {
		std::cerr << "stream error while writing to m.out.csv\n";
		return 1;
	}
	ArgumentParser argParse;
	int retVal = argParse.parseAg(argc, argv);
	if (retVal == 2) return 0;
	else if (retVal == 1) {
		std::cout << "Invalid argument. Check --help for more info.\n";
		return 1;
	}

	std::string dir1 = argParse.get_directory1();
	std::string dir2 = argParse.get_directory2();
	std::string clist = argParse.get_clist();
	std::string expfile = argParse.get_explist();

	if (dir1.empty() || dir2.empty()) {
		std::cout << "Invalid argument. --dir1, --dir2 are missing\n    Check --help for more info.\n";
		return 1;
	}

	// make generic path
	for (int i = 0; i < dir1.length(); i++) if (dir1[i] == '\\') dir1[i] = '/';
	for (int i = 0; i < dir2.length(); i++) if (dir2[i] == '\\') dir2[i] = '/';

	if (dir1[dir1.length() - 1] != '/') dir1.append("/");
	if (dir2[dir2.length() - 1] != '/') dir2.append("/");

	// get exception lists
	std::set<std::string> explist;
	if (!expfile.empty() && !fill_exp(expfile, explist)) return 1;

	java_files files(dir2);
	if (!clist.empty()) files.setCustomList(clist);

	WrrapperParser parser;
	std::string dir1_file;
	dir1_file.reserve(1024);
	for (const std::string& dir2_file : files) {
		JavaLang* lang1 = nullptr;
		JavaLang* lang2 = nullptr;
		dir1_file.assign(dir1);
		dir1_file.append(dir2_file.substr(dir2.length()));
		if (stdfs::exists(dir1_file)) {
			try {
				parser.setFile(dir2_file.c_str());
				parser.parse();
				lang1 = jLang;
				if (jLang == nullptr) {
					std::cout << "skipping file : " << dir2_file << std::endl;
					continue;
				}
				jLang = nullptr;
				parser.setFile(dir1_file.c_str());
				parser.parse();
				lang2 = jLang;
				if (jLang == nullptr) {
					std::cout << "skipping file : " << dir1_file << std::endl;
					delete lang1;
					continue;
				}
				printCSV(lang1, lang2, csv);
				jLang = nullptr;
			}
			catch (std::ifstream::failure& e) {
				std::cout << "caught stream exception\n";
				std::cout << "  what() : " << e.what() << std::endl;
			}
			catch (InvalidException& e) {
				std::cerr << "InvaildException : " << e.what() << std::endl;
			}
			catch (...) {
				std::cerr << "Something went wrong while parsing file/writting to a csv file.\n";
			}
			if (lang1 != nullptr) delete lang1;
			if (lang2 != nullptr) delete lang2;
		}
		else {
			std::cout << dir1_file << " not found\n";
		}
		std::cout << dir2_file << " - completed\n";
		dir1_file.erase();
	}
	csv.close();
	std::cout << "\nEND\n";
	return 0;
}

