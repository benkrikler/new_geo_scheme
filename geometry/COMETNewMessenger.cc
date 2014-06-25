#include "COMETNewMessenger.hh"

void COMETNewMessenger::Init(COMETComponentController* c){
    fController= dynamic_cast<COMETNewController*>(c);
    // Setup commands
    // Commands take ownership of the 
    G4UIparameter* name=new G4UIparameter ("Name", 's', false);

    G4String cmd_name=GetFullComponentName()+"/Dimension";
    fMakeLength=new G4UIcommand(cmd_name.c_str(),this);
    fMakeLength->SetParameter(new G4UIparameter(*name));
    fMakeLength->SetGuidance("Add a new dimension parameter such as a length or width");
    fMakeLength->SetGuidance("Either assign a value with units or provide an equation.");
    fMakeLength->SetGuidance("If you use a value with units, separate the units from the number by a space");
    fMakeLength->SetGuidance("If you provide an equation, start the equation with an '=' and "
            "surround all variables with square brackets ( eg.[SomOtherLength])");

    cmd_name=GetFullComponentName()+"/Print";
    fPrint=new G4UIcommand(cmd_name.c_str(),this);
    fPrint->SetParameter(new G4UIparameter(*name));
    fPrint->SetGuidance("Print the value of a named parameter");

    delete name;
}

COMETNewMessenger::~COMETNewMessenger(){
    delete fPrint;
    delete fMakeLength;
    delete fController;
}

void COMETNewMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue){
        std::stringstream ss(newValue);
        G4String name, remainder;
        // First word in newValue is the parameter name 
        ss>>name;
        remainder=ss.str();
        remainder.erase(0,name.length());
    if (cmd == fPrint) {
        fController->PrintParameter(name);
    } else if (cmd == fMakeLength) {
        fController->MakeLength(name,remainder);
    }
}

