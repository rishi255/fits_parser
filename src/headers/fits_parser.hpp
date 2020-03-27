#include <string>
#include <vector>
#include <unordered_map>
#include "card.hpp"

typedef std::unordered_map <std::string, std::string> umap;

class fits_parser   // class to parse primary HDU of a FITS file.
{
private:
    std::string filename;
    
    std::vector<card> cards;
    std::vector<card> comments;
    std::vector<card> history;

    umap cardmap;   // map <keyword, value_with_comment>
    void populate_map();
    void extract_cards(std::fstream &); 
    void update_file(std::fstream &, std::string &, std::string &);

public:
    fits_parser(std::string &fname);
    void getcommands(std::fstream &);
};