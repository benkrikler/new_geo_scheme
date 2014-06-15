#include "COMETNewController.hh"
#include "COMETDoubleParameter.hh"

void COMETNewController::MakeLength(const G4String& name,const G4String& assignment){
    COMETDoubleParameter* param=new COMETDoubleParameter(this);
    param->SetName(name);
    param->SetSource(assignment);
    AddParameter(param);
}
