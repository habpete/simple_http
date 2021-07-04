#include "service.h"

#include "parser/parser.h"
#include "sender/sender.h"

std::string Execute(std::string& pathToCURL) {
    cURLparser parser(pathToCURL);
    if (!parser.FileIsExist()) {
        return "";
    }
    Query* qParams = parser.Execute();

    std::string resp = Sender::Execute(qParams);

    delete qParams;

    return resp;
}