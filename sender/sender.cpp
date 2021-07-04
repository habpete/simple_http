#include "sender.h"

#include <boost/asio.hpp>
#include <ostream>
#include <regex>

using boost::asio::ip::tcp;

const std::string tcpPort = "80";

namespace senderUtils {
    std::string getQueryTypeString (queryTypes qType) {
        std::string result;

        switch (qType) {
            case Get:
                result = "GET";
                break;
            case Post:
                result = "POST";
                break;
            case None:
                break;
        }

        return result;
    }
    std::string getUri (std::string& host) {
        std::string result;
        std::regex re;
        size_t matchIdx = 0;

        if (host.compare("://")) {
            re = "://(.*)/(.*)";
            matchIdx = 2;
        } else {
            re = "/(.*)";
            matchIdx = 1;
        }

        std::smatch matches;
        if (!std::regex_search(host, matches, re)) {
            return "";
        }

        if (matches.size() < matchIdx) {
            return "";
        }

        return matches.str(matchIdx);
    }
    std::string getHostString(std::string& host) {
        std::regex re("://(.*)/");
        std::smatch matches;

        if (!std::regex_search(host, matches, re)) {
            return host;
        }

        size_t position = matches.position(1);

        return host.substr(0, position + matches.str(1).size());
    }
};

namespace senderHelpers {
    void fillRequestStream(Query*& query, std::ostream& refReqStream) {
        refReqStream << senderUtils::getQueryTypeString(query->qType) << " " << senderUtils::getUri(query->host) << "HTTP/1.1\r\n";
        refReqStream << "Host: " << "\r\n";
        refReqStream << "Transfer-Encoding: chunked\r\n";
        refReqStream << "Connection: close\r\n";

        if (query->qType == Get) {
            return;
        }

        refReqStream << query->body << "\r\n\r\n";
    }
    void getResponseString(boost::asio::streambuf& responseStream, std::string& outResponseString) {}
};

void Send(Query*& query, std::string& outResponse) {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query rslvQuery(senderUtils::getHostString(query->host), tcpPort);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(rslvQuery);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    boost::asio::streambuf request;
    std::ostream reqStream(&request);
    senderHelpers::fillRequestStream(query, reqStream);
    boost::asio::write(socket, request);

    boost::asio::streambuf response;
    senderHelpers::getResponseString(response, outResponse);
}