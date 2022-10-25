#include <iostream>
#include <signal.h>
#include "java/grammar.h"
#include "java/node.h"
#include "java/javafiles.h"
#include "java/java_parser.h"
#include "arguments.h"

extern JavaLang* jLang;
const char* curr_file = nullptr;

void SignalHandler(int signal) {
	if (signal == SIGSEGV) {
		std::cout << "Invalid memory access... potentially bug in the code.\n";
		if(::curr_file != nullptr) std::cout << "File : " << ::curr_file << std::endl << std::endl;
	}
	else if (signal == SIGTERM) {
		std::cout << "Someone Terminated this program...\n";
		std::cout << "File : " << ::curr_file << std::endl;
		std::cout << "Terminated Successfully...\n";
	}
	else if (signal == SIGINT) {
		std::cout << "Okay... Exiting\n";
	}
	else if (signal == SIGABRT) { 
		std::cout << "Unexpected program got aborted\n";
	}
	else {
		std::cout << "Unexpected Terminated.";
	}
	exit(1);
}

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
	size_t noOfFiles = 0;
	size_t noOfErrors = 0;
	size_t noOfFileNotFound = 0;
	signal(SIGTERM, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGABRT, SignalHandler);
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

	std::cout << "Initializing...\n";

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

	JavaParser parser;
	std::string dir1_file;
	dir1_file.reserve(1024);
	std::cout << "Parsing started...\n";
	for (const std::string& dir2_file : files) {
		JavaLang* lang1 = nullptr;
		JavaLang* lang2 = nullptr;
		dir1_file.assign(dir1);
		dir1_file.append(dir2_file.substr(dir2.length()));
		if (stdfs::exists(dir1_file)) {
			noOfFiles += 1;
			try {
				::curr_file = dir2_file.c_str();
				parser.setFile(dir2_file.c_str());
				parser.parse();
				lang1 = jLang;
				if (jLang == nullptr) {
					noOfErrors += 1;
					std::cout << "skipping file : " << dir2_file << std::endl << std::endl;
					continue;
				}
				jLang = nullptr;
				parser.setFile(dir1_file.c_str());
				parser.parse();
				lang2 = jLang;
				if (jLang == nullptr) {
					noOfErrors += 1;
					std::cout << "skipping file : " << dir1_file << std::endl << std::endl;
					delete lang1;
					continue;
				}
				printCSV(lang1, lang2, csv);
				
				std::cout << dir2_file << " - completed\n";
			}
			catch (ParserException& e) {
				const char* str = e.what();
				std::string tmp(70, '-');
				std::cerr << tmp << std::endl;
				std::cerr << "Error   : " << str << std::endl;
				std::cerr << "File    : " << ::curr_file << std::endl;
				std::cerr << "line no : " << Tokenizer::getLineNo() << std::endl;
				std::cerr << tmp << std::endl;
				noOfErrors += 1;
			}
			catch (std::invalid_argument& e) {
				std::cerr << "----------------------------------------------------------------------\n";
				std::cerr << "Something went wrong while parsing.\nFile : " << ::curr_file << std::endl;
				std::cerr << e.what() << std::endl;
				std::cerr << "----------------------------------------------------------------------\n\n";
				noOfErrors += 1;
			}
			catch (boost::regex_error e) {
				std::cerr << "----------------------------------------------------------------------\n";
				std::cerr << "Skipping File : " << ::curr_file;
				std::cerr << "\n\t" << e.what();
				std::cerr << "\n----------------------------------------------------------------------\n\n";
				noOfErrors += 1;
			}
			catch (...) {
				std::cerr << "Something went wrong...\n~~Exiting~~\n";
				exit(2);
			}
		}
		else {
			noOfFileNotFound += 1;
			std::cout << dir1_file << " - not found\n";
		}
		jLang = nullptr;
		if (lang1 != nullptr) delete lang1;
		if (lang2 != nullptr) delete lang2;
		dir1_file.erase();
	}
	csv.close();
	std::cout << "\n\t[COMPLETED]\n\t\tTotal files parsed : " << noOfFiles;
	std::cout << "\n\t\tSkipped files      : " << noOfErrors;
	std::cout << "\n\t\tFiles Not Found    : " << noOfFileNotFound << std::endl;
	return 0;
}

