#ifndef INI_H
#define INI_H

#include <string>
#include <map>

class Ini
{
public:
    Ini(bool caseSensitive = false);
    Ini(const Ini& other);
    ~Ini();


    std::string get(std::string section, std::string key, std::string defaultOutput = "");
    int         get_int(std::string section, std::string key, int defaultOutput = 0);
    double      get_double(std::string section, std::string key, double defaultOutput = 0.0);

    void set(std::string section, std::string key, std::string value);
    void set_int(std::string section, std::string key, int value);
    void set_double(std::string section, std::string key, double value);

    inline bool isCaseSensitive();

    static Ini fromFile(std::string path);
    bool save(std::string path);

private:
    std::map<std::string, std::map<std::string, std::string>>* m_content;
    bool m_caseSensitive;

	void tolower(std::string& target) const;
	bool startsWith(const std::string& input, const char startsWith) const;
};

#endif // INI_H
