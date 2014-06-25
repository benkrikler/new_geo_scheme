#include "COMETParameterParser.hh"
#include <cctype>
#include <G4Tokenizer.hh>

using namespace COMET::geo_scheme::parser;
   static const G4String fTokenDelimiters;


Token Tokenizer::operator()()const{
  size_t position=fNextStart;

  // if we've reached the end return a blank string
  if(position > fInput.size()) return "";

  Token new_token;
  TokenType type;
  char c_curr;
  const char* delimiters=" \t[]
  // Wind on until we know what type this token will be
  while(position<fInput.size()){
    c_curr=fInput[position];
    position++;

    // check what type of token we're making
    type=FirstCharTokenType(c_curr);

    // if it's whitespace so far, skip
    if(type==kWhitespace) continue;

    // if we've just found a second ambiguous character complain
    if(new_token.GetType()==kAmbiguous && type==kAmbiguous){
      throw generic_parse_problem(position);
    }
    // Add this character to the token
    if(new_token.GetType()==kAmbiguous && type=kString) new_token.SetType(kFunction);
    else new_token.SetType(type);
    new_token+=c_curr;

    // if it's ambiguous, move on
    if(type==kAmbiguous){
      continue;
    }else{
      break;
    }

  }

  // Now get all the remaining characters that match this token type
  c_curr=fInput[position];
  while(CheckTokenType(c_curr, new_token.GetType()) && position<fInput.size() ){
    new_token+=c_curr;
    position++
  }

  // Now set the beginning of the next token
  fNextStart=position;
}

TokenType Tokenizer::FirstCharTokenType(char c)const{
  TokenType ret_val;
    if(isalpha(c)) ret_val =  kString;
    else if(isdigit(c)) ret_val =  kNumber;
    else if(c=='.') ret_val =  kAmbiguous;
    else if(c=='(') ret_val =  kBracket;
    else if(c=='[') ret_val =  kParameter;
    else if(ispunct(c)) ret_val =  kOperator;
    else if(isspace(c)) ret_val =  kWhitespace;
    return ret_val;
}

bool Tokenizer::CheckTokenType(char c, TokenType type)const{
  bool go_on;
  switch (type){
    case kString:
      go_on=isalnum(c);
      break;
    case kNumber:
      go_on=(isdigit(c)|| c=='.');
      break;
    case kOperator:
      go_on=(ispunct(c));
      break;
    case kFunction:
      go_on=(isalnum(c) || c=='_');
      break;
    case kBracket: go_on=false;
      break;
    case kParameter:
      go_on=(isalnum(c) || c=='_' || c=':' || c==']');
      break;
  }
  return ret_val;
}
