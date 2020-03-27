#include <string>
#include <vector>
#include <map>

typedef std::map <std::string, std::string> umap;

class card
{
    std::string cardtext;
    std::string key, value_with_comment;
public:
    card();

    card(std::string __cardtext, bool isEnd = false);

    std::string get_key();   // returns key from cardtext by trimming

    std::string get_value_with_comment();

    std::string get_comment();   // exclusively for COMMENT keyword

    std::string get_history(); // exclusively for HISTORY keyword
};

class hdu
{
public:
    std::vector<card> cards;
    std::vector<card> comments;
    std::vector<card> history;

    umap cardmap;   // map <keyword, value_with_comment>

    void clear()
    {
        cards.clear();
        comments.clear();
        history.clear();
    }

};

class fits_parser   // class to parse a FITS file.
{
private:
    std::string filename;
    std::vector<hdu> HDUS;  

    void populate_map();
    void extract_cards(std::fstream &); 
    void update_file(std::fstream &, std::string &, std::string &);

public:
    fits_parser(std::string &fname);
    void getcommands(std::fstream &);
};