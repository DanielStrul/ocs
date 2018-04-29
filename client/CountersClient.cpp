//
// CountersClient.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Source for the CountersClient class:
// - opens an udp-v6 socket
// - provides an API for:
//      sending a request to a counters server
//      receiving and displaying the server's reply
//
// This code is derived from the Boost tutorial here:
// https://www.boost.org/doc/libs/1_67_0/doc/html/boost_asio/tutorial/tutdaytime4/src.html
//
#include "CountersClient.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include "Constants.h"
#include "Logger.h"

namespace ocs
{
namespace CountersClient
{

    using boost::asio::ip::udp;
        
    // Ctor:
    // Implements the asio's server startup logic
    CountersClient::CountersClient(const Configuration& configuration, boost::asio::io_service& io_context)
     : configuration_(configuration)
     , io_context_(io_context)
     , socket_(io_context_)
     , receiver_endpoint_()
    {
        // Open a socket
        socket_.open(udp::v6());
        
        // Resolve the target hostname/service to an endpoint
        udp::resolver resolver(io_context_);
        udp::resolver::query query(udp::v6(), configuration_.hostname, configuration_.service);
        receiver_endpoint_ = *resolver.resolve(query);
        Logger(debug) << "Endpoint resolved to: " << receiver_endpoint_;
    }
        
    // getCounters():
    // - sends a request to the target server
    // - receives the server's reply (message)
    // - decode the reply (message) into a query count
    // - display the count to the console (via the logger)
    // - encapsulate the whole workflow in a try-block so that exceptions should not bubble-up to the main polling loop
    void CountersClient::getCounters()
    {
        try
        {
            Logger(debug) << "Sending a GET command to the server";
            sendCommand();
            const auto reply = receiveReply();
            const auto count = decodeCount(reply);
            Logger(info) << "Counter was successfully received, new count is: " << count;
        }
        catch (const std::exception& e)
        {
            Logger(error) << e.what();
        }
    }

    // sendCommand():
    // Sends a "GET" command to the target server
    void CountersClient::sendCommand()
    {
        const std::string command = "GET";
        socket_.send_to(boost::asio::buffer(command), receiver_endpoint_);
    }

    // receiveReply():
    // Receives a reply to a command from the target server
    std::string CountersClient::receiveReply()
    {
        std::array<char, Constants::defaultBufferSize> recv_buffer;
        udp::endpoint sender_endpoint;
        size_t len = socket_.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);
        return std::string(recv_buffer.begin(), len);
    }

    // decodeCount():
    // - decodes a "GET" reply message into a query count
    // - throws if the reply is an error message or cannot be read
    unsigned long long CountersClient::decodeCount(const std::string& reply)
    {
        if (boost::algorithm::starts_with(reply , "OK:"))
        {
            return std::strtoull(reply.c_str() + 3, nullptr, 10);
        }
        else if (boost::algorithm::starts_with(reply , "ERROR:"))
        {
            std::string msg = "The server responded with an error message: " + reply.substr(6);
            Logger(error) << msg;
            throw std::logic_error(msg);
        }
        else 
        {
            std::string msg = "Could not parse the server's response (invalid or corrupt)";
            Logger(error) << msg;
            throw std::logic_error(msg);
        }
    }

} // namespace CountersClient
} // namespace ocs
