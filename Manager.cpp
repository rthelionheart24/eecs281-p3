// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350
#include "Manager.h"

void Manager::read_logs(std::ifstream &in)
{
    int ID = 0;
    std::string timestamp, category, msg;

    while (std::getline(in, timestamp, '|'))
    {
        std::getline(in, category, '|');
        std::getline(in, msg);
        entries.push_back({ID++, timestamp, category, msg});
    }
    std::sort(entries.begin(), entries.end(), log_entry_comp());
    std::cout << ID << " entries read\n";

    // //Initialize the map for categories
    // for (size_t i = 0; i < entries.size(); i++)
    // {

    //     category_map[entries[i].category].push_back(i);
    // }

    // //Initialize the map for keywords
    // for (size_t i = 0; i < entries.size(); i++)
    // {
    //     std::string s = entries[i].msg;
    //     int len = 0;
    //     int start = 0;

    //     while (start + len < s.size())
    //     {

    //         if (isalnum(s[start + len]))
    //         {
    //             len++;
    //         }
    //         else
    //         {
    //             std::string temp = s.substr(start, len - 1);
    //             msg_map[temp].push_back(i);

    //             start = start + len + 1;
    //             len = 0;
    //         }
    //     }
    // }
}

void Manager::process_CMD()
{
    char CMD;
    //Read in all the commands
    do
    {
        std::cout << "% ";
        std::cin >> CMD;

        std::string temp;
        int index;

        switch (CMD)
        {
        case '#':
            std::getline(std::cin, temp);
            break;
        case 't':
        {
            std::string s1, s2;
            std::getline(std::cin, s1, '|');
            std::getline(std::cin, s2);
            t_CMD(s1, s2);
            break;
        }
        case 'm':
        {
            std::string s;
            std::getline(std::cin, s);
            m_CMD(s);
            break;
        }
        // case 'c':
        //     std::cin >> temp;
        //     c_CMD(temp);
        //     break;
        // case 'k':
        //     std::cin >> temp;
        //     k_CMD(temp);
        //     break;
        case 'a':

            std::cin >> index;
            a_CMD(index);
            break;
        case 'r':
            r_CMD();
            break;
        case 'd':
            std::cin >> index;
            d_CMD(index);
            break;

        case 'b':
            std::cin >> index;
            b_CMD(index);
            break;
        case 'e':
            std::cin >> index;
            e_CMD(index);
            break;
        case 's':
            s_CMD();
            break;
        case 'l':
            l_CMD();
            break;
        case 'g':
            g_CMD();
            break;
        case 'p':
            p_CMD();
            break;

        default:
            std::cerr << "Invalid command, please re-enter:\n";
        }

    } while (CMD != 'q');
}

void Manager::t_CMD(std::string &t1, std::string &t2)
{

    if (t1.length() != 14 || t2.length() != 14)
    {
        std::cerr << "Timestamps have invalid lengths\n";
        return;
    }

    search_results.clear();

    auto start = std::lower_bound(entries.begin(), entries.end(), t1, comp_helper());
    auto end = std::upper_bound(entries.begin(), entries.end(), t2, comp_helper());

    for (; start < end; start++)
    {
        search_results.push_back(static_cast<int>(start - entries.begin()));
    }

    std::cout << "Timestamps search: " << search_results.size() << " entries found\n";
}

void Manager::m_CMD(std::string &t)
{
    search_results.clear();

    auto start = std::lower_bound(entries.begin(), entries.end(), t, comp_helper());
    auto end = std::upper_bound(entries.begin(), entries.end(), t, comp_helper());

    for (; start < end; start++)
    {
        search_results.push_back(static_cast<int>(start - entries.begin()));
    }

    std::cout << "Timestamp search: " << search_results.size() << " entries found\n";
}

// void Manager::c_CMD(std::string &s)
// {
//     //TODO
//     search_results.clear();
// }

// void Manager::k_CMD(std::string &s)
// {
//     //TODO
//     search_results.clear();
// }

void Manager::a_CMD(int index)
{

    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::cerr << "Index out of bound for appending\n";
        return;
    }

    excerpt_list.push_back({static_cast<int>(excerpt_list.size()), index});
    std::cout << "log entry " << index << " appended\n";
}

void Manager::r_CMD()
{

    if (search_results.empty())
    {
        std::cerr << "No previous search for r\n";
        return;
    }

    for (auto &i : search_results)
    {
        excerpt_list.push_back({static_cast<int>(excerpt_list.size()), i});
    }
    std::cout << search_results.size() << " log entries appended\n";
}

void Manager::d_CMD(int index)
{
    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::cerr << "Index out of bound for deletion\n";
        return;
    }

    for (size_t i = index; i < entries.size() - 1; i++)
    {

        excerpt_list[i] = excerpt_list[i + 1];
        excerpt_list[i].ID--;
    }
    excerpt_list.pop_back();

    std::cout << "Deleted excerpt list entry " << index << "\n";
}

void Manager::b_CMD(int index)
{
    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::cerr << "Index out of bound for moving to the beginning\n";
        return;
    }

    excerpt_entry temp = excerpt_list[index];

    for (size_t i = 1; i <= static_cast<size_t>(index); i++)
    {
        excerpt_list[i] = excerpt_list[i - 1];
        excerpt_list[i].ID++;
    }
    excerpt_list[0] = temp;

    std::cout << "Moved excerpt list entry " << index << "\n";
}

void Manager::e_CMD(int index)
{
    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::cerr << "Index out of bound for moving to the end\n";
        return;
    }

    excerpt_entry temp = excerpt_list[index];

    for (size_t i = index; i < excerpt_list.size() - 1; i++)
    {
        excerpt_list[i] = excerpt_list[i + 1];
        excerpt_list[i].ID--;
    }
    excerpt_list.pop_back();
    excerpt_list.push_back(temp);

    std::cout << "Moved excerpt list entry " << index << "\n";
}

void Manager::s_CMD()
{
    if (excerpt_list.empty())
        std::cout << "excerpt list sorted\n (previously empty)\n";

    else
    {
        std::cout << "excerpt list sorted\n previous ordering:\n"
                  << excerpt_list.front().ID << "|" << entries[excerpt_list.front().index] << "\n"
                  << "...\n"
                  << excerpt_list.back().ID << "|" << entries[excerpt_list.back().index] << "\n";

        std::sort(excerpt_list.begin(), excerpt_list.end(), excerpt_entry_comp(entries));

        std::cout << "new ordering:\n"
                  << excerpt_list.front().ID << "|" << entries[excerpt_list.front().index] << "\n"
                  << "...\n"
                  << excerpt_list.back().ID << "|" << entries[excerpt_list.back().index] << "\n";
    }
}

void Manager::l_CMD()

{
    if (excerpt_list.empty())
        std::cout << "excerpt list sorted\n (previously empty)\n";
    else
    {
        std::cout << "excerpt list cleared\n previous contents: \n"
                  << excerpt_list.front().ID << "|" << entries[excerpt_list.front().index] << "\n"
                  << "...\n"
                  << excerpt_list.back().ID << "|" << entries[excerpt_list.back().index] << "\n";
        excerpt_list.clear();
    }
}

void Manager::g_CMD()
{
    if (search_results.empty())
    {
        std::cerr << "No previous search for g\n";
        return;
    }

    //A temporary sorted version of search results
    std::deque<excerpt_entry> temp;

    for (auto &i : search_results)
    {
        temp.push_back({0, i});
    }

    std::sort(temp.begin(), temp.end(), excerpt_entry_comp(entries));

    for (auto &i : temp)
    {
        std::cout << entries[i.index];
    }
}

void Manager::p_CMD()
{
    for (auto &i : excerpt_list)
    {
        std::cout << i.ID << "|" << entries[i.index];
    }
}
