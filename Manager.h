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

struct log_entry
{
    int ID;
    std::string timestamp;
    std::string category;
    std::string msg;
};

struct excerpt_entry
{
    int ID;
    int index;
};

std::ostream &operator<<(std::ostream &os, const log_entry &e)
{
    os << e.ID << "|" << e.timestamp << "|" << e.category << "|" << e.msg << "\n";

    return os;
}

// std::ostream &operator<<(std::ostream &os, const excerpt_entry &e)
// {
//     os << e.ID << "|"
//        << entries[e.index].ID << "|"
//        << e.log->timestamp << "|"
//        << e.log->category << "|"
//        << e.log->msg << "\n";
//     return os;
// }

class log_entry_comp
{
public:
    bool operator()(const log_entry &e1, const log_entry &e2) const
    {
        if (e1.timestamp == e2.timestamp)
            return e1.ID < e2.ID;
        return e1.timestamp < e2.timestamp;
    }
};

class excerpt_entry_comp
{
private:
    std::vector<log_entry> entries;

public:
    excerpt_entry_comp(std::vector<log_entry> &entries_in) : entries(entries_in) {}

    bool operator()(const excerpt_entry &e1, const excerpt_entry &e2)
    {

        if (entries[e1.index].timestamp == entries[e2.index].timestamp)
        {

            std::string s1 = entries[e1.index].category, s2 = entries[e2.index].category;

            std::transform(s1.begin(),
                           s1.end(),
                           s1.begin(),
                           [](char c) { return std::tolower(c); });

            std::transform(s2.begin(),
                           s2.end(),
                           s2.begin(),
                           [](char c) { return std::tolower(c); });

            if (s1 == s2)
            {
                return entries[e1.index].ID < entries[e2.index].ID;
            }
            return s1 <= s2;
        }

        return entries[e1.index].timestamp < entries[e2.index].timestamp;
    }
};

class Manager
{
private:
    std::vector<log_entry> entries;

    std::vector<int> search_results;

    std::deque<excerpt_entry> excerpt_list;

    int last_excerpt;

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

    //Delete log entry
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