#include "JsonUtil.h"

namespace JsonUtil
{

std::string escapeString(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (char c : s)
    {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else                out += c;
    }
    return out;
}

std::string readString(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";

    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;

    std::string result;
    while (pos < json.size() && json[pos] != '"')
    {
        if (json[pos] == '\\' && pos + 1 < json.size())
        {
            pos++;
            if      (json[pos] == '"')  result += '"';
            else if (json[pos] == '\\') result += '\\';
            else if (json[pos] == 'n')  result += '\n';
            else if (json[pos] == 't')  result += '\t';
            else if (json[pos] == 'r')  result += '\r';
            else                        result += json[pos];
        }
        else
        {
            result += json[pos];
        }
        pos++;
    }
    return result;
}

int readInt(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return 0;

    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;

    std::string numStr;
    if (pos < json.size() && json[pos] == '-') { numStr += '-'; pos++; }
    while (pos < json.size() && std::isdigit(static_cast<unsigned char>(json[pos])))
        numStr += json[pos++];

    return numStr.empty() ? 0 : std::stoi(numStr);
}

std::vector<std::string> splitObjects(const std::string& json)
{
    std::vector<std::string> result;

    const std::string search = "\"items\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return result;

    pos += search.size();
    while (pos < json.size() && json[pos] != '[') pos++;
    if (pos >= json.size()) return result;
    pos++;

    int    depth = 0;
    size_t start = std::string::npos;
    bool   inStr = false;

    for (size_t i = pos; i < json.size(); i++)
    {
        const char c = json[i];

        if (c == '"' && (i == 0 || json[i - 1] != '\\'))
        {
            inStr = !inStr;
        }
        else if (!inStr)
        {
            if (c == '{')
            {
                if (depth == 0) start = i;
                depth++;
            }
            else if (c == '}')
            {
                depth--;
                if (depth == 0 && start != std::string::npos)
                {
                    result.push_back(json.substr(start, i - start + 1));
                    start = std::string::npos;
                }
            }
            else if (c == ']' && depth == 0)
            {
                break;
            }
        }
    }

    return result;
}

} // namespace JsonUtil
