#include "ini.h"
#include <fstream>
#include <regex>

Ini::Ini(bool caseSensitive)
    : m_content(new std::map<std::string, std::map<std::string, std::string>>)
    , m_caseSensitive(caseSensitive)
{
}

Ini::Ini(const Ini& other)
{
    m_caseSensitive = other.m_caseSensitive;
    m_content = new std::map<std::string, std::map<std::string, std::string>>(*other.m_content);
}

Ini::~Ini()
{
    delete m_content;
}


std::string Ini::get(std::string section, std::string key, std::string defaultOutput)
{
    if(!isCaseSensitive())
    {
		tolower(section);
        tolower(key);
    }

    auto sectionIter = m_content->find(section);
    if(sectionIter == m_content->end())
    {
        // adding the value to the inifile if the entry doesn't exist
        set(section, key, defaultOutput);
        return defaultOutput;
    }

    auto& sect = (*m_content)[section];
    auto paramIter = sect.find(key);
    if(paramIter == sect.end())
    {
        // adding the value to the inifile if the entry doesn't exist
        set(section, key, defaultOutput);
        return defaultOutput;
    }
    else
    {
        return sect[key];
    }
}

int Ini::get_int(std::string section, std::string key, int defaultOutput)
{
    return std::stoi(get(section, key, std::to_string(defaultOutput)));
}

double Ini::get_double(std::string section, std::string key, double defaultOutput)
{
    return std::stod(get(section, key, std::to_string(defaultOutput)));
}

void Ini::set(std::string section, std::string key, std::string value)
{
    if(!isCaseSensitive())
    {
        tolower(section);
        tolower(key);
    }

    auto sectionIter = m_content->find(section);
    if(sectionIter == m_content->end())
    {
        m_content->insert(std::make_pair(section, std::map<std::string, std::string>()));

        // Recursively continue towards the next step
        set(section, key, value);
    }
    else
    {
        auto& sect = (*m_content)[section];
        auto pairIter = sect.find(key);
        if(pairIter == sect.end())
        {
            sect.insert(std::make_pair(key, value));
        }
        else
        {
            sect[key] = value;
        }
    }
}

void Ini::set_int(std::string section, std::string key, int value)
{
    set(section, key, std::to_string(value));
}

void Ini::set_double(std::string section, std::string key, double value)
{
    set(section, key, std::to_string(value));
}

bool Ini::isCaseSensitive()
{
    return m_caseSensitive;
}

Ini Ini::fromFile(std::string path)
{
    std::ifstream file(path, std::ifstream::in);
    Ini ini;

    if(!file.good())
    {
        file.close();
        return ini;
    }

    std::string currentSection = "";

    auto sectionRegex = std::regex("\\[([a-zA-Z_0-9]+)\\]\\s*"); // [AnY_text9001]
    auto paramRegex   = std::regex("([a-zA-Z0-9_]+)=(.*)");      // any_PROP09=anYthing 9!
    std::smatch matches;

    for(std::string line; file.good() && getline(file, line);)
    {
        if(std::regex_search(line, matches, sectionRegex))
        {
            currentSection = matches[1]; // Group 1 is the () first outer subgroup
        }
        else if(std::regex_search(line, matches,paramRegex))
        {
            if(currentSection.length() > 0)
            {
                ini.set(currentSection, matches[1], matches[2]);
            }
        }
    }

    file.close();
    return ini;
}

bool Ini::save(std::string path)
{
    std::ofstream file(path, std::ofstream::trunc);
    if(!file.good())
    {
        file.close();
        return false;
    }

    for(auto const& section : *m_content)
    {
        file << "[" << section.first << ']' << std::endl;

        for(auto const& param : section.second)
        {
            file << param.first << '=' << param.second << std::endl;
        }

        file << std::endl;
    }

    file.close();
    return true;
}

void Ini::tolower(std::string& target) const
{
	    for(char& c : target)
    {
        if(c >= 'A' && c <= 'Z')
        {
            c -= ('A' - 'a');
        }
    }
}

bool Ini::startsWith(const std::string& input, const char startsWith) const
{
	return input.length() > 0 && input[0] == startsWith;
}
