// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "grammar.h"


// Unqualified %code blocks.


JavaLang *jLang = nullptr;

void checkArrayType(std::string* str1, std::string* str2) {
    if (str2->at(str2->size() - 1) == ']') {
        str1->append("[]");
    }
}




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {


  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
  {}
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}

  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_type.
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
                                                     { jLang = new JavaLang((yystack_[3].value.classHeader), (yystack_[1].value.funList)); YYACCEPT; }
    break;

  case 3:
                                                     { jLang = new JavaLang((yystack_[2].value.classHeader)); YYACCEPT; }
    break;

  case 4:
                                                     { jLang = new JavaLang((yystack_[3].value.classHeader), (yystack_[1].value.funList)); YYACCEPT; }
    break;

  case 5:
                                                     { jLang = new JavaLang((yystack_[2].value.classHeader)); YYACCEPT; }
    break;

  case 6:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); }
    break;

  case 7:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); }
    break;

  case 8:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); }
    break;

  case 9:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); }
    break;

  case 10:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); delete (yystack_[0].value.str); }
    break;

  case 11:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); }
    break;

  case 12:
                                            { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); delete (yystack_[0].value.str); }
    break;

  case 13:
                                     { (yylhs.value.classHeader) = (yystack_[0].value.classHeader); (yylhs.value.classHeader)->setModifier((yystack_[1].value.modifiers)); }
    break;

  case 14:
                                     { (yylhs.value.classHeader) = (yystack_[0].value.classHeader); }
    break;

  case 15:
                                     { (yylhs.value.classHeader) = (yystack_[1].value.classHeader); (yylhs.value.classHeader)->setGenericClass(); delete (yystack_[0].value.str); }
    break;

  case 16:
                                 { (yylhs.value.classHeader) = new JavaClassHeader((yystack_[0].value.str)); delete (yystack_[0].value.str); }
    break;

  case 17:
                                        { (yylhs.value.funList) = (yystack_[1].value.funList); (yylhs.value.funList)->addFunction((yystack_[0].value.javaFunc)); delete (yystack_[0].value.javaFunc); }
    break;

  case 18:
                                        { (yylhs.value.funList) = new FunctionList(); (yylhs.value.funList)->addFunction((yystack_[0].value.javaFunc)); delete (yystack_[0].value.javaFunc); }
    break;

  case 19:
                                            { (yylhs.value.javaFunc) = (yystack_[2].value.javaFunc); (yystack_[2].value.javaFunc)->setParameters((yystack_[1].value.param)); delete (yystack_[1].value.param); 
                                              (yystack_[2].value.javaFunc)->setBodyHash((yystack_[0].value.bodyHash)); delete (yystack_[0].value.bodyHash);
                                            }
    break;

  case 20:
                                                       { (yylhs.value.javaFunc) = new JavaFunction((yystack_[1].value.str), (yystack_[0].value.str)); (yylhs.value.javaFunc)->setJavaModifier((yystack_[2].value.modifiers));
                                                         delete (yystack_[2].value.modifiers); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 21:
                                                       { (yystack_[2].value.str)->append(*((yystack_[1].value.str)));
                                                         (yylhs.value.javaFunc) = new JavaFunction((yystack_[2].value.str), (yystack_[0].value.str)); (yylhs.value.javaFunc)->setJavaModifier((yystack_[3].value.modifiers));
                                                         delete (yystack_[3].value.modifiers); delete (yystack_[2].value.str); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 22:
                                                       { (yylhs.value.javaFunc) = new JavaFunction((yystack_[1].value.str), (yystack_[0].value.str)); (yylhs.value.javaFunc)->setJavaModifier((yystack_[3].value.modifiers));
                                                         (yylhs.value.javaFunc)->setGenericFunction();
                                                         delete (yystack_[3].value.modifiers); delete (yystack_[2].value.str); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 23:
                                                       {
                                                         (yystack_[2].value.str)->append(*((yystack_[1].value.str)));
                                                         (yylhs.value.javaFunc) = new JavaFunction((yystack_[2].value.str), (yystack_[0].value.str)); (yylhs.value.javaFunc)->setJavaModifier((yystack_[4].value.modifiers));
                                                         (yylhs.value.javaFunc)->setGenericFunction();
                                                         delete (yystack_[4].value.modifiers); delete (yystack_[3].value.str); delete (yystack_[2].value.str); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 24:
                                                       { (yylhs.value.javaFunc) = new JavaFunction((yystack_[0].value.str)); (yylhs.value.javaFunc)->setJavaModifier((yystack_[1].value.modifiers)); 
                                                         delete (yystack_[1].value.modifiers); delete (yystack_[0].value.str);
                                                       }
    break;

  case 25:
                                                       { (yylhs.value.javaFunc) = new JavaFunction((yystack_[1].value.str), (yystack_[0].value.str)); 
                                                         delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 26:
                                                       { (yystack_[2].value.str)->append(*((yystack_[1].value.str)));
                                                         (yylhs.value.javaFunc) = new JavaFunction((yystack_[2].value.str), (yystack_[0].value.str));
                                                         delete (yystack_[2].value.str); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 27:
                                                       { (yylhs.value.javaFunc) = new JavaFunction((yystack_[1].value.str), (yystack_[0].value.str));
                                                         (yylhs.value.javaFunc)->setGenericFunction();
                                                         delete (yystack_[2].value.str); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 28:
                                                       { (yystack_[2].value.str)->append(*((yystack_[1].value.str)));
                                                         (yylhs.value.javaFunc) = new JavaFunction((yystack_[2].value.str), (yystack_[0].value.str));
                                                         (yylhs.value.javaFunc)->setGenericFunction();
                                                         delete (yystack_[3].value.str); delete (yystack_[2].value.str); delete (yystack_[1].value.str); delete (yystack_[0].value.str);
                                                       }
    break;

  case 29:
                                                       { (yylhs.value.javaFunc) = new JavaFunction((yystack_[0].value.str)); delete (yystack_[0].value.str); }
    break;

  case 30:
                                                        { (yylhs.value.param) = new Parameters(); yylexBody(); }
    break;

  case 31:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = new Parameters(); (yylhs.value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str); yylexBody();
                                                        }
    break;

  case 32:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = new Parameters(); (yylhs.value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str); yylexBody();
                                                        }
    break;

  case 33:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = new Parameters(); (yylhs.value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str);
                                                        }
    break;

  case 34:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = new Parameters(); (yylhs.value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str);
                                                        }
    break;

  case 35:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = (yystack_[3].value.param); (yystack_[3].value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str);
                                                        }
    break;

  case 36:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = (yystack_[4].value.param); (yystack_[4].value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str);
                                                        }
    break;

  case 37:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = (yystack_[3].value.param); (yystack_[3].value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str); yylexBody();
                                                        }
    break;

  case 38:
                                                        { checkArrayType((yystack_[2].value.str), (yystack_[1].value.str));
                                                          (yylhs.value.param) = (yystack_[4].value.param); (yystack_[4].value.param)->addParameter((yystack_[2].value.str));
                                                          delete (yystack_[2].value.str); delete (yystack_[1].value.str); yylexBody();
                                                        }
    break;

  case 39:
                                      { (yystack_[1].value.modifiers)->setStatic(); (yylhs.value.modifiers) = (yystack_[1].value.modifiers); }
    break;

  case 40:
                                      { (yystack_[1].value.modifiers)->setAbstract(); (yylhs.value.modifiers) = (yystack_[1].value.modifiers); }
    break;

  case 41:
                                      { (yystack_[1].value.modifiers)->setStrictfp(); (yylhs.value.modifiers) = (yystack_[1].value.modifiers); }
    break;

  case 42:
                                      { (yystack_[1].value.modifiers)->setSynchronized(); (yylhs.value.modifiers) = (yystack_[1].value.modifiers);}
    break;

  case 43:
                                      { (yystack_[1].value.modifiers)->setNative(); (yylhs.value.modifiers) = (yystack_[1].value.modifiers); }
    break;

  case 44:
                                      { (yystack_[1].value.modifiers)->setFinal(); (yylhs.value.modifiers) = (yystack_[1].value.modifiers); }
    break;

  case 45:
                                      { (yystack_[1].value.modifiers)->setScope((yystack_[0].value.scope)); delete (yystack_[0].value.scope); (yylhs.value.modifiers) = (yystack_[1].value.modifiers); }
    break;

  case 46:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setScope((yystack_[0].value.scope)); delete (yystack_[0].value.scope); }
    break;

  case 47:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setStatic(); }
    break;

  case 48:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setAbstract(); }
    break;

  case 49:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setStrictfp(); }
    break;

  case 50:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setSynchronized(); }
    break;

  case 51:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setNative(); }
    break;

  case 52:
                                      { (yylhs.value.modifiers) = new JavaModifiers(); (yylhs.value.modifiers)->setFinal(); }
    break;

  case 53:
                      { (yylhs.value.scope) = new JavaScope(JavaScope::Scope::PUBLIC); }
    break;

  case 54:
                      { (yylhs.value.scope) = new JavaScope(JavaScope::Scope::PRIVATE); }
    break;

  case 55:
                      { (yylhs.value.scope) = new JavaScope(JavaScope::Scope::PROTECTED); }
    break;

  case 56:
                      { (yylhs.value.scope) = new JavaScope(JavaScope::Scope::DEFAULT); }
    break;



            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char parser::yypact_ninf_ = -17;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      95,   -12,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,     6,    -2,     2,   -17,   109,   -17,   -17,   -17,
      20,   -17,   -17,   -17,   -17,   -17,    35,   -17,   -17,   -17,
     -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -14,
      48,    50,   -17,    64,    80,   -17,    65,   -17,    62,     4,
     -17,   -17,    -6,   -11,    18,    63,   -17,   -17,   -17,    77,
     -17,    79,    93,    94,   110,   -17,   -17,   111,    33,   -17,
     112,    -5,   114,    -4,   -17,   -17,   115,   120,   -17,   -17,
     121,   -17,   -17,   -17,   -17,   -17,   -17,   -17
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,    47,    48,    49,    50,    51,    53,    54,    55,
      56,    52,     0,     0,     0,    14,     0,    46,    16,     1,
       0,    11,     8,     9,    10,    12,     0,     6,     7,    15,
      39,    40,    41,    42,    43,    44,    13,    45,     5,    29,
       0,     0,    18,     0,     0,     3,     0,    25,     0,     0,
       4,    17,     0,     0,    24,     0,     2,    26,    27,     0,
      30,     0,     0,     0,     0,    19,    20,     0,     0,    28,
       0,     0,     0,     0,    21,    22,     0,     0,    31,    33,
       0,    37,    35,    23,    32,    34,    38,    36
  };

  const short
  parser::yypgoto_[] =
  {
     -17,   -17,   -17,   -17,    70,    91,    89,   -17,   -17,   131,
     -16
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    12,    13,    14,    15,    41,    42,    43,    53,    44,
      17
  };

  const signed char
  parser::yytable_[] =
  {
      37,    20,    60,    78,    81,    26,    19,    63,    47,    48,
      18,    64,    61,    65,    79,    82,    62,    21,    22,    23,
      24,    25,    27,    28,    38,    29,    58,    59,    37,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    45,
      66,    67,    39,    40,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    50,    75,    76,    39,    40,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    56,
      49,    52,    39,    40,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    57,    68,    36,    39,    40,    30,
      31,    32,    33,    34,     7,     8,     9,    10,    35,    69,
       1,    70,    54,    55,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     1,    71,    72,    46,    30,    31,
      32,    33,    34,     7,     8,     9,    10,    35,    84,    86,
      51,    16,    73,    74,    77,    51,    80,    83,     0,    85,
      87
  };

  const signed char
  parser::yycheck_[] =
  {
      16,     3,     8,     8,     8,     3,     0,    18,    22,    23,
      22,    22,    18,    24,    19,    19,    22,    19,    20,    21,
      22,    23,    20,    21,     4,    23,    22,    23,    44,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,     4,
      22,    23,    22,    23,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,     4,    22,    23,    22,    23,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,     4,
      22,     7,    22,    23,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    22,    22,    16,    22,    23,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    22,
       5,    22,    22,    23,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,     5,    22,    22,    26,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,     8,     8,
      41,     0,    22,    22,    22,    46,    22,    22,    -1,    19,
      19
  };

  const signed char
  parser::yystos_[] =
  {
       0,     5,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    26,    27,    28,    29,    34,    35,    22,     0,
       3,    19,    20,    21,    22,    23,     3,    20,    21,    23,
       9,    10,    11,    12,    13,    18,    29,    35,     4,    22,
      23,    30,    31,    32,    34,     4,    30,    22,    23,    22,
       4,    31,     7,    33,    22,    23,     4,    22,    22,    23,
       8,    18,    22,    18,    22,    24,    22,    23,    22,    22,
      22,    22,    22,    22,    22,    22,    23,    22,     8,    19,
      22,     8,    19,    22,     8,    19,     8,    19
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    25,    26,    26,    26,    26,    27,    27,    27,    27,
      27,    27,    27,    28,    28,    28,    29,    30,    30,    31,
      32,    32,    32,    32,    32,    32,    32,    32,    32,    32,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    35,    35,    35,    35
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     4,     3,     4,     3,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     2,     2,     2,     1,     3,
       3,     4,     4,     5,     2,     2,     3,     3,     4,     1,
       2,     4,     5,     4,     5,     4,     5,     4,     5,     2,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "O_BRACE", "C_BRACE", "T_CLASS", "ERROR",
  "O_PARAM", "C_PARAM", "STATIC", "ABSTRACT", "STRICTFP", "SYNCHRONIZED",
  "NATIVE", "PUBLIC", "PRIVATE", "PROTECTED", "DEFAULT", "FINAL", "COMMA",
  "EXTENDS", "IMPLEMENTS", "STRINGS", "GENERIC", "BODY", "$accept", "JAVA",
  "class_name_ext", "class_header", "class_name", "function_list",
  "function", "function_header", "parameters", "modifiers", "scope", YY_NULLPTR
  };


  const unsigned char
  parser::yyrline_[] =
  {
       0,    45,    45,    46,    47,    48,    51,    52,    53,    54,
      55,    56,    57,    60,    61,    62,    65,    68,    69,    72,
      77,    80,    84,    88,    94,    97,   100,   104,   108,   113,
     116,   117,   121,   125,   129,   133,   137,   141,   145,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   167,   168,   169,   170
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
    };
    const int user_token_number_max_ = 279;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

} // yy

