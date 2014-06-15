#ifndef SIMG4_COMETCOMPONENTFACTORY_H_
#define SIMG4_COMETCOMPONENTFACTORY_H_

class COMETComponentGeometry;
class G4String;
#include <map>
#include <globals.hh>

class COMETComponentFactory{

 private: 
  COMETComponentFactory(){};
  ~COMETComponentFactory(){};

 public:
  static COMETComponentFactory* Instance();

  typedef COMETComponentGeometry*(*Maker)(COMETComponentGeometry*);
  static void RegisterComponent(const G4String&, Maker);
	static COMETComponentGeometry* BuildComponent(const G4String&, COMETComponentGeometry*);
  static G4bool Exists(const G4String& ); 
  static void DumpComponents(const G4String& prefix="    |");

 private:
  static COMETComponentFactory* fInstance;
  typedef std::map<G4String,Maker> Register;
  Register fMakersRegister;
};

#define COMETRegisterComponent_STRING(CLASS, NAME)\
namespace {\
COMETComponentGeometry* Maker_##CLASS(COMETComponentGeometry* parent){ return new CLASS(parent); }\
\
struct RegistryProxy_##CLASS{\
    RegistryProxy_##CLASS() { \
      COMETComponentFactory::RegisterComponent(NAME,&Maker_##CLASS); \
    }\
}proxy_for_##CLASS;\
}

#define COMETRegisterComponent( CLASS  ) COMETRegisterComponent_STRING(CLASS##Geometry , #CLASS)

#endif //SIMG4_COMETCOMPONENTFACTORY_H_
