#include "sender/sender.h"
#include "spec/spec.h"

#include <iostream>
#include <string>

const std::string example = "mycurl -m POST http://mysite.com \"body\"";

queryTypes getQueryType (char*& queryType) {
    if (std::string(queryType) == "post") {
        return Post;
    } else if (std::string(queryType) == "get") {
        return Get;
    }
    return None;
}

int main (int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "empty string. Need " << example << std::endl;
        return -1;
    }

    Query* qParams = new Query();
    qParams->qType = getQueryType(argv[3]);
    qParams->host = argv[4];
    
    if (argc == 5) {
        qParams->body = argv[5];
    }

    std::string response;
    Send(qParams, response);

    return 0;
}