#include "COMETNewController.hh"
#include "COMETDoubleParameter.hh"

void COMETNewController::MakeLength(const G4String& name,const G4String& assignment){
    COMETDoubleParameter* param=new COMETDoubleParameter(this);
    param->SetName(name);
    param->SetUnitsCategory("Length");
    param->SetSource(assignment);
    AddParameter(param);
}

void COMETNewController::MakeAngle(const G4String& name,const G4String& assignment){
    COMETDoubleParameter* param=new COMETDoubleParameter(this);
    param->SetName(name);
    param->SetUnitsCategory("Angle");
    param->SetSource(assignment);
    AddParameter(param);
}
