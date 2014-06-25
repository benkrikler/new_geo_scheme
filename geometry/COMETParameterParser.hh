#ifndef SIMG4_COMETPARAMETERPARSER_H_
#define SIMG4_COMETPARAMETERPARSER_H_

#include <G4String.hh>
#include <exception>

namespace COMET{
 namespace geo_scheme{
   class Parser;
 namespace parser{
  class  Tokenizer;
  class  Token;
  enum  TokenType { kUnits,kString, kParameter, kOperator , kFunction
                    kBracket, kNonMeaningful, kWhitespace,kAmbiguous};
  namespace errors{
    class unmatched_parenthesis;
    class generic_parse_problem;
  }
 }
}

// 
// class COMET::geo_scheme::parser::Token:public G4String{
//    public:
// 
//     Token(const G4String& s,TokenType t):G4String(s),fType(t),fNextStart(0){};
//     virtual ~Token(){};
// 
//     TokenType SetType(TokenType t){ fType=t;};
//     TokenType GetType()const{return fType};
//    private:
//     TokenType fType;
// };
// 
// class COMET::geo_scheme::parser::Tokenizer{
//  public:
//   Tokenizer(const G4String& s):fInput(s){};
//   virtual ~Tokenizer(){};
// 
//   Token operator()()const;
// 
//   static TokenType FirstCharTokenType(char c)const;
//   static TokenType CheckTokenType(char c)const;
// 
//  private:
//   G4String fInput;
//   size_t fNextStart;
// };

// class to break a parameter definition string into tokens and process the
// results
class COMET::geo_scheme::Parser{
  typedef std::vector<G4String> TokenList;
  public:
   parser(){};
   virtual ~parser(){};

  public:
   bool ProcessString(const G4String& s);

  private:
   G4String fInput;
   TokenList fTokens;
   static const G4String fTokenDelimiters;

}

#endif //SIMG4_COMETPARAMETERPARSER_H_
