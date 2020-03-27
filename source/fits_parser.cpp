#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>

#include <sstream>
#include <functional>
#include <iomanip>
#include <iterator>

#include "headers/fits_parser.hpp"

#define ull unsigned long long int 

fits_parser::fits_parser (std::string &fname)
{
    filename = fname;
    std::fstream file(fname, std::ios::in | std::ios::out | std::ios::binary);
    extract_cards(file);
    populate_map();
    getcommands(file);
}

void fits_parser::populate_map()
{
    cardmap.clear();
    for (auto i=cards.begin(); i!=cards.end() && i->get_key() != "END     ";i++)
    {
        cardmap.emplace(i->get_key(), i->get_value_with_comment());
    }
}

void fits_parser::extract_cards(std::fstream &file)
    {
        file.clear();
        file.seekp(0, std::ios::beg);
        cards.clear();
        comments.clear();
        history.clear();
        char cardtext[81];

        file.read(cardtext, 80);
        std::string s(cardtext);
        
        while(!file.eof() && s.substr(0,3) != "END")
        {
            if(s.substr(0,3) != "END")
            {
                if(s.substr(0,7) == "COMMENT")
                    comments.emplace_back(card(std::string(cardtext)));
                
                else if (s.substr(0,7) == "HISTORY")
                    history.emplace_back(card(std::string(cardtext)));

                else
                    cards.emplace_back(card(std::string(cardtext)));
            }
            else
            {
                cards.emplace_back(card(std::string(""), true));
                s = "END     ";
            }

            file.read(cardtext, 80);
            s = std::string(cardtext);
        }

        cards.shrink_to_fit();
        comments.shrink_to_fit();
        history.shrink_to_fit();
    }

void fits_parser::getcommands (std::fstream &file)
    {
        std::cout << "\nEnter keyword (in caps) to query, \"END\" to exit: \n";
        std::string query, keyarg, valarg;
        
        do
        {
            std::cin >> query;
            
            if (query == "END")
                break;

            else if (query == "DISPLAY")
            {
                for(auto i=cards.begin(); i!=cards.end(); i++)
                    std::cout << i->get_key() << "= " 
                    << i->get_value_with_comment() << std::endl;
            }

            else if (query == "UPDATE")
            {
                std::cin >> keyarg;
                std::cin >> valarg;
                
                if(keyarg == "COMMENT")
                {
                    ; // TODO handle COMMENT updates
                }

                else if(keyarg == "HISTORY")
                {
                    ; // TODO handle HISTORY update
                }

                else
                    update_file(file, keyarg, valarg);
            }

            else if (query == "COMMENT")
            {
                // just display all comments in order
                for(auto&& i : comments)
                {
                    std::cout << i.get_comment() << "\n";
                }
            }

            else if (query == "HISTORY")
            {
                // just display all history in order
                for(auto&& i : history)
                {
                    std::cout << i.get_history() << "\n";
                }
            }

            else    
            {
                // just a keyword whose value to display
                
                std::cout << "->        ";
                if(query.size() < 8)
                    query.append(8-query.size(), ' ');

                umap::const_iterator it = cardmap.find(query);
            
                if(it != cardmap.end())
                    std::cout << it->second << std::endl;
                else
                    std::cerr << " NOT FOUND!\n";
            }

            std::cout << "\n";
        }while(query != "END");
    }

void fits_parser::update_file
(std::fstream &file, std::string &keyarg, std::string &valarg)
    {
        file.clear();
        file.seekp(0, std::ios::beg);

        bool found = false, charStr = false, invalidFormat = false;;
   
        // * manipulate valarg to conform to standards
        if (valarg.front() == '\'')
        {
            charStr = true;
            if(valarg.back() != '\'')
            {
                invalidFormat = true;
                std::cerr << "UNCLOSED single quote ?? \n";
                // (then append single quote to back later anyway)
            }
        }
        
        // * value string size cannot be greater than 70 
        // * (including quotes ' ' in case of char strings)
        if(valarg.size() > 70)
        {
            std::cerr << "VALARG TOO LONG!\n";
            // TODO: handle continued-string keywords
        }
        else if (valarg.size() < 70)
        {
            // * append required no of spaces
            if (charStr)
            {
                valarg.append(69-valarg.size(), ' ');

                if(invalidFormat)
                    valarg.append("\'");
                // closing quote appended if not provided
            }
            else
            {
                valarg.append(70-valarg.size(), ' ');
            }
        }
        
        char temp[81];        
        if(!file.eof())
            file.read(temp, 80);
        std::string s(temp);
        
        while(!file.eof())
        {
            if(s.substr(0, keyarg.size()) == keyarg)
            {
                std::cout << "FOUND! Trying to update...\n";
                ull pos = file.tellp();
                file.seekp(pos - 70);
                file << valarg;
                file.seekp(pos);  // reset cursor to old position
                found = true;
                break;
            }

            file.read(temp, 80);
            s = std::string(temp);
        }

        if(!found)
            std::cerr << "KEYWORD NOT FOUND!\n";

        extract_cards(file);
        populate_map();
    }