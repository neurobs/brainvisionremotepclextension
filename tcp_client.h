#pragma once

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

//
// This class manages socket timeouts by applying the concept of a deadline.
// Each asynchronous operation is given a deadline by which it must complete.
// Deadlines are enforced by an "actor" that persists for the lifetime of the
// client object:
//
//  +----------------+
//  |                |     
//  | check_deadline |<---+
//  |                |    |
//  +----------------+    | async_wait()
//              |         |
//              +---------+
//
// If the actor determines that the deadline has expired, the socket is closed
// and any outstanding operations are consequently cancelled. The socket
// operations themselves use boost::lambda function objects as completion
// handlers. For a given socket operation, the client object runs the
// io_service to block thread execution until the actor completes.
//
class TCP_client
{
public:
	TCP_client();

	void connect(const std::string& host, const std::string& service,
		boost::posix_time::time_duration timeout);

	std::string read_line(boost::posix_time::time_duration timeout);

	void write(const std::string& line,
		boost::posix_time::time_duration timeout);

private:
	void check_deadline();

	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::deadline_timer deadline_;
	boost::asio::streambuf input_buffer_;
};
