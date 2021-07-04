#include "parser.h"

#include <filesystem>
#include <fstream>
#include <regex>

cURLparser::cURLparser(std::string& filePath) {
    this->m_FilePath = filePath;
}

cURLparser::~cURLparser() {}

// public methods for external use
bool cURLparser::FileIsExist() {
    auto ex = std::filesystem::exists(this->m_FilePath);
    return ex;
}

Query* cURLparser::Execute() {
    std::ifstream curlFS;
    curlFS.open(this->m_FilePath);

    Query* qParams = new Query();

    std::string tmpLine;
    while (std::getline(curlFS, tmpLine)) {
        queryTypes qType = getQueryType(tmpLine);
        if (qType != queryTypes::None) {
            qParams->qType = qType;
        }

        std::string host = getHost(tmpLine);
        if (host.size() > 0) {
            qParams->host = host;
        }

        std::string header = getHeaders(tmpLine);
        if (header.size() > 0) {
            qParams->headers.push_back(header);
        }

        std::string body = getBody(tmpLine);
        if (body.size() > 0) {
            qParams->body.push_back(body);
        }
    }

    return qParams;
}

// extractors of query fields
queryTypes cURLparser::getQueryType(std::string& tmpLine) {
    std::regex re("curl (.*)");
    if (!std::regex_match(tmpLine, re)) {
        return None;
    }

    if (tmpLine.find(" POST ") != std::string::npos) {
        return Post;
    } else if (tmpLine.find(" GET ") != std::string::npos) {
        return Get;
    }

    return None;
}
std::string cURLparser::getHost(std::string& tmpLine) {
    std::regex re("curl (.*) '(.*)'");
    std::smatch matches;

    if (!std::regex_search(tmpLine, matches, re)) {
        return "";
    }

    if (matches.size() < 3) {
        return "";
    }

    return matches.str(2);
}
std::string cURLparser::getHeaders(std::string& tmpLine) {
    std::regex re("-H (.*)");
    std::smatch matches;

    if (!std::regex_search(tmpLine, matches, re)) {
        return "";
    }

    if (matches.size() < 2) {
        return "";
    }

    return matches.str(1);
}
std::string cURLparser::getBody(std::string& tmpLine) {
    return "";
}