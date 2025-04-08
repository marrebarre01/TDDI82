#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <map>
#include <iomanip>
#include <unordered_map>

std::vector<std::string> read_file(std::string const& filename)
{
    std::ifstream file{filename};
    std::vector<std::string> text{};

    copy(std::istream_iterator<std::string>{ file },
         std::istream_iterator<std::string>{ },
         std::back_inserter(text));

    file.close();

    return text;
}

std::vector<std::string> get_args(int argc, char* argv[])
{
    std::vector<std::string> arguments{};
    for (size_t i{}; i < argc; ++i)
    {
        if (!(i == 0 || i == 1))
        {
            arguments.push_back(argv[i]);
        }
    }
    return arguments;
}

void print(std::vector<std::string> const& text)
{
    std::copy(text.begin(), text.end(),
              std::ostream_iterator<std::string>{ std::cout, " "});
    std::cout << std::endl;
}

std::vector<std::string> get_flags(std::vector<std::string> const& args)
{
    std::string flag{};
    std::vector<std::string> flags{};
    
    for (std::string const& arg : args)
    {
        auto it = std::find(arg.begin(), arg.end(), '=');
        std::copy(arg.begin(), it, std::back_inserter(flag));
        flags.push_back(flag);
        ++it;
        flag = "";
    }

    return flags;
}

std::vector<std::string> get_parameters(std::vector<std::string> const& args)
{
    std::string parameter{};
    std::vector<std::string> parameters{};
    
    for (std::string const& arg : args)
    {
        auto it = std::find(arg.begin(), arg.end(), '=');
        ++it;
        std::copy(it, arg.end(), std::back_inserter(parameter));
        parameters.push_back(parameter);
        parameter = "";
    }

    return parameters;
}

void remove(std::string const& word, std::vector<std::string> & text)
{
    auto it = std::remove(text.begin(), text.end(), word);
    text.erase(it, text.end());
}

void replace(std::string const& words, std::vector<std::string> & text)
{
    auto it = words.find('+');
    std::replace(text.begin(), text.end(), words.substr(0, it), words.substr(it + 1));
}

std::map<std::string, int> get_frequency(std::vector<std::string> const& text)
{
    std::map<std::string, int> tmp{};
    std::vector<std::string> unique{};

    // Gör copy till ny vector med all unika ord
    std::unique_copy(text.begin(), text.end(), std::back_inserter(unique));

    // Räkna antal av varje ord till en int vector
    for (auto item : unique)
    {
        tmp[item] = std::count(text.begin(), text.end(), item);
    }
    return tmp;
}

void print_frequency(std::map<std::string, int> const& items, 
    std::vector<std::string> const& text)
{
    int max_length
    {
        (*std::max_element(text.begin(), text.end(),
        [](std::string const& lhs, std::string const& rhs)
        {
        return lhs.length() < rhs.length();
        })).length()
    };

    // Omvandla map till vektor för att kunna sortera
    /*
        Vi försökte först använda oss av en unordered_map men fastande
        AI gav oss tipset att skapa en vektor med pairs och därefter sortera
        den med hjälp av andra värdet i paret.
    */
    std::vector<std::pair<std::string, int>> sorted_items(items.begin(), items.end());

    std::sort(sorted_items.begin(), sorted_items.end(),
    [](std::pair<std::string, int> const& lhs, std::pair<std::string, int> const& rhs)
    {
        return lhs.second < rhs.second;
    });

    std::reverse(sorted_items.begin(), sorted_items.end());

    std::for_each(sorted_items.begin(), sorted_items.end(),
    [&max_length](const std::pair<const std::string, int>& element) 
    {
        std::cout << std::right << std::setw(max_length) << element.first << " " ;
        std::cout << element.second << std::endl;
    });
}


void print_table(std::map<std::string, int> const& items,
                 std::vector<std::string> const& text)
{
    int max_length
    {
        (*std::max_element(text.begin(), text.end(),
        [](std::string const& lhs, std::string const& rhs)
        {
            return lhs.length() < rhs.length();
        })).length()
    };

    std::for_each(items.begin(), items.end(),
    [&max_length](const std::pair<const std::string, int>& element) 
    {
        std::cout << std::left << std::setw(max_length) << element.first << " " ;
        std::cout << element.second << std::endl;
    });
}

using namespace std;

int main(int argc, char* argv[])
{
    vector<string> text      { read_file(argv[1]) };
    vector<string> arguments { get_args(argc, argv) };
    vector<string> flags     { get_flags(arguments) };
    vector<string> parameters{ get_parameters(arguments) };

    for (size_t i{}; i < flags.size(); ++i)
    {
        if (flags.at(i) == "--print")
        {
            print(text);
        }
        else if (flags.at(i) == "--remove")
        {
            remove(parameters.at(i), text);
        }
        else if (flags.at(i) == "--substitute")
        {
            replace(parameters.at(i), text);
        }
        else if (flags.at(i) == "--frequency")
        {
            print_frequency(get_frequency(text), text);
        }
        else if (flags.at(i) == "--table")
        {
            print_table(get_frequency(text), text);
        }
    }

     
    return 0;
}
