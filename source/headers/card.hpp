#include <string>

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