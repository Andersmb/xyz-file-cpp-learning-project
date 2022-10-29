#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "xyzfile.hpp"

XYZFile::XYZFile(std::string Filename){
    this->Filename = Filename;
    this->Parse();
    this->ReadAtomicMasses();
    }
 
void XYZFile::Parse(){
    std::ifstream file (this->Filename);
    
    int natoms;
    std::string comment;
    
    file >> natoms;               // Read first word into an integer
    std::getline(file, comment);  // Must consume the first line
    std::getline(file, comment);  // Now we can get the second line
    
    this->Natoms = natoms;
    this->Comment = comment;
    
    std::vector<std::vector<double>> coords(this->Natoms);
    std::vector<std::string> labels(this->Natoms);
        
    std::string line;
    for (int atom = 0; atom < this->Natoms; atom++){
        std::getline(file, line);
        std::istringstream iss(line);
        
        std::string label;
        double x, y, z;
        iss >> label;
        iss >> x >> y >> z;
        
        coords[atom].push_back(x);
        coords[atom].push_back(y);
        coords[atom].push_back(z);
        
        labels[atom] = label;
    }
    
    this->Coordinates = coords;
    this->Labels = labels;
    file.close();
}

void XYZFile::Write(){
    std::ofstream file (this->Filename);
    file << this->Natoms << std::endl;
    file << this->Comment << std::endl;
    
    std::string sym;
    double x, y, z;
    for (int i = 0; i < this->Natoms; i++){
        sym = this->Labels[i];
        x = this->Coordinates[i][0];
        y = this->Coordinates[i][1];
        z = this->Coordinates[i][2];
        
        file << sym << " " << x << " " << y << " " << z << std::endl;
    }
}

void XYZFile::Summary(){
    std::cout << "File path        : " << this->Filename << std::endl;
    std::cout << "Number of Atoms  : " << this->Natoms << std::endl;
    std::cout << "Comment          : " << this->Comment << std::endl;
    std::cout << "Center of Mass   : " << this->CenterOfMass()[0] << " " << this->CenterOfMass()[1] << " " << this->CenterOfMass()[2] << std::endl;
}

void XYZFile::ReadAtomicMasses(){
    std::map<std::string, float> dict;
    
    std::ifstream file ("atomic_masses.txt");
    
    std::string key;
    float value;
    std::string line;
    
    while (file.good()){
        std::getline(file, line);
        std::istringstream iss(line);
        iss >> key >> value;
        dict[key] = value;
    }
    
    this->AtomicMasses = dict;
}

std::vector<double> XYZFile::CenterOfMass(){
    std::vector<double> com;
    
    // Only necessary to compute the total mass once, so use separate loop
    double totalMass = 0;
    for (std::string atom : this->Labels){
        totalMass += this->AtomicMasses[atom];
    }
    
    double com_component;
    for (int dim = 0; dim < 3; dim++){
        com_component = 0;
        for (int atom = 0; atom < this->Natoms; atom++){
            com_component += this->Coordinates[atom][dim] * this->AtomicMasses[this->Labels[atom]];
        }
        com.push_back(com_component / totalMass);
    }
    return com;
}

void XYZFile::CenterCOM_inplace(){
    // Translate atomic coordinates such that the center of mass is at the origin
    // We subtract COM from each atom vector (component wise)
    std::vector<double> com = this->CenterOfMass();

    for (int comp = 0; comp < 3; comp++){
        for (int atom = 0; atom < this->Natoms; atom++){
            this->Coordinates[atom][comp] -= com[comp];
        }
    }
}

std::vector<std::vector<double>> XYZFile::CenterCOM_copy(){
    std::vector<std::vector<double>> result(this->Natoms, std::vector<double>(3));
    std::vector<double> com = this->CenterOfMass();

    for (int comp = 0; comp < 3; comp++){
        for (int atom = 0; atom < this->Natoms; atom++){
            result[atom][comp] = this->Coordinates[atom][comp] - com[comp];
        }
    }
    return result;
}