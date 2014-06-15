#ifndef TESTNEWGEOMETRY_H_
#define TESTNEWGEOMETRY_H_

#include "COMETComponentGeometry.hh"

class COMETNewMessenger;

class TestNewGeometry:public COMETComponentGeometry{
    public:

    TestNewGeometry(COMETComponentGeometry* parent):
        COMETComponentGeometry("TestNew", parent){Init();};
    ~TestNewGeometry();

    void Init();

    G4LogicalVolume* GetPiece();

    private:
    COMETNewMessenger* fMessenger;

};

#endif// TESTNEWGEOMETRY_H_
