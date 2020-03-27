#include <iostream>

#include "headers/fits_parser.hpp"

int main()
{
    std::string filename;
    std::cout << "Enter filename to read (without extension .fits): ";
    std::cin >> filename;

    std::string fpath = "..\\fits_files\\";
    filename = filename + ".fits";
    fpath.append(filename);
    std::cout << "PATH: " << fpath << "\n";
    fits_parser fitsparser (fpath);
    
    return 0;
}