#include "node.h"
#include <iostream>

//~~~~~~~~~~~~ Class JavaLang ~~~~~~~~~~~~
//
JavaLang::JavaLang(JavaClassHeader* p1, FunctionList* p2) : header(p1), functionList(p2) { }
JavaClassHeader* JavaLang::getClassHeader() {
	return this->header;
}
FunctionList* JavaLang::getFunctionList() {
	return this->functionList;
}
JavaLang::~JavaLang() {
	if (this->header != nullptr) delete this->header;
	if (this->functionList != nullptr) delete this->functionList;
}

//~~~~~~~~~~~~ Class JavaClassHeader ~~~~~~~~~~~~
//
JavaClassHeader::JavaClassHeader(std::string* p1) : className(*p1), mod(nullptr) {
	delete p1;
}
std::string JavaClassHeader::getClassName() {
	return this->className;
}
void JavaClassHeader::setModifier(JavaModifiers* mod) { this->mod = mod; }
JavaModifiers* JavaClassHeader::getModifiers() { return this->mod; }
JavaClassHeader::~JavaClassHeader() {
	this->className.clear();
	if (this->mod != nullptr) delete this->mod;
}

//~~~~~~~~~~~~ Class FunctionList ~~~~~~~~~~~~
//
void FunctionList::addFunction(JavaFunction* jFun) {
	this->list.insert(*jFun);
}
std::set<JavaFunction> FunctionList::getFunctionList() {
	return this->list;
}
FunctionList::~FunctionList() {
	this->list.clear();
}

//~~~~~~~~~~~~ Class JavaFunction ~~~~~~~~~~~~
//
JavaFunction::JavaFunction(std::string* p1, std::string* p2) : returnType(*p1), name(*p2),
															 bodyHash{0,0} { }
bool JavaFunction::operator<(const JavaFunction& jFunc) const {
	size_t count1 = this->paramList.size();
	size_t count2 = jFunc.paramList.size();
	if (count1 < count2) return true;
	else if (count1 > count2) return false;
	else if (count1 == count2) {
		int cmp = this->name.compare(jFunc.name);
		if (cmp == -1) return true;
		else if (cmp > 0) return false;
		else if (cmp == 0) {
			int indx = 0;
			for (const std::string& x : this->paramList) {
				cmp = x.compare(jFunc.paramList[indx]);
				if (cmp < 0) return true;
				else if (cmp == 1) return false;
				indx += 1;
			}
		}
		return false;
	}
	//throw InvalidException();
}
void JavaFunction::setJavaModifier(JavaModifiers* jMod) {
	this->jMod = *jMod;
}
void JavaFunction::setParameters(Parameters* param) {
	const std::vector<std::string>& val = param->getParameter();
	this->paramList.assign(val.begin(), val.end());
}
void JavaFunction::setBodyHash(uint64_t* body) {
	this->bodyHash[0] = body[0];
	this->bodyHash[1] = body[1];
}
JavaFunction::~JavaFunction() {
	this->name.clear();
	this->returnType.clear();
	this->paramList.clear();
}
void JavaFunction::print() const {
	std::cout << this->returnType << " " << this->name << '(';
	for (std::string str : this->paramList) {
		std::cout << str << ", ";
	}
	std::cout << ") " << this->bodyHash[0] << this->bodyHash[1] << std::endl;
}


//~~~~~~~~~~~~ Class Parameters ~~~~~~~~~~~~
//
void Parameters::addParameter(std::string* param) {
	this->paramList.push_back(*param);
}
std::vector<std::string> Parameters::getParameter() {
	return this->paramList;
}
Parameters::~Parameters() {
	this->paramList.clear();
}


//~~~~~~~~~~~~ Class JavaModifiers ~~~~~~~~~~~~
//
JavaModifiers::JavaModifiers() {
	this->jscope = nullptr;
	this->_static = false;
	this->synchronized = false;
	this->native = false;
	this->strictfp = false;
	this->_abstract = false;
}
void JavaModifiers::setStatic() { this->_static = true; }
void JavaModifiers::setSynchronized() { this->synchronized = true; }
void JavaModifiers::setNative() { this->native = true; }
void JavaModifiers::setStrictfp() { this->strictfp = true; }
void JavaModifiers::setAbstract() { this->_abstract = true; }
void JavaModifiers::setScope(JavaScope* scope) { this->jscope = scope; }
JavaScope::Scope JavaModifiers::getScope() {
	return (this->jscope != nullptr) ? this->jscope->getScope() : JavaScope::Scope::DEFAULT;
}
bool JavaModifiers::isStatic() { return this->_static; }
bool JavaModifiers::isSynchronized() { return this->synchronized; }
bool JavaModifiers::isNative() { return this->native; }
bool JavaModifiers::isStrictfp() { return this->strictfp; }
bool JavaModifiers::isAbstract() { return this->_abstract; }
void JavaModifiers::operator=(JavaModifiers &mod) {
	JavaScope::Scope scope = mod.getScope();
	this->jscope = new JavaScope(scope);
	this->_abstract = mod._abstract;
	this->_static = mod._static;
	this->native = mod.native;
	this->strictfp = mod.strictfp;
	this->synchronized = mod.synchronized;
}
JavaModifiers::~JavaModifiers() {
	if (jscope != nullptr)
		delete jscope;
}


//~~~~~~~~~~~~ Class JavaScope ~~~~~~~~~~~~
//
JavaScope::JavaScope(JavaScope::Scope p1) : scope(p1) { };
JavaScope::Scope JavaScope::getScope() const {
	return this->scope;
}
