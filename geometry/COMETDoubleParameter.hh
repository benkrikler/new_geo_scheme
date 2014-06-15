#ifndef SIMG4_COMETDOUBLEPARAMETER_HH
#define SIMG4_COMETDOUBLEPARAMETER_HH

#include "COMETVCombinationParameter.hh"
#include <map>
#include <vector>
class TFormula;

class COMETDoubleParameter:public COMETVCombinationParameter<double>{
    struct IdParam {int id; COMETVParameter* param;};
    typedef std::map<G4String,IdParam> DependencyList;
    typedef std::vector<G4String> DependencyIndices;
    public:
        COMETDoubleParameter(COMETComponentController* c):
            COMETVCombinationParameter<double>(c),fExpression(NULL),fSetByExpression(false){};
        ~COMETDoubleParameter(){};

    public: 
        // -----error codes-------
        enum ParsingError{
            kSuccess=0,
            kUnmatchedDelimiter=1,
            kUnParseable=2,
            kBadDependency=3,
            kTooManyParams=4
        };
        // ----- end error codes-------

        void MakeValue();
        ParsingError SetSource(const G4String& source);
        G4String GetDescription()const;

    private:
        ParsingError SetByExpression(G4String expression);
        ParsingError SetByValue(G4String expression);
        void CheckError(const G4String& input, ParsingError code);
        int AddDependency(const G4String& target);

    private:
    DependencyList fParameters;
    DependencyIndices fParametersIndices;
    TFormula* fExpression;
    G4String fUnitsType;
    G4bool fSetByExpression;
    enum {kMaxParams=100};

};

#endif //SIMG4_COMETDOUBLEPARAMETER_HH
