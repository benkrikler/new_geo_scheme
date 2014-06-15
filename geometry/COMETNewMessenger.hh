#ifndef SIMG4_COMETNEWMESSENGER_HH_
#define SIMG4_COMETNEWMESSENGER_HH_

#include "COMETNewController.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include <G4String.hh>

class COMETNewMessenger:public G4UImessenger{
    enum ParseCode_t {kSuccess, kUnmatchedParenthesis};
    public:
        COMETNewMessenger(COMETComponentController* c):G4UImessenger(),fController(NULL){Init(c);};
        void Init(COMETComponentController*);
        ~COMETNewMessenger();

        virtual void SetNewValue(G4UIcommand* cmd, G4String newValue);
    private:
        ParseCode_t ParseDoubleExpression();

        G4String GetFullComponentName(){return fController->GetFullComponentName();}

        G4UIdirectory* fDirectory;
        G4UIcommand* fMakeLength;
        G4UIcommand* fPrint;
        COMETNewController* fController;
};

#endif //SIMG4_COMETNEWMESSENGER_HH_
