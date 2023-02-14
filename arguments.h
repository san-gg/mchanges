#pragma once
#ifndef __ARGUMENTS_
#define __ARGUMENTS_

/*
	Very specific for Java Lang.
*/
class ArgumentParser {
	std::string directory1;
	std::string directory2;
	std::string clist;
	std::string exp;
	// 1 - do Method Change
	// 2 - check new Method
	int options;
	
	int strcompare(const char *, const char *);
	int argvalue(const char *, const char *, const char *);
	int argvaluehelp(const char*);
	int argoptions(const char*);
	void printHelp();
public:
	ArgumentParser();
	std::string get_directory1();
	std::string get_directory2();
	std::string get_clist();
	std::string get_explist();
	int get_options();
	int parseAg(int, char **);
};

#endif // !__ARGUMENTS_

