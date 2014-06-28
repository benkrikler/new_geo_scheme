#include "COMETParameterParser.hh"
#include "COMETParameterList.hh"
#include <iostream>
#include <TFormula.h>
#include <ICOMETLog.hxx>
using COMET::geometry::EParameterError;
using COMET::geometry::EUnmatchedBracket;
using COMET::geometry::ENonExistentParameter;
using COMET::geometry::EWrongReturnType;
using COMET::geometry::EMethodNotApplicable;
using COMET::geometry::ETooFewParameters;
using COMET::geometry::ENestedSqBr;

namespace{
    enum state{ kParameter, kFunction , fIgnore ,kZero,kStart_param, kStop_param, kStart_func, kStop_func,kArgument};

    void IncrementState(state& curr,state& prev){
        switch(curr){
            case kStart_param : prev=curr ; curr=kParameter ; break ; 
            case kStop_param  : prev=curr ; curr=fIgnore    ; break ; 
            case kStart_func  : prev=curr ; curr=kFunction  ; break ; 
            case kStop_func   : prev=curr ; curr=kArgument  ; break ; 
            default: break;
        }
    }
}

COMETParameterParser::COMETParameterParser(COMETComponentController* c, COMETParameterList* d):
    fController(c),fDelimeter("[,=]():"," \t\n"),fDependencies(d){
}

std::string COMETParameterParser::Process(const std::string& input) {
    fDependencies->Clear();
    return Parse(input);
}

std::string COMETParameterParser::Parse(const std::string& input) {
    COMETTokenizer<COMETDelimeter> tokens(fDelimeter,input);
    state curr=fIgnore;
    state prev=kZero;
    int i=0;
    int br_depth=0;
    std::string sub_sequence;
    std::string replaced;
    while(tokens.next()){
        // Check for brackets
        if(tokens()=="(") { br_depth++; if(br_depth==1)continue;
        }else if(tokens()==")") { 
            br_depth--;
            // Did we just find the last bracket ?
            if(0==br_depth) { 
                replaced+="("+Parse(sub_sequence)+")";//,controller);
                sub_sequence="";
                continue;
            }
        }
        // If we're inside brackets collect tokens then continue
        if(br_depth>0) {
            sub_sequence+=tokens(); 
            IncrementState(curr,prev);
            continue;
        }

        // Not inside brackets so check for special types of token
        if(  tokens()[0]=='['    ) { 
          if(curr==kParameter || curr==kStart_param) { throw ENestedSqBr(input); return "";}
          else prev=curr ; curr =kStart_param; 
        }else if(tokens()[0]==']') { prev=curr ; curr = kStop_param;    
            int ret=AddDependency(sub_sequence);
            if(ret<0) throw ENonExistentParameter(sub_sequence);
            replaced+=Form("[%d]",ret);
        }else {
            IncrementState(curr,prev);
            sub_sequence+=tokens();
            replaced+=tokens();
        }

        // debug
            const char*  State_str[]={ "parameter", "function" , "ignorable" ,
                    "zero","start_param", "stop_param",
                    "start_func", "stop_func","argument"};
            COMETLog("Token "<<i<<": '"<<tokens()<<"' is "<<State_str[curr]);
            i++;
        // debug
    }
    return replaced;
}

int COMETParameterParser::AddDependency(const G4String& target){
    // Check if we already know of this parameter
    DependencyList::const_iterator it =fParameters.find(target);
    if(it!= fParameters.end()) return it->second.id;

    // Make a new parameter
    COMETExternalParameter<double>* param=COMETExternalParameter<double>::Make(fController,target);
    if(!param) return -1;

    // add the new parameter to the dependency lists
    fParametersIndices.push_back(target);
    IdParam step;
    step.id=fParametersIndices.size()-1;
    step.param=param;
    fParameters[target]=step;

    // return the position in the ordered list of dependencies
    return step.id;
}

void COMETParameterParser::PrintDependencies(const std::string& prefix){
    for(DependencyIndices::const_iterator i_depend=fParametersIndices.begin();
            i_depend!=fParametersIndices.end();i_depend++){
        COMETLog(prefix<<*i_depend);
    }

}
