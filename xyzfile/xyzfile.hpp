#pragma once

#include <iostream>
#include <vector>
#include <map>

class XYZFile
{
// Methods
public:
    XYZFile(std::string Filename);
    void Summary();
    void Write();
    std::vector<double> CenterOfMass();
    void CenterCOM_inplace();
    std::vector<std::vector<double>> CenterCOM_copy();
    
private:
    void Parse();
    void ReadAtomicMasses();

// Attributes
public:
    int Natoms;
    std::string Filename;
    std::string Comment;
    
private:
    std::vector<std::vector<double>> Coordinates;
    std::vector<std::string> Labels;
    std::map<std::string, float> AtomicMasses;
};