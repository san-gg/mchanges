#pragma once
#ifndef __ARGUMENTS_
#define __ARGUMENTS_

class ArgumentParser {
	std::string directory1;
	std::string directory2;
	std::string clist;
	std::string exp;
	
	int strcompare(const char *, const char *);
	int argvalue(const char *, const char *, const char *);
	int argvaluehelp(const char*);
	void printHelp();
public:
	std::string get_directory1();
	std::string get_directory2();
	int parseAg(int, char **);
};

#endif // !__ARGUMENTS_

