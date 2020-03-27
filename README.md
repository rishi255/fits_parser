# fits_parser

A parser with an **interactive menu** that can **extract details from a FITS file**, and lets the user:  
* query keywords to get their values, and  
* update values of keywords in file.

## How to build and run:  

This project uses [CMake](https://cmake.org/), so if you don't have it, you'll need to get it from [here](https://cmake.org/download/) before proceeding.  

Once CMake is installed:
1. Open a terminal (Git Bash for Windows, bash for linux)  
2. Go to the directory where you want to download the repo  
3. `git clone https://github.com/rishi255/fits_parser.git`
4. Once done, follow these steps to build:  
   * `cd fits_parser`  
   * `cmake --build . --config Release`  
5. To run the project:  
   - `cd build`
   - `./FITS_Parser.exe` (for Windows) or `./FITS_Parser` (for Unix)

## Current features:  
Can parse all HDUs (primary HDU and standard extensions)

* **DISPLAY** - displays all keywords (HDU wise) with associated values and / comments present in the file.  
* **UPDATE [KEYWORD] [VALUE]** - update existing value of provided keyword with provided value.  
    * if **[VALUE]** is of invalid format/size, handles it accordingly.  
    (eg. if value is a character string, checks for single quotes at front and back, adds closing quote if not present.)  
* **[KEYWORD]** - displays value of provided keyword (HDU wise, shows all results if keyword present in more than one HDU)
* **COMMENT** - displays all comments (HDU wise) in the order they are found in the file.
* **END** - end program.  

## Limitations:  

* Unable to handle continued-string keywords.
* ~~Currently unable to handle **HISTORY** keywords, and can perform **no** actions on them.~~
* ~~Unable to handle extensions, can only extract primary header.~~

## Coming up...

* Command line functionality!  
Will be able to handle all commands described above as **command line arguments**, where the first argument will have to be the file name.  
eg. `./FITS_Parser  ../sample_file.fits  DISPLAY`
