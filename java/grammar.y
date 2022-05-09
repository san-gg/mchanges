%{
#include "node.h"
JavaLang *jLang = nullptr;
%}

%union {
	JavaClassHeader *classHeader;
	std::string *str;
	JavaScope *scope;
	JavaModifiers *modifiers;
}

%token O_BRACE C_BRACE T_CLASS ERROR
%token STATIC ABSTRACT STRICTFP SYNCHRONIZED NATIVE
%token PUBLIC PRIVATE PROTECTED DEFAULT
%token COMMA
%token <str> STRINGS 

%type <classHeader> CLASS_HEADER CLASS_NAME
%type <scope> SCOPE
%type <modifiers> MODIFIERS


%start JAVA

%%

JAVA : CLASS_HEADER O_BRACE C_BRACE	{ jLang = new JavaLang($1); YYACCEPT; }
		;


CLASS_HEADER : MODIFIERS CLASS_NAME		{ $$ = $2; $$->setModifier($1); }
			| CLASS_NAME				{ $$ = $1; }
			| CLASS_HEADER STRINGS		{ $$ = $1; delete $2; }
			| CLASS_HEADER COMMA		{ $$ = $1; }
			;


CLASS_NAME : T_CLASS STRINGS	{ $$ = new JavaClassHeader($2); }
			;


MODIFIERS : MODIFIERS STATIC			{ $1->setStatic(); $$ = $1; }
			| MODIFIERS ABSTRACT		{ $1->setAbstract(); $$ = $1; }
			| MODIFIERS STRICTFP		{ $1->setStrictfp(); $$ = $1; }
			| MODIFIERS SYNCHRONIZED	{ $1->setSynchronized(); $$ = $1;}
			| MODIFIERS NATIVE			{ $1->setNative(); $$ = $1; }
			| SCOPE						{ $$ = new JavaModifiers(); $$->setScope($1); }
			| STATIC					{ $$ = new JavaModifiers(); $$->setStatic(); }
			| ABSTRACT					{ $$ = new JavaModifiers(); $$->setAbstract(); }
			| STRICTFP					{ $$ = new JavaModifiers(); $$->setStrictfp(); }
			| SYNCHRONIZED				{ $$ = new JavaModifiers(); $$->setSynchronized(); }
			| NATIVE					{ $$ = new JavaModifiers(); $$->setNative(); }
			;

SCOPE : PUBLIC			{ $$ = new JavaScope("public"); }
		| PRIVATE		{ $$ = new JavaScope("private"); }
		| PROTECTED		{ $$ = new JavaScope("protected"); }
		| DEFAULT		{ $$ = new JavaScope("default"); }
		;


%%