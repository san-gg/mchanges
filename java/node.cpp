#include "node.h"
#include <iostream>

//~~~~~~~~~~~~ Class JavaLang ~~~~~~~~~~~~
//
JavaLang::JavaLang(JavaClassHeader* p1, FunctionList* p2) : header(p1), functionList(p2) { }
JavaLang::JavaLang(JavaClassHeader* p1) : header(p1) { this->functionList = new FunctionList(); }
JavaClassHeader* JavaLang::getClassHeader() {
	return this->header;
}
FunctionList* JavaLang::getFunctionList() {
	return this->functionList;
}
JavaLang::~JavaLang() {
	if (this->header != nullptr) delete this->header;
	if (this->functionList != nullptr) delete this->functionList;
	this->header = nullptr;
	this->functionList = nullptr;
}

//~~~~~~~~~~~~ Class JavaClassHeader ~~~~~~~~~~~~
//
JavaClassHeader::JavaClassHeader(std::string* p1) : className(*p1), mod(nullptr), genericClass(false) { }
std::string JavaClassHeader::getClassName() {
	return this->className;
}
void JavaClassHeader::setModifier(JavaModifiers* mod) { this->mod = mod; }
void JavaClassHeader::setGenericClass() { this->genericClass = true; }
JavaModifiers* JavaClassHeader::getModifiers() { return this->mod; }
bool JavaClassHeader::isGenericClass() { return this->genericClass; }
JavaClassHeader::~JavaClassHeader() {
	this->className.clear();
	if (this->mod != nullptr) delete this->mod;
	this->mod = nullptr;
}

//~~~~~~~~~~~~ Class FunctionList ~~~~~~~~~~~~
//
void FunctionList::addFunction(JavaFunction* jFun) {
	this->list.insert(*jFun);
}
const std::set<JavaFunction>* FunctionList::getFunctionList() {
	return &this->list;
}
FunctionList::~FunctionList() {
	this->list.clear();
}

//~~~~~~~~~~~~ Class JavaFunction ~~~~~~~~~~~~
//
JavaFunction::JavaFunction(std::string* p1, std::string* p2) : returnType(*p1), name(*p2),
													bodyHash{0,0}, genericFunction(false) { }
JavaFunction::JavaFunction(std::string* p1) : name(*p1), bodyHash{0,0}, genericFunction(false) { }
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
}
void JavaFunction::setJavaModifier(JavaModifiers* jMod) {
	this->jMod = *jMod;
}
void JavaFunction::setParameters(Parameters* param) {
	const std::vector<std::string>& val = param->getParameter();
	this->paramList.assign(val.begin(), val.end());
}
void JavaFunction::setGenericFunction() {
	this->genericFunction = true;
}
void JavaFunction::setBodyHash(uint64_t* body) {
	this->bodyHash[0] = body[0];
	this->bodyHash[1] = body[1];
}
int JavaFunction::cmpBodyHash(const JavaFunction& jFunction) const {
	if (this->bodyHash[0] != jFunction.bodyHash[0]) return -1;
	if (this->bodyHash[1] != jFunction.bodyHash[1]) return 1;
	return 0;
}
std::string JavaFunction::getReturnType() const { return this->returnType; }
std::string JavaFunction::getName() const { return this->name; }
std::vector<std::string> JavaFunction::getParamList() const { return this->paramList; }
bool JavaFunction::isGenericFunction() { return this->genericFunction; }
JavaFunction::~JavaFunction() {
	this->name.clear();
	this->returnType.clear();
	this->paramList.clear();
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
	this->jscope.scope = JavaScope::Scope::DEFAULT;
	this->_static = false;
	this->_synchronized = false;
	this->_native = false;
	this->_strictfp = false;
	this->_abstract = false;
	this->_final = false;
}
void JavaModifiers::setStatic() { this->_static = true; }
void JavaModifiers::setSynchronized() { this->_synchronized = true; }
void JavaModifiers::setNative() { this->_native = true; }
void JavaModifiers::setStrictfp() { this->_strictfp = true; }
void JavaModifiers::setAbstract() { this->_abstract = true; }
void JavaModifiers::setFinal() { this->_final = true; }
void JavaModifiers::setScope(JavaScope* scope) { this->jscope = scope->scope; }
JavaScope::Scope JavaModifiers::getScope() { return this->jscope.scope; }
bool JavaModifiers::isStatic() { return this->_static; }
bool JavaModifiers::isSynchronized() { return this->_synchronized; }
bool JavaModifiers::isNative() { return this->_native; }
bool JavaModifiers::isStrictfp() { return this->_strictfp; }
bool JavaModifiers::isAbstract() { return this->_abstract; }
bool JavaModifiers::isFinal() { return this->_final; }
void JavaModifiers::operator=(JavaModifiers &mod) {
	this->jscope.scope = mod.getScope();
	this->_abstract = mod._abstract;
	this->_static = mod._static;
	this->_native = mod._native;
	this->_strictfp = mod._strictfp;
	this->_synchronized = mod._synchronized;
}


//~~~~~~~~~~~~ Class JavaScope ~~~~~~~~~~~~
//
JavaScope::JavaScope(JavaScope::Scope p1) : scope(p1) { };
