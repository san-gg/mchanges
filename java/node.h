#pragma once
#ifndef __NODE_
#define __NODE_

#include <string>

class JavaClassHeader;
class JavaModifiers;
class JavaScope;

class JavaLang {
	JavaClassHeader* header;
public:
	JavaLang(JavaClassHeader*);
	JavaClassHeader* getClassHeader();
	~JavaLang();
};

class JavaClassHeader {
	std::string className;
	JavaModifiers* mod;
public:
	JavaClassHeader(std::string*);
	std::string getClassName();
	void setModifier(JavaModifiers*);
	~JavaClassHeader();
};

class JavaModifiers {
	JavaScope* jscope;
	bool _static;
	bool synchronized;
	bool native;
	bool strictfp;
	bool _abstract;
public:
	JavaModifiers();
	void setStatic();
	void setSynchronized();
	void setNative();
	void setStrictfp();
	void setAbstract();
	void setScope(JavaScope*);

	std::string* getScope();
	bool isStatic();
	bool isSynchronized();
	bool isNative();
	bool isStrictfp();
	bool isAbstract();
	~JavaModifiers();
};

class JavaScope {
	std::string identifier;
public:
	JavaScope(const char*);
	std::string* getIdentifier();
	~JavaScope();
};

#endif // !__NODE_