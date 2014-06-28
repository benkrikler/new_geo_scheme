#ifndef SIMG4_COMETPARAMETERPARSER_H_
#define SIMG4_COMETPARAMETERPARSER_H_

#include <string>
#include <exception>
#include "COMETComponentController.hh"
class COMETParameterList;

namespace COMET{
    namespace geometry{
        struct EParameterError:std::exception{
            EParameterError(const std::string& m):fMsg(m){};
            ~EParameterError()throw(){};
            const char* what()const throw() {return fMsg.c_str();}
            private: std::string fMsg;
        };
        struct EUnmatchedBracket:EParameterError{
            EUnmatchedBracket(const std::string& l)
                :EParameterError("Unmatched bracket in expression: "+l){}
            ~EUnmatchedBracket()throw(){};
        };
        struct ENonExistentParameter:EParameterError{
            ENonExistentParameter(const std::string& p)
                :EParameterError("Parameter '"+p +"' cannot be found.\n"){}
            ~ENonExistentParameter()throw(){};
        };
        struct EWrongReturnType:EParameterError{
            EWrongReturnType(const std::string& l,const std::string& t)
                :EParameterError("Expression: '"+l+"' doesn't produce a "+t+"\n"){}
            ~EWrongReturnType()throw(){};
        };
        struct EMethodNotApplicable:EParameterError{
            EMethodNotApplicable(const std::string& m,const std::string& p,const std::string& t):
                EParameterError("Unable to call "+m+" on "+p+" which is of type "+t+"\n"){}
            ~EMethodNotApplicable()throw(){};
        };
        struct ETooFewParameters:EParameterError{
            ETooFewParameters(const std::string& m,const std::string& p)
                :EParameterError("Method '"+m+"' on '"+p+"' has too few parameters.\n"){}
            ~ETooFewParameters()throw(){};
        };
        struct ENestedSqBr:EParameterError{
            ENestedSqBr(const std::string& line)
                :EParameterError("Nested square brackets in expresssion '"+line+"\n"){}
            ~ENestedSqBr()throw(){};
        };
        namespace parameters{
            enum delim_t {
                kToken,
                kCapturing_delimiter,
                kNon_capturing_delimiter,
                kToken_type_change
            };
        }
    }
}

template <typename Delimiter>
class COMETTokenizer{

    public: 
    COMETTokenizer(Delimiter delimiter, const std::string& data):
        fDelim(delimiter),fDataStart(data.begin()),fDataEnd(data.end()),
        fCurrentPoint(data.begin()){
        };
    ~COMETTokenizer(){}

    bool next(){
        fCurrent="";
        bool keep_going=true;
        COMET::geometry::parameters::delim_t t;
        while(keep_going && fCurrentPoint!=fDataEnd){
            t=fDelim(*fCurrentPoint);
            switch(t){
                case COMET::geometry::parameters::kToken_type_change:
                    keep_going=false;
                    break;
                case COMET::geometry::parameters::kCapturing_delimiter:
                    keep_going=false;
                    if(fCurrent.empty()) {
                        fCurrent=*(fCurrentPoint++);
                    }
                    break;
                case COMET::geometry::parameters::kToken:
                    fCurrent+=*fCurrentPoint;
                    fCurrentPoint++;
                    break;
                case COMET::geometry::parameters::kNon_capturing_delimiter:
                    keep_going=false;
                    fCurrentPoint++;
                    break;
            }
        }
        if (fCurrentPoint==fDataEnd && fCurrent.empty()) return false;
        else if( fCurrent.empty() ) return next();
        return true;

    }
    const std::string& operator()()const{return fCurrent;}

    private:
    Delimiter fDelim;
    const std::string::const_iterator fDataStart;
    const std::string::const_iterator fDataEnd;
    std::string::const_iterator fCurrentPoint;
    std::string fCurrent;

};

class COMETDelimeter{
    public: 
        COMETDelimeter(const char* capt="[].", const char* non_capt=" \t\n"):fCapture(capt),fNonCapture(non_capt){}
        COMET::geometry::parameters::delim_t operator()(const char& c){
            for(std::string::const_iterator i=fCapture.begin(); i !=fCapture.end();i++)
                if( (*i)==c) return COMET::geometry::parameters::kCapturing_delimiter;
            for(std::string::const_iterator i=fNonCapture.begin(); i !=fNonCapture.end();i++)
                if( (*i)==c) return COMET::geometry::parameters::kNon_capturing_delimiter;
            return COMET::geometry::parameters::kToken;
        }
    private:
        const std::string fCapture, fNonCapture;
};



class COMETParameterParser{
    struct IdParam {
        int id;
        COMETVParameter* param;
        std::string type;
    };
    typedef std::map<G4String,IdParam> DependencyList;
    typedef std::vector<G4String> DependencyIndices;
    public:
        COMETParameterParser(COMETComponentController* c, COMETParameterList* d);
        virtual ~COMETParameterParser(){};
        std::string Process(const std::string& input);

        void PrintDependencies(const std::string& prefix="");

    private:
        std::string Parse(const std::string& input);
        int AddDependency(const std::string& param){};

    private:
    COMETComponentController* fController;
    COMETDelimeter fDelimeter;
    DependencyList fParameters;
    DependencyIndices fParametersIndices;
};

#endif //SIMG4_COMETPARAMETERPARSER_H_
