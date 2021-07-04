#pragma once

#include <string>

enum queryTypes {
    None = 0,
    Get,
    Post,
};

struct Query {
    queryTypes qType;
    std::string host;
    std::string body;
};