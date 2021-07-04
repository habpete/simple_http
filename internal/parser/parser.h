#pragma once

#include "../spec/spec.h"

class cURLparser {
public:
    cURLparser(std::string& filePath);
    ~cURLparser();
private:
    std::string m_FilePath;
public:
    bool FileIsExist();
    Query* Execute();
private:
    queryTypes getQueryType(std::string& tmpLine);
    std::string getHost(std::string& tmpLine);
    std::string getHeaders(std::string& tmpLine);
    std::string getBody(std::string& tmpLine);
};