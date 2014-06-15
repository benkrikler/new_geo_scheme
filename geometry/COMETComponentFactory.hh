#ifndef SIMG4_COMETCOMPONENTFACTORY_H_
#define SIMG4_COMETCOMPONENTFACTORY_H_

class COMETComponentGeometry;
class G4String;
#include <map>

class COMETComponentFactory{

 private: 
  COMETComponentFactory(){};
  ~COMETComponentFactory(){};

 public:
  static COMETComponentFactory* Instance();

  typedef COMETComponentGeometry*(*Maker)(COMETComponentGeometry*);
  static void RegisterComponent(const G4String&, Maker);
	static COMETComponentGeometry* BuildComponent(const G4String&, COMETComponentGeometry*);

 private:
  static COMETComponentFactory* fInstance;
  typedef std::map<G4String,Maker> Register;
  Register fMakersRegister;
};

#define COMETRegisterComponent_STRING(CLASS, NAME)\
namespace {\
COMETComponentGeometry* Maker_##CLASS(COMETComponentGeometry* parent){ return new CLASS(parent); }\
\
class RegistryProxy_##CLASS{\
    RegistryProxy_##CLASS() { \
      Factory::RegisterComponent(NAME,&Maker_##CLASS); \
    }\
}proxy_for_##CLASS;\
}

#define COMETRegisterComponent( CLASS  ) COMETRegisterComponent_STRING(CLASS , #CLASS)

#endif //SIMG4_COMETCOMPONENTFACTORY_H_
