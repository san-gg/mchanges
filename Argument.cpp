#include <iostream>
#include "arguments.h"

std::string ArgumentParser::get_directory1() { return this->directory1; }
std::string ArgumentParser::get_directory2() { return this->directory2; }
std::string ArgumentParser::get_clist() { return this->clist; }
std::string ArgumentParser::get_explist() { return this->exp; }

int ArgumentParser::strcompare(const char * s1, const char * s2) {
	int x = 0;
	int y = 0;
	int cmp = 0;
	while (true) {
		if ((s1[x] == '\0') && (s2[y] == '\0')) break;
		if ((s1[x] == '\0') || (s2[y] == '\0') || (s1[x] != s2[y])) {
			cmp = 1;
			break;
		}
		x++;
		y++;
	}
	return cmp;
}


int ArgumentParser::argvalue(const char * actual, const char * expected, const char * value) {
	if (value == 0) return 0;
	else if (value[0] == '-') return 0;
	if (strcompare(actual, expected) == 0) return 1;
	
	return 0;
}
int ArgumentParser::argvaluehelp(const char* value) {
	if (strcompare(value, "--help") == 0)
		return 1;
	return 0;
}

int ArgumentParser::parseAg(int argc, char ** argv) {
	int _argno = 1, time = -1, granularity = -1;
	char * arg = 0;
	int returnVal = 0, isArgCorrect = 0;

	while (_argno < argc) {
		arg = argv[_argno];
		if (argvalue(arg, "--dir1", argv[_argno + 1]))
			this->directory1.assign(argv[_argno + 1]);
		else if (argvalue(arg, "--dir2", argv[_argno + 1]))
			this->directory2.assign(argv[_argno + 1]);
		else if (argvalue(arg, "--clist", argv[_argno + 1]))
			this->clist.assign(argv[_argno + 1]);
		else if (argvalue(arg, "--exp", argv[_argno + 1]))
			this->exp.assign(argv[_argno + 1]);
		else if (argvaluehelp(arg)) {
			printHelp();
			returnVal = 2;
			break;
		}
		else {
			returnVal = 1;
			break;
		}
		_argno += 2;
	}
	return returnVal;
}

void ArgumentParser::printHelp() {
	std::cout << "\nmchanges.exe [--dir1 directory] [--dir2 directory] [--clist list | --exp except]\n\n";
	std::cout << "Description:\n";
	std::cout << "    Tool that finds no. of java methods changed between java files. Generates csv report\n";
	std::cout << "    for all the java files present in --dir1 and --dir2 directories.\n";
	std::cout << "    Addition parameter:\n";
	std::cout << "    --clist list of selective java files to be considered. Path mentioned in\n";
	std::cout << "       file.txt needs to be relative to the mentioned directory in --dir1 and --dir2.\n";
	std::cout << "    --exp java paths that will be ignored.\n\n";
	std::cout << "Parameter List:\n";
	std::cout << "    --dir1    directory     Directory path\n";
	std::cout << "    --dir2    directory     Directory path. Java files will be compared with --dir1\n";
	std::cout << "    --clist   file.txt      List of selective java files.\n";
	std::cout << "    --exp     file.txt      Paths that will be ignored.\n";
	return;
}