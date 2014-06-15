#include "TestNewGeometry.hh"
#include "COMETNewController.hh"
#include "COMETNewMessenger.hh"
#include <G4Box.hh>
#include <G4Tubs.hh>
#include "COMETComponentFactory.hh"

TestNewGeometry::~TestNewGeometry(){
    delete fMessenger;
}

void TestNewGeometry::Init(){
    SetController(new COMETNewController(this));
    fMessenger=new COMETNewMessenger(GetController());
}

G4LogicalVolume* TestNewGeometry::GetPiece(){
    G4VSolid* container=MakeBox("");
    G4LogicalVolume* logicContainer=MakeLogicalVolume(container,"Material","");

    G4VSolid* tube=MakeCylinder("Tube");
    G4LogicalVolume* logicTube=MakeLogicalVolume(tube,"Tube:Material","Tube");
    PlacePiece("Tube",logicContainer, logicTube);

    return logicContainer;
} 

COMETRegisterComponent( TestNew );
