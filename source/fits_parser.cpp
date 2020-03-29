#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
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
    for(auto && h : HDUS)
    {
        h.cardmap.clear();
        for (auto i=h.cards.begin(); i!=h.cards.end() && i->get_key() != "END     ";i++)
        {
            h.cardmap.emplace(i->get_key(), i->get_value_with_comment());
        }
    }
}

void fits_parser::extract_cards(std::fstream &file)
{
    file.clear();
    file.seekp(0, std::ios::beg);
    HDUS.clear();

    char cardtext[81];

    file.read(cardtext, 80);
    std::string s(cardtext);
    
    hdu temp_hdu;
    while(!file.eof() && s.substr(0,8) != "END     ")
    {
        if(s.substr(0,3) != "END")
        {
            if(s.substr(0,7) == "COMMENT")
                temp_hdu.comments.emplace_back(card(std::string(cardtext)));
            
            else if (s.substr(0,7) == "HISTORY")
                temp_hdu.history.emplace_back(card(std::string(cardtext)));

            else
                temp_hdu.cards.emplace_back(card(std::string(cardtext)));
        }
        else
        {
            temp_hdu.cards.emplace_back(card(std::string(""), true));
            s = "END     ";
        }

        file.read(cardtext, 80);
        s = std::string(cardtext);
    }
    // end of primary HDU 

    HDUS.emplace_back(temp_hdu);
    temp_hdu.clear();

    file.read(cardtext, 80);
    s = std::string(cardtext);

    size_t x = 1;
    bool extFound = false, binTblExt = false, imgExt = false, ascTblExt = false, notStandard;
    while(!file.eof())
    {
        extFound = false;
        if(s.substr(0,8)=="XTENSION")
        {
            std::cout <<    "\n\n\n\n******************************EXTENSION  FOUND******************************\n\n\n";
            extFound = true;
            
            if(s.substr(11,8)=="IMAGE   ")
                imgExt = true;
            else if (s.substr(11,5)=="BINTABLE")
                binTblExt = true;
            else if (s.substr(11,5)=="TABLE   ")
                ascTblExt = true;
            else
                notStandard = true;             
        }

        while(extFound)
        {
            // std::cout << "EXT FIELD NUMBER " << x++ << "\n";
            if(s.substr(0,3) != "END")
            {
                if(s.substr(0,7) == "COMMENT")
                    temp_hdu.comments.emplace_back(card(std::string(cardtext)));
                
                else if (s.substr(0,7) == "HISTORY")
                    temp_hdu.history.emplace_back(card(std::string(cardtext)));
        
                else
                    temp_hdu.cards.emplace_back(card(std::string(cardtext)));
            }
            else
            {
                temp_hdu.cards.emplace_back(card(std::string(""), true));
                s = "END     ";
                extFound = false;

                temp_hdu.cards.shrink_to_fit();
                temp_hdu.comments.shrink_to_fit();
                temp_hdu.history.shrink_to_fit();
                // std::cout << "YAY NEW XTENSION!\n";

                HDUS.emplace_back(temp_hdu);
            }
            file.read(cardtext, 80);
            s = std::string(cardtext);
        } // end inner while loop (single extension scan over)
        
        file.read(cardtext, 80);
        s = std::string(cardtext);

        temp_hdu.clear();
    } // end outer while loop (all extensions scanned)

}

void fits_parser::getcommands (std::fstream &file)
{
    std::cout << "\nEnter keyword (in caps) to query, \"END\" to exit: \n";
    std::string query, keyarg, valarg;
    size_t hdu_no;
    
    do
    {
        std::cin >> query;
        
        if (query == "END")
            break;

        else if (query == "DISPLAY")
        {
            std::cout << "NUMBER OF HDUS: " << HDUS.size() << "\n\n";
            size_t c = 1;
            for(auto&& h : HDUS)
            {
                std::cout << "\n\n******************************HEADER NUMBER: " << c++ << "******************************\n\n";
                for(auto i=h.cards.begin(); i!=h.cards.end(); i++)
                    std::cout << i->get_key() << "= " << i->get_value_with_comment() << std::endl;
            }
        }

        else if (query == "UPDATE")
        {
            std::cin >> hdu_no;
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
                update_file(file, hdu_no, keyarg, valarg);
        }

        else if (query == "COMMENT")
        {
            // just display all comments in order
            size_t c = 1;
            for(auto&& h : HDUS)
            {
                std::cout << "\n\n******************************HEADER NUMBER: " << c++ << "******************************\n\n";
                for(auto&& i : h.comments)
                {
                    std::cout << i.get_comment() << "\n";
                }
            }
        }

        else if (query == "HISTORY")
        {
            // just display all history in order
            size_t c = 1;
            for(auto&& h : HDUS)
            {
                std::cout << "\n\n******************************HEADER NUMBER: " << c++ << "******************************\n\n";
                for(auto&& i : h.history)
                {
                    std::cout << i.get_history() << "\n";
                }
            }
        }

        else    
        {
            // just a keyword whose value to display (from all HDUs)
            size_t c = 1;
            for(auto&& h : HDUS)
            {
                if(query.size() < 8)
                    query.append(8-query.size(), ' ');

                umap::const_iterator it = h.cardmap.find(query);

                if(it != h.cardmap.end())
                {
                    std::cout << "\n\nHEADER NUMBER: " << c << "\n\n";
                    std::cout << "->        ";
                    std::cout << it->second << std::endl;
                }
                else
                    std::cerr << " NOT FOUND!\n";
                
                c++;
            }
        }
        std::cout << "\nEnter query: ";

    }while(query != "END");
}

bool fits_parser::update_file
(std::fstream &file, size_t &hdu_no, std::string &keyarg, std::string &valarg)
{
    bool retval = false;
    file.clear();
    file.seekp(0, std::ios::beg);

    bool found = false, charStr = false, invalidFormat = false;;

    if(hdu_no > HDUS.size())
    {
        std::cerr << "INVALID HDU NUMBER PASSED! \n";
        return false;
    }

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
    
    size_t hducount = 0; 
    bool extFound = false;
    int load = 0;
    while(!file.eof() && hducount < hdu_no)
    {
        switch (load)
        {
        case 0:
            std::cout << "Searching..";
            load = 2;
            break;
        case 1:
            std::cout << "..";
            load = 2;    
            break;
        case 2:
            std::cout << "..";
            load = 3;
            break;
        case 3: 
            std::cout << "..\b\b\b\b\b\b";
            load = 1;
            break;
        default:
            break;
        }
        extFound = false;
        if(s.substr(0,8)=="XTENSION")
        {
            hducount++;
            extFound = true;
        }
        file.read(temp, 80);
        s = std::string(temp);
    }

    // if extFound is true, means the previous scanned card had the XTENSION keyword
    while(!file.eof())
    {
        switch (load)
        {
        case 0:
            std::cout << "Searching..";
            load = 2;
            break;
        case 1:
            std::cout << "..";
            load = 2;    
            break;
        case 2:
            std::cout << "..";
            load = 3;
            break;
        case 3: 
            std::cout << "..\b\b\b\b\b\b";
            load = 1;
            break;
        default:
            break;
        }
        if(s.substr(0,3) != "END")
        {
            if(s.substr(0, keyarg.size()) == keyarg)
            {
                std::cout << "FOUND in header no. " << hducount << "! Trying to update...\n";
                ull pos = file.tellp();
                file.seekp(pos - 70);
                file << valarg;
                file.seekp(pos);  // reset cursor to old position
                found = true;
                retval = true;
                break;
            }
        }
        else
        {
            break;
        }
        
        file.read(temp, 80);
        s = std::string(temp);
    }

    if(!found)
    {
        std::cerr << "KEYWORD NOT FOUND!\n";
        retval = false;
    }

    extract_cards(file);
    populate_map();
    return retval;
}