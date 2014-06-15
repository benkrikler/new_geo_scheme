#ifndef SIMG4_COMETNEWCONTROLLER_HH_
#define SIMG4_COMETNEWCONTROLLER_HH_

#include "COMETComponentController.hh"

class COMETNewController:public COMETComponentController{
    public:
        COMETNewController(const COMETComponentGeometry* geometry):
            COMETComponentController(geometry){ };
        ~COMETNewController(){
        };

        virtual void CreateParameters(){};

        // methods to pull from the base class
        using COMETComponentController::GetFullComponentName;
        using COMETComponentController::AddParameter;

        void PrintParameter(const G4String& name){COMETComponentController::PrintOneParameter(name);};
        void MakeLength(const G4String& name,const G4String& assignment);

};

#endif //SIMG4_COMETNEWCONTROLLER_HH_
