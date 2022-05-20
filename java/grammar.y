%{
#include "node.h"
JavaLang *jLang = nullptr;
%}

%union {
	JavaClassHeader *classHeader;
	std::string *str;
	JavaScope *scope;
	JavaModifiers *modifiers;
	FunctionList *funList;
	JavaFunction *javaFunc;
	Parameters *param;
	uint64_t *bodyHash;
}

%token O_BRACE C_BRACE T_CLASS ERROR O_PARAM C_PARAM
%token STATIC ABSTRACT STRICTFP SYNCHRONIZED NATIVE
%token PUBLIC PRIVATE PROTECTED DEFAULT
%token COMMA
%token <str> STRINGS
%token <bodyHash> BODY

%type <classHeader> CLASS_HEADER CLASS_NAME
%type <scope> SCOPE
%type <modifiers> MODIFIERS
%type <funList> FUNCTION_LIST
%type <javaFunc> FUNCTION FUNCTION_HEADER
%type <param> PARAMETERS


%start JAVA

%%

JAVA : CLASS_HEADER O_BRACE FUNCTION_LIST C_BRACE	{ jLang = new JavaLang($1, $3); YYACCEPT; }
		;


CLASS_HEADER : MODIFIERS CLASS_NAME		{ $$ = $2; $$->setModifier($1); }
			| CLASS_NAME				{ $$ = $1; }
			| CLASS_HEADER STRINGS		{ $$ = $1; delete $2; }
			| CLASS_HEADER COMMA		{ $$ = $1; }
			;


CLASS_NAME : T_CLASS STRINGS	{ $$ = new JavaClassHeader($2); delete $2; }
			;

FUNCTION_LIST : FUNCTION_LIST FUNCTION	{ $$ = $1; $$->addFunction($2); delete $2; }
			| FUNCTION					{ $$ = new FunctionList(); $$->addFunction($1); delete $1; }
			;

FUNCTION : FUNCTION_HEADER PARAMETERS BODY	{ $$ = $1; $1->setParameters($2); delete $2; 
											  $1->setBodyHash($3); delete $3;
											}
		;

FUNCTION_HEADER : MODIFIERS STRINGS STRINGS		{ $$ = new JavaFunction($2, $3); $$->setJavaModifier($1);
												  delete $1; delete $2; delete $3;
												}
				| MODIFIERS STRINGS				{ $$ = new JavaFunction($2); $$->setJavaModifier($1); 
												  delete $1; delete $2;
												}
				| STRINGS STRINGS				{ $$ = new JavaFunction($1, $2); 
												  delete $1; delete $2;
												}
				| STRINGS						{ $$ = new JavaFunction($1); delete $1; }
				;

PARAMETERS : O_PARAM C_PARAM							{	$$ = new Parameters(); yylexBody();	}
		| O_PARAM STRINGS STRINGS C_PARAM				{	$$ = new Parameters(); $$->addParameter($2);
															delete $2; delete $3; yylexBody();
														}
		| O_PARAM STRINGS STRINGS STRINGS C_PARAM		{	$$ = new Parameters(); $$->addParameter($3);
															delete $2; delete $3; delete $4; yylexBody();
														}

		| O_PARAM STRINGS STRINGS COMMA					{	$$ = new Parameters(); $$->addParameter($2);
															delete $2; delete $3;
														}
		| O_PARAM STRINGS STRINGS STRINGS COMMA			{	$$ = new Parameters(); $$->addParameter($3);
															delete $2; delete $3; delete $4;
														}
		| PARAMETERS STRINGS STRINGS COMMA				{	$$ = $1; $1->addParameter($2);
															delete $2; delete $3;
														}
		| PARAMETERS STRINGS STRINGS STRINGS COMMA		{	$$ = $1; $1->addParameter($3);
															delete $2; delete $3; delete $4;
														}
		| PARAMETERS STRINGS STRINGS C_PARAM			{	$$ = $1; $1->addParameter($2);
															delete $2; delete $3; yylexBody();
														}
		| PARAMETERS STRINGS STRINGS STRINGS C_PARAM	{	$$ = $1; $1->addParameter($3);
															delete $2; delete $3; delete $4; yylexBody();
														}
		;


MODIFIERS : MODIFIERS STATIC			{ $1->setStatic(); $$ = $1; }
			| MODIFIERS ABSTRACT		{ $1->setAbstract(); $$ = $1; }
			| MODIFIERS STRICTFP		{ $1->setStrictfp(); $$ = $1; }
			| MODIFIERS SYNCHRONIZED	{ $1->setSynchronized(); $$ = $1;}
			| MODIFIERS NATIVE			{ $1->setNative(); $$ = $1; }
			| SCOPE						{ $$ = new JavaModifiers(); $$->setScope($1); delete $1; }
			| STATIC					{ $$ = new JavaModifiers(); $$->setStatic(); }
			| ABSTRACT					{ $$ = new JavaModifiers(); $$->setAbstract(); }
			| STRICTFP					{ $$ = new JavaModifiers(); $$->setStrictfp(); }
			| SYNCHRONIZED				{ $$ = new JavaModifiers(); $$->setSynchronized(); }
			| NATIVE					{ $$ = new JavaModifiers(); $$->setNative(); }
			;

SCOPE : PUBLIC			{ $$ = new JavaScope(JavaScope::Scope::PUBLIC); }
		| PRIVATE		{ $$ = new JavaScope(JavaScope::Scope::PRIVATE); }
		| PROTECTED		{ $$ = new JavaScope(JavaScope::Scope::PROTECTED); }
		| DEFAULT		{ $$ = new JavaScope(JavaScope::Scope::DEFAULT); }
		;

%%