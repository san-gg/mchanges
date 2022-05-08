#include "node.h"

JavaLang::JavaLang(JavaClassHeader* p1) : header(p1) { }
JavaClassHeader* JavaLang::getClassHeader() {
	return this->header;
}
JavaLang::~JavaLang() {
	if (this->header != nullptr) delete this->header;
}


JavaClassHeader::JavaClassHeader(std::string* p1) : className(*p1), mod(nullptr) {
	delete p1;
}
std::string JavaClassHeader::getClassName() {
	return this->className;
}
void JavaClassHeader::setModifier(JavaModifiers* mod) { this->mod = mod; }
JavaClassHeader::~JavaClassHeader() {
	this->className.clear();
	if (this->mod != nullptr) delete this->mod;
}

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
std::string* JavaModifiers::getScope() { return this->jscope->getIdentifier(); }
bool JavaModifiers::isStatic() { return this->_static; }
bool JavaModifiers::isSynchronized() { return this->synchronized; }
bool JavaModifiers::isNative() { return this->native; }
bool JavaModifiers::isStrictfp() { return this->strictfp; }
bool JavaModifiers::isAbstract() { return this->_abstract; }
JavaModifiers::~JavaModifiers() {
	if (jscope != nullptr) delete jscope;
}



JavaScope::JavaScope(const char* p1) : identifier(p1) { }
std::string* JavaScope::getIdentifier() {
	return &this->identifier;
}
JavaScope::~JavaScope() {
		this->identifier.clear();
	}
