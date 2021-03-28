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

    //Initialize the map for categories
    for (size_t i = 0; i < entries.size(); i++)
    {
        std::string s = entries[i].category;
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        category_map[s].push_back(static_cast<int>(i));
    }

    //Initialize order vector
    entries_order.resize(entries.size());

    for (size_t i = 0; i < entries.size(); i++)
    {
        entries_order[entries[i].ID] = static_cast<int>(i);
    }

    //Initialize the map for keywords
    for (size_t i = 0; i < entries.size(); i++)
    {
        std::string s1 = entries[i].msg;
        std::string s2 = entries[i].category;

        std::transform(s1.begin(), s1.end(), s1.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(s2.begin(), s2.end(), s2.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        int len = 0;
        int start = 0;

        while (start + len <= static_cast<int>(s1.size()))
        {

            if (isalnum(s1[start + len]))
                len++;

            else
            {
                std::string temp = s1.substr(start, len);
                keyword_map[temp].push_back(static_cast<int>(i));

                start = start + len + 1;
                len = 0;
            }
        }

        len = 0;
        start = 0;

        while (start + len <= static_cast<int>(s2.size()))
        {

            if (isalnum(s2[start + len]))
                len++;

            else
            {
                std::string temp = s2.substr(start, len);
                keyword_map[temp].push_back(static_cast<int>(i));

                start = start + len + 1;
                len = 0;
            }
        }
    }

    for (auto &i : keyword_map)
    {
        std::sort(i.second.begin(), i.second.end());
        auto it = std::unique(i.second.begin(), i.second.end());
        i.second.resize(it - i.second.begin());
    }

    search_results.push_back(-1);
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
        case 'q':
            return;
            break;
        case 't':
        {

            std::string s1, s2;

            std::getline(std::cin, s1, '|');
            s1 = s1.substr(1, 14);

            std::cin >> s2;

            t_CMD(s1, s2);
            break;
        }
        case 'm':
        {
            std::string s;
            std::cin >> s;
            m_CMD(s);
            break;
        }
        case 'c':
            std::getline(std::cin, temp);
            temp = temp.substr(1);
            c_CMD(temp);
            break;
        case 'k':
            std::getline(std::cin, temp);
            temp = temp.substr(1);
            k_CMD(temp);
            break;
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

    auto start = std::lower_bound(entries.begin(), entries.end(), t1, lower_helper());
    auto end = std::upper_bound(entries.begin(), entries.end(), t2, upper_helper());

    for (; start < end; start++)
    {
        search_results.push_back(static_cast<int>(start - entries.begin()));
    }

    std::cout << "Timestamps search: " << search_results.size() << " entries found\n";
}

void Manager::m_CMD(std::string &t)
{
    if (t.length() != 14)
    {
        std::cerr << "Timestamp has invalid length\n";
        return;
    }

    search_results.clear();

    auto start = std::lower_bound(entries.begin(), entries.end(), t, lower_helper());
    auto end = std::upper_bound(entries.begin(), entries.end(), t, upper_helper());

    for (; start < end; start++)
    {
        search_results.push_back(static_cast<int>(start - entries.begin()));
    }

    std::cout << "Timestamp search: " << search_results.size() << " entries found\n";
}

void Manager::c_CMD(std::string &s)
{

    search_results.clear();

    std::string dummy = s;
    std::transform(dummy.begin(), dummy.end(), dummy.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    search_results = category_map[dummy];

    std::cout << "Category search: " << search_results.size() << " entries found\n";

    search_results = category_map[dummy];
}

void Manager::k_CMD(std::string &s)
{

    search_results.clear();

    std::string dummy = s;
    std::transform(dummy.begin(), dummy.end(), dummy.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    int start = 0;
    int len = 0;

    while (start + len <= static_cast<int>(dummy.size()))
    {

        if (isalnum(dummy[start + len]))
            len++;

        else
        {
            if (len == 0)
            {
                start++;
                continue;
            }

            std::string temp = dummy.substr(start, len);

            if (search_results.empty())
                search_results = keyword_map[temp];
            else
            {

                auto it = std::set_intersection(search_results.begin(), search_results.end(),
                                                keyword_map[temp].begin(), keyword_map[temp].end(), search_results.begin());
                search_results.resize(it - search_results.begin());

                if (search_results.size() == 0)
                    break;
            }

            start = start + len + 1;
            len = 0;
        }
    }

    std::cout << "Keyword search: " << search_results.size() << " entries found\n";
}

void Manager::a_CMD(int index)
{

    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::cerr << "Index out of bound for appending\n";
        return;
    }

    excerpt_list.push_back(entries_order[index]);
    std::cout << "log entry " << index << " appended\n";
}

void Manager::r_CMD()
{

    if (search_results.front() == -1)
    {
        std::cerr << "No previous search for r\n";
        return;
    }

    for (auto &i : search_results)
    {
        excerpt_list.push_back(i);
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

    for (size_t i = index; i < excerpt_list.size() - 1; i++)
    {

        excerpt_list[i] = excerpt_list[i + 1];
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

    int temp = excerpt_list[index];

    for (size_t i = static_cast<size_t>(index); i >= 1; i--)
    {
        excerpt_list[i] = excerpt_list[i - 1];
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

    int temp = excerpt_list[index];

    for (size_t i = index; i < excerpt_list.size(); i++)
    {
        excerpt_list[i] = excerpt_list[i + 1];
    }
    excerpt_list.pop_back();
    excerpt_list.push_back(temp);

    std::cout << "Moved excerpt list entry " << index << "\n";
}

void Manager::s_CMD()
{
    if (excerpt_list.empty())
        std::cout << "excerpt list sorted\n(previously empty)\n";

    else
    {
        std::cout << "excerpt list sorted\nprevious ordering:\n"
                  << 0 << "|" << entries[excerpt_list.front()]
                  << "...\n"
                  << excerpt_list.size() - 1 << "|" << entries[excerpt_list.back()];

        std::sort(excerpt_list.begin(), excerpt_list.end());

        std::cout << "new ordering:\n"
                  << 0 << "|" << entries[excerpt_list.front()]
                  << "...\n"
                  << excerpt_list.size() - 1 << "|" << entries[excerpt_list.back()];
    }
}

void Manager::l_CMD()

{
    if (excerpt_list.empty())
        std::cout << "excerpt list cleared\n(previously empty)\n";
    else
    {
        std::cout << "excerpt list cleared\nprevious contents:\n"
                  << 0 << "|" << entries[excerpt_list.front()]
                  << "...\n"
                  << excerpt_list.size() - 1 << "|" << entries[excerpt_list.back()];
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
    // std::deque<int> temp;

    for (auto &i : search_results)
    {
        std::cout << entries[i];
        // temp.push_back(i);
    }

    // std::sort(temp.begin(), temp.end(), excerpt_entry_comp(entries));

    // for (auto &i : temp)
    // {
    //     std::cout << entries[i];
    // }
}

void Manager::p_CMD()
{
    for (size_t i = 0; i < excerpt_list.size(); i++)
    {
        std::cout << i << "|" << entries[excerpt_list[i]];
    }
}
