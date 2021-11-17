#include <debug.h>
#include <cstdio>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

bool __is_addr2line_available = false;
const char * __path_to_executable = nullptr;

std::string __exec_cmd(const std::string& cmd)
{
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}

std::string __get_addr(const std::string& input)
{
    std::string output;
    bool start = false;

    for (auto i : input)
    {
        if (i == '[')
        {
            start = true;
            continue;
        }

        if (start)
        {
            if (i != ']')
            {
                output += i;
            }
            else
            {
                break;
            }
        }
    }

    return output;
}

std::vector < std::string > str2lines(const std::string& input)
{
    std::string line;
    std::vector < std::string > ret;

    for (auto i : input)
    {
        if (i == '\n')
        {
            ret.emplace_back(line);
            line.clear();
            continue;
        }

        line += i;
    }

    return ret;
}

std::string __clean_addr2line_output(const std::string& input)
{
    std::vector < std::string > line = str2lines(input);
    for (auto i : line)
    {
        if (i[0] == '?' && i[1] == '?')
        {
            continue;
        }

        return i;
    }

    return "(no specific information)";
}

void __check_addr2line()
{
    __is_addr2line_available = (system("addr2line --help >/dev/null 2>/dev/null") == 0);
}
