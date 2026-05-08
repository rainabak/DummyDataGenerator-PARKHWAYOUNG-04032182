#pragma once
#include <string>
#include <vector>

namespace JsonUtil
{
    // 문자열 값 내 특수문자를 JSON 규격에 맞게 이스케이프한다.
    std::string escapeString(const std::string& s);

    // JSON 문자열에서 key에 해당하는 문자열 값을 추출한다.
    std::string readString(const std::string& json, const std::string& key);

    // JSON 문자열에서 key에 해당하는 정수 값을 추출한다.
    int readInt(const std::string& json, const std::string& key);

    // JSON 최상위 "items" 배열에서 개별 객체 문자열 목록을 추출한다.
    std::vector<std::string> splitObjects(const std::string& json);
}
