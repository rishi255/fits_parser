# fits_parser

A parser with an **interactive menu** that can **extract details from a FITS file**, and lets the user:  
* query keywords to get their values, and  
* update values of keywords in file.

## Current features:

* **DISPLAY** - displays all keywords with associated values and / comments present in the file.  
* **UPDATE [KEYWORD] [VALUE]** - update existing value of provided keyword with provided value.  
    * if **[VALUE]** is of invalid format/size, handles it accordingly.  
    (eg. if value is a character string, checks for single quotes at front and back, adds closing quote if not present.)  
* **[KEYWORD]** - displays value of provided keyword.
* **COMMENT** - displays all comments in the order they are found in the file.
* **END** - end program.  

## Limitations:

* Currently unable to handle **HISTORY** keywords, and can perform **no** actions on them.
* Unable to handle continued-string keywords (multi-line comments and so on).