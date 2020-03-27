#include "fits_parser.hpp"
#include <iostream>

int main()
{
    std::string filename;
    std::cout << "Enter filename to read (without extension .fits): ";
    std::cin >> filename;

    std::string fpath = "..\\fits_files\\";
    filename = filename + ".fits";
    fpath.append(filename);
    fits_parser fitsparser (fpath);
    
    return 0;
}