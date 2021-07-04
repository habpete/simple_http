#pragma once

#include <string>
#include <list>

enum queryTypes {
    None = 0,
    Get,
    Post,
};

struct Query {
    queryTypes qType;
    std::string host;
    std::list<std::string> headers;
    std::list<std::string> body;
};