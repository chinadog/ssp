#ifndef INIREADER_H
#define INIREADER_H


#include <map>
#include <string>

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader
{
public:
    // Construct INIReader and parse given filename. See ini.h for more info
    // about the parsing.
    INIReader(const std::string& filename);

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int parseError() const;

    // Get a string value from INI file, returning default_value if not found.
    std::string get(const std::string& section, const std::string& name,
                    const std::string& default_value) const;

    // Get an integer (long) value from INI file, returning default_value if
    // not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
    long getInteger(const std::string& section, const std::string& name, long default_value) const;

    // Get a real (floating point double) value from INI file, returning
    // default_value if not found or not a valid floating point value
    // according to strtod().
    double getReal(const std::string& section, const std::string& name, double default_value) const;

    // Get a boolean value from INI file, returning default_value if not found or if
    // not a valid true/false value. Valid true values are "true", "yes", "on", "1",
    // and valid false values are "false", "no", "off", "0" (not case sensitive).
    bool getBoolean(const std::string& section, const std::string& name, bool default_value) const;

private:
    int m_error;
    std::map<std::string, std::string> m_values;

    static std::string makeKey(const std::string& section, const std::string& name);
    static int valueHandler(void* user, const char* section, const char* name,
                            const char* value);
};


#endif // INIREADER_H
