#include <iostream>
#include <direct.h>
#include "headers/fits_parser.hpp"

std::string GetCurrentWorkingDir( void ) {
  char buff[FILENAME_MAX];
  _getcwd( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

int main()
{
    std::string filename;
    std::cout << "Enter filename to read (without extension .fits): ";
    std::cin >> filename;
    filename = filename + ".fits";

    std::string fpath = GetCurrentWorkingDir();
    std::cout << "CURR: " << fpath << "\n";

    fpath = fpath.substr(0, fpath.find_last_of("/\\"));
    std::cout << "ONE BACK: " << fpath << "\n";

    fpath.append("\\fits_files\\");
    fpath.append(filename);
    std::cout << "PROPER PATH: " << fpath << "|\n";
    fits_parser fitsparser (fpath);
    
    return 0;
}