#include "sender.h"
#include "../spec/spec.h"

#include <ostream>
#include <regex>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace senderHelpers {
    std::string getQueryType (queryTypes qType) {
        std::string result;

        switch (qType) {
            case Get:
                result = "GET";
                break;
            case Post:
                result = "POST";
                break;
            default:
                break;
        }

        return result;
    }
    std::string getIpAddress (std::string& host) {
        std::regex re("(.*)/");
        std::smatch matches;

        if (!std::regex_search(host, matches, re)) {
            return host;
        }

        std::string tmpHost = matches.str(1);
        re = "(.*):";
        if (!std::regex_search(host, matches, re)) {
            return tmpHost;
        }

        return matches.str(1);
    }
    std::string getPort (std::string& host) {
        std::regex re("(.*):(.*)");
        std::smatch matches;

        if (!std::regex_search(host, matches, re)) {
            return "";
        }

        if (matches.size() < 3) {
            return "";
        }

        std::string port = matches.str(2);
        size_t uriStartPos = port.find("/");
        if (uriStartPos == std::string::npos) {
            return port;
        }

        return port.substr(0, uriStartPos);
    }
    std::string getUri (std::string& host) {
        std::regex re("/(.*)");
        std::smatch matches;

        if (!std::regex_search(host, matches, re)) {
            return "";
        }

        if (matches.size() < 2) {
            return "";
        }

        return matches.str(1);
    }
    std::string getDstHost (std::string& host) {
        std::string ipAddr = getIpAddress(host);
        std::string port = getPort(host);
        if (port.compare("80")) {
            return ipAddr + ":" + port;
        }
        return ipAddr;
    }
    void fillRequestStream(std::ostream& refReqStream, Query*& query) {
        refReqStream << getQueryType(query->qType) << " " << getUri(query->host) << " HTTP/1.1\r\n";
        refReqStream << "Host: " << getDstHost(query->host) << "\r\n";
        refReqStream << "Accept: */*\r\n";
        refReqStream << "Connection: close\r\n\r\n";
    }
    std::string getResponseFromStream(boost::asio::streambuf& respStream) {
        return "";
    }
};

std::string Sender::Execute (Query*& query) {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    std::string ipAddr = senderHelpers::getIpAddress(query->host);
    std::string port = senderHelpers::getPort(query->host);
    tcp::resolver::query rslvQuery(ipAddr, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(rslvQuery);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    boost::asio::streambuf request;
    std::ostream reqStream(&request);
    boost::asio::write(socket, request);

    boost::asio::streambuf response;
    std::string resp = senderHelpers::getResponseFromStream(response);

    return resp;
}