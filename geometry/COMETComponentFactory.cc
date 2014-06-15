#include "COMETComponentFactory.hh"
#include <G4String.hh>
#include "COMETComponentGeometry.hh"
#include "ICOMETLog.hxx"

COMETComponentFactory* COMETComponentFactory::fInstance=NULL;

COMETComponentFactory* COMETComponentFactory::Instance(){
  if(!fInstance) fInstance=new COMETComponentFactory();
  return fInstance;
}

void COMETComponentFactory::RegisterComponent(const G4String& name, Maker make){
  COMETComponentFactory* factory=Instance();
  Register::const_iterator it = factory->fMakersRegister.find(name);
  if(it!=factory->fMakersRegister.end()){
    COMETError("Trying to register a component with a name that is already in use: '"<<name<<"'");
    return;
  }
  factory->fMakersRegister[name]=make;
}

COMETComponentGeometry* COMETComponentFactory::BuildComponent(const G4String& name, COMETComponentGeometry* parent){
  COMETComponentFactory* factory=Instance();
  COMETComponentGeometry* component=NULL;
  Register::iterator it  = factory->fMakersRegister.find(name);
  if(it != factory->fMakersRegister.end() ){
    // make the module
    Maker make= it->second;
    component=make(parent);
  }else{
    COMETError("Unknown component requested: '"<<name<<"'");
  }
  return component;
}

G4bool COMETComponentFactory::Exists(const G4String& name){
  COMETComponentFactory* factory=Instance();
  int count=factory->fMakersRegister.count(name);
  return count>0;
}

void COMETComponentFactory::DumpComponents(const G4String& prefix){
  COMETComponentFactory* factory=Instance();
  COMETError(prefix<<factory->fMakersRegister.size()<<" components are available: ");
  for(Register::const_iterator it=factory->fMakersRegister.begin();
      it!=factory->fMakersRegister.end(); it++){
      COMETError(prefix<<it->first);
  }
}
