// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
#ifndef MANAGER
#define MANAGER

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <map>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct log_entry
{
    int ID;
    std::string timestamp;
    std::string category;
    std::string msg;

    friend std::ostream &operator<<(std::ostream &os, const log_entry &e)
    {
        os << e.ID << "|" << e.timestamp << "|" << e.category << "|" << e.msg << "\n";

        return os;
    }
};

struct excerpt_entry
{
    int ID;
    int index;
};

class log_entry_comp
{
public:
    bool operator()(const log_entry &e1, const log_entry &e2) const
    {
        std::string temp1 = e1.timestamp;
        std::string temp2 = e2.timestamp;

        temp1.erase(std::remove(temp1.begin(), temp1.end(), ':'), temp1.end());
        temp2.erase(std::remove(temp2.begin(), temp2.end(), ':'), temp2.end());

        int64_t converted1 = std::stol(temp1);
        int64_t converted2 = std::stol(temp2);

        if (converted1 == converted2)
        {
            std::string s1 = e1.category, s2 = e2.category;

            if (strcasecmp(s1.c_str(), s2.c_str()) == 0)
            {
                return e1.ID < e2.ID;
            }
            return strcasecmp(s1.c_str(), s2.c_str()) < 0;
        }

        return converted1 < converted2;
    }
};

class excerpt_entry_comp
{
private:
    std::vector<log_entry> entries;

public:
    excerpt_entry_comp(std::vector<log_entry> &entries_in) : entries(entries_in) {}

    bool operator()(const int e1, const int e2) const
    {
        std::string temp1 = entries[e1].timestamp;
        std::string temp2 = entries[e2].timestamp;

        temp1.erase(std::remove(temp1.begin(), temp1.end(), ':'), temp1.end());
        temp2.erase(std::remove(temp2.begin(), temp2.end(), ':'), temp2.end());

        int64_t converted1 = std::stol(temp1);
        int64_t converted2 = std::stol(temp2);

        if (converted1 == converted2)
        {
            std::string s1 = entries[e1].category, s2 = entries[e2].category;

            // std::transform(s1.begin(),
            //                s1.end(),
            //                s1.begin(),
            //                [](unsigned char c) { return std::tolower(c); });

            // std::transform(s2.begin(),
            //                s2.end(),
            //                s2.begin(),
            //                [](unsigned char c) { return std::tolower(c); });

            if (strcasecmp(s1.c_str(), s2.c_str()) == 0)
            {
                return entries[e1].ID < entries[e2].ID;
            }
            return strcasecmp(s1.c_str(), s2.c_str()) < 0;
        }

        return converted1 < converted2;
    }
};

class lower_helper
{
public:
    bool operator()(const log_entry &e, const std::string &s) const //lower
    {
        std::string temp1 = e.timestamp;
        std::string temp2 = s;

        temp1.erase(std::remove(temp1.begin(), temp1.end(), ':'), temp1.end());
        temp2.erase(std::remove(temp2.begin(), temp2.end(), ':'), temp2.end());

        int64_t converted1 = std::stol(temp1);
        int64_t converted2 = std::stol(temp2);

        return converted1 < converted2;
    }
};

class upper_helper
{
public:
    bool operator()(const std::string &s, const log_entry &e) const //upper
    {

        std::string temp1 = s;
        std::string temp2 = e.timestamp;

        temp1.erase(std::remove(temp1.begin(), temp1.end(), ':'), temp1.end());
        temp2.erase(std::remove(temp2.begin(), temp2.end(), ':'), temp2.end());

        int64_t converted1 = std::stol(temp1);
        int64_t converted2 = std::stol(temp2);

        return converted1 < converted2;
    }
};

class Manager
{
private:
    std::vector<log_entry> entries;
    std::vector<int> entries_order;

    std::vector<int> search_results;

    std::deque<int> excerpt_list;

    std::unordered_map<std::string, std::vector<int>> category_map;

    std::unordered_map<std::string, std::vector<int>> keyword_map;

    bool searched = false;

public:
    void read_logs(std::ifstream &in);

    void process_CMD();

    //Timestamps search
    void t_CMD(std::string &t1, std::string &t2);

    //Matching search
    void m_CMD(std::string &t);

    //Category search
    void c_CMD(std::string &s);

    //Keywords search
    void k_CMD(std::string &s);

    //Append log entry
    void a_CMD(int index);

    //Append search results
    void r_CMD();

    //Delete excerpt entry
    void d_CMD(int index);

    //Move to beginning
    void b_CMD(int index);

    //Move to end
    void e_CMD(int index);

    //Sort excerpt list
    void s_CMD();

    //Clear excerpt list
    void l_CMD();

    //Print most recent search results
    void g_CMD();

    //Print excerpt list
    void p_CMD();
};

#endif