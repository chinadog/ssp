
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "Ini.h"
#include "Inireader.h"

using std::string;

INIReader::INIReader(const string& filename)
{
    m_error = ini_parse(filename.c_str(), valueHandler, this);
}

int INIReader::parseError() const
{
    return m_error;
}

string INIReader::get(const string& section, const string& name, const string& default_value) const
{
    string key = makeKey(section, name);
    // Use _values.find() here instead of _values.at() to support pre C++11 compilers
    return m_values.count(key) ? m_values.find(key)->second : default_value;
}

long INIReader::getInteger(const string& section, const string& name, long default_value) const
{
    string valstr = get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::getReal(const string& section, const string& name, double default_value) const
{
    string valstr = get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::getBoolean(const string& section, const string& name, bool default_value) const
{
    string valstr = get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

string INIReader::makeKey(const string& section, const string& name)
{
    string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int INIReader::valueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    INIReader* reader = (INIReader*)user;
    string key = makeKey(section, name);
    if (reader->m_values[key].size() > 0)
        reader->m_values[key] += "\n";
    reader->m_values[key] += value;
    return 1;
}
