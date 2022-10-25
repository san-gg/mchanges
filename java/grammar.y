%code requires { #include "node.h" }

%code {

JavaLang *jLang = nullptr;

void checkArrayType(std::string* str1, std::string* str2) {
    if (str2->at(str2->size() - 1) == ']') {
        str1->append("[]");
    }
}

}

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
%token PUBLIC PRIVATE PROTECTED DEFAULT FINAL
%token COMMA EXTENDS IMPLEMENTS
%token <str> STRINGS GENERIC
%token <bodyHash> BODY

%type <classHeader> class_header class_name class_name_ext
%type <scope> scope
%type <modifiers> modifiers
%type <funList> function_list
%type <javaFunc> function function_header
%type <param> parameters


%start JAVA

%%

JAVA : class_header O_BRACE function_list C_BRACE    { jLang = new JavaLang($1, $3); YYACCEPT; }
     | class_header O_BRACE C_BRACE                  { jLang = new JavaLang($1); YYACCEPT; }
     | class_name_ext O_BRACE function_list C_BRACE  { jLang = new JavaLang($1, $3); YYACCEPT; }
     | class_name_ext O_BRACE C_BRACE                { jLang = new JavaLang($1); YYACCEPT; }
     ;

class_name_ext : class_header EXTENDS       { $$ = $1; }
               | class_header IMPLEMENTS    { $$ = $1; }
               | class_name_ext EXTENDS     { $$ = $1; }
               | class_name_ext IMPLEMENTS  { $$ = $1; }
               | class_name_ext STRINGS     { $$ = $1; delete $2; }
               | class_name_ext COMMA       { $$ = $1; }
               | class_name_ext GENERIC     { $$ = $1; delete $2; }
               ;

class_header : modifiers class_name  { $$ = $2; $$->setModifier($1); }
             | class_name            { $$ = $1; }
             | class_header GENERIC  { $$ = $1; $$->setGenericClass(); delete $2; }
			 ;

class_name : T_CLASS STRINGS     { $$ = new JavaClassHeader($2); delete $2; }
           ;

function_list : function_list function	{ $$ = $1; $$->addFunction($2); delete $2; }
              | function                { $$ = new FunctionList(); $$->addFunction($1); delete $1; }
              ;

function : function_header parameters BODY  { $$ = $1; $1->setParameters($2); delete $2; 
                                              $1->setBodyHash($3); delete $3;
                                            }
         ;

function_header : modifiers STRINGS STRINGS            { $$ = new JavaFunction($2, $3); $$->setJavaModifier($1);
                                                         delete $1; delete $2; delete $3;
                                                       }
           | modifiers STRINGS GENERIC STRINGS         { $2->append(*($3));
                                                         $$ = new JavaFunction($2, $4); $$->setJavaModifier($1);
                                                         delete $1; delete $2; delete $3; delete $4;
                                                       }
           | modifiers GENERIC STRINGS STRINGS         { $$ = new JavaFunction($3, $4); $$->setJavaModifier($1);
                                                         $$->setGenericFunction();
                                                         delete $1; delete $2; delete $3; delete $4;
                                                       }
           | modifiers GENERIC STRINGS GENERIC STRINGS {
                                                         $3->append(*($4));
                                                         $$ = new JavaFunction($3, $5); $$->setJavaModifier($1);
                                                         $$->setGenericFunction();
                                                         delete $1; delete $2; delete $3; delete $4; delete $5;
                                                       }
           | modifiers STRINGS                         { $$ = new JavaFunction($2); $$->setJavaModifier($1); 
                                                         delete $1; delete $2;
                                                       }
           | STRINGS STRINGS                           { $$ = new JavaFunction($1, $2); 
                                                         delete $1; delete $2;
                                                       }
           | STRINGS GENERIC STRINGS                   { $1->append(*($2));
                                                         $$ = new JavaFunction($1, $3);
                                                         delete $1; delete $2; delete $3;
                                                       }
           | GENERIC STRINGS STRINGS                   { $$ = new JavaFunction($2, $3);
                                                         $$->setGenericFunction();
                                                         delete $1; delete $2; delete $3;
                                                       }
           | GENERIC STRINGS GENERIC STRINGS           { $2->append(*($3));
                                                         $$ = new JavaFunction($2, $4);
                                                         $$->setGenericFunction();
                                                         delete $1; delete $2; delete $3; delete $4;
                                                       }
           | STRINGS                                   { $$ = new JavaFunction($1); delete $1; }
           ;

parameters : O_PARAM C_PARAM                            { $$ = new Parameters(); yylexBody(); }
           | O_PARAM STRINGS STRINGS C_PARAM            { checkArrayType($2, $3);
                                                          $$ = new Parameters(); $$->addParameter($2);
                                                          delete $2; delete $3; yylexBody();
                                                        }
           | O_PARAM FINAL STRINGS STRINGS C_PARAM      { checkArrayType($3, $4);
                                                          $$ = new Parameters(); $$->addParameter($3);
                                                          delete $3; delete $4; yylexBody();
                                                        }
           | O_PARAM STRINGS STRINGS COMMA              { checkArrayType($2, $3);
                                                          $$ = new Parameters(); $$->addParameter($2);
                                                          delete $2; delete $3;
                                                        }
           | O_PARAM FINAL STRINGS STRINGS COMMA        { checkArrayType($3, $4);
                                                          $$ = new Parameters(); $$->addParameter($3);
                                                          delete $3; delete $4;
                                                        }
           | parameters STRINGS STRINGS COMMA           { checkArrayType($2, $3);
                                                          $$ = $1; $1->addParameter($2);
                                                          delete $2; delete $3;
                                                        }
           | parameters FINAL STRINGS STRINGS COMMA     { checkArrayType($3, $4);
                                                          $$ = $1; $1->addParameter($3);
                                                          delete $3; delete $4;
                                                        }
           | parameters STRINGS STRINGS C_PARAM         { checkArrayType($2, $3);
                                                          $$ = $1; $1->addParameter($2);
                                                          delete $2; delete $3; yylexBody();
                                                        }
           | parameters FINAL STRINGS STRINGS C_PARAM   { checkArrayType($3, $4);
                                                          $$ = $1; $1->addParameter($3);
                                                          delete $3; delete $4; yylexBody();
                                                        }
           ;

modifiers : modifiers STATIC          { $1->setStatic(); $$ = $1; }
          | modifiers ABSTRACT        { $1->setAbstract(); $$ = $1; }
          | modifiers STRICTFP        { $1->setStrictfp(); $$ = $1; }
          | modifiers SYNCHRONIZED    { $1->setSynchronized(); $$ = $1;}
          | modifiers NATIVE          { $1->setNative(); $$ = $1; }
          | modifiers FINAL           { $1->setFinal(); $$ = $1; }
          | modifiers scope           { $1->setScope($2); delete $2; $$ = $1; }
          | scope                     { $$ = new JavaModifiers(); $$->setScope($1); delete $1; }
          | STATIC                    { $$ = new JavaModifiers(); $$->setStatic(); }
          | ABSTRACT                  { $$ = new JavaModifiers(); $$->setAbstract(); }
          | STRICTFP                  { $$ = new JavaModifiers(); $$->setStrictfp(); }
          | SYNCHRONIZED              { $$ = new JavaModifiers(); $$->setSynchronized(); }
          | NATIVE                    { $$ = new JavaModifiers(); $$->setNative(); }
          | FINAL                     { $$ = new JavaModifiers(); $$->setFinal(); }
          ;

scope : PUBLIC        { $$ = new JavaScope(JavaScope::Scope::PUBLIC); }
      | PRIVATE       { $$ = new JavaScope(JavaScope::Scope::PRIVATE); }
      | PROTECTED     { $$ = new JavaScope(JavaScope::Scope::PROTECTED); }
      | DEFAULT       { $$ = new JavaScope(JavaScope::Scope::DEFAULT); }
      ;

%%