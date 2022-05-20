#pragma once
#ifndef __NODE_
#define __NODE_

#include <string>
#include <set>
#include <vector>



class JavaScope {
public:
	enum class Scope {
		PUBLIC = 0,
		PRIVATE = 1,
		PROTECTED = 2,
		DEFAULT = 3
	}scope;
	JavaScope(JavaScope::Scope = JavaScope::Scope::DEFAULT);
};

class JavaModifiers {
	JavaScope jscope;
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
	void operator=(JavaModifiers&);

	JavaScope::Scope getScope();
	bool isStatic();
	bool isSynchronized();
	bool isNative();
	bool isStrictfp();
	bool isAbstract();
};

class JavaClassHeader {
	std::string className;
	JavaModifiers* mod;
public:
	JavaClassHeader(std::string*);
	std::string getClassName();
	void setModifier(JavaModifiers*);
	JavaModifiers* getModifiers();
	~JavaClassHeader();
};

class Parameters {
	std::vector<std::string> paramList;
public:
	void addParameter(std::string*);
	std::vector<std::string> getParameter();
	~Parameters();
};

class JavaFunction {
	JavaModifiers jMod;
	uint64_t bodyHash[2];
	std::string name;
	std::string returnType;
	std::vector<std::string> paramList;
public:
	JavaFunction(std::string*, std::string*);
	JavaFunction(std::string*);
	void setJavaModifier(JavaModifiers*);
	void setParameters(Parameters*);
	void setBodyHash(uint64_t*);
	bool operator < (const JavaFunction&) const;
	int cmpBodyHash(const JavaFunction&) const;
	std::string getReturnType() const;
	std::string getName() const;
	std::vector<std::string> getParamList() const;
	~JavaFunction();
};

class FunctionList {
	std::set<JavaFunction> list;
public:
	void addFunction(JavaFunction*);
	const std::set<JavaFunction>* getFunctionList();
	~FunctionList();
};

class JavaLang {
	JavaClassHeader* header;
	FunctionList* functionList;
public:
	JavaLang(JavaClassHeader*, FunctionList*);
	JavaClassHeader* getClassHeader();
	FunctionList* getFunctionList();
	~JavaLang();
};

#endif // !__NODE_