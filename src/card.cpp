#include "card.hpp"

card::card() { cardtext.reserve(80); }

card::card(std::string __cardtext, bool isEnd = false)
{
    cardtext.reserve(80);
    if(!isEnd)
        this->cardtext = __cardtext;
    else
    {
        std::string ct(80, ' ');
        ct.replace(0, 3, "END");
        this->cardtext = ct;
    }
    key = this->cardtext.substr(0,8);
    value_with_comment = this->cardtext.substr(9);
}

std::string card::get_key()   // returns key from cardtext by trimming
{
    return key;
}

std::string card::get_value_with_comment()
{
    return value_with_comment;
}

std::string card::get_comment()   // exclusively for COMMENT keyword
{
    return this->cardtext.substr(10);
}

std::string card::get_history() // exclusively for HISTORY keyword
{
    return this->cardtext.substr(8);
}
