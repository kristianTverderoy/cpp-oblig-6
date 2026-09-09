#include <boost/asio.hpp>
#include <iostream>
#include <string>

class Server {
private:
  class Connection {
  public:
    boost::asio::ip::tcp::socket socket;
    Connection(boost::asio::io_context &io_context) : socket(io_context) {}
  };

  boost::asio::io_context io_context;

  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;

  void handle_request(std::shared_ptr<Connection> connection) {
    auto read_buffer = std::make_shared<boost::asio::streambuf>();
    // Read from client until newline ("\r\n")
    async_read_until(
        connection->socket, *read_buffer, "\r\n",
        [this, connection, read_buffer](const boost::system::error_code &ec,
                                        size_t) {
          // If not error:
          if (!ec) {
            // Retrieve message from client as string:
            std::istream read_stream(read_buffer.get());
            std::string message;
            getline(read_stream, message);
            if (!message.empty()) {
              message.pop_back(); // Remove "\r" at the end of message
            }

            std::cout << "Client connected: " << std::endl;

            auto write_buffer = std::make_shared<boost::asio::streambuf>();
            std::ostream write_stream(write_buffer.get());

            message.erase(message.find_last_not_of("\r\n") + 1);
            size_t start = message.find(' ') + 1;
            size_t end = message.find(' ', start);
            std::string path = message.substr(start, end - start);
            std::string displayMessage;
            if (path == "/") {
              displayMessage = "Dette er hovudsiden";
              write_stream << "HTTP/1.1 200 OK\r\n";
              write_stream << "Content-Type: text/plain\r\n";
              write_stream << "Content-Length:" +
                                  std::to_string(displayMessage.length()) +
                                  "\r\n";
              write_stream << "\r\n";
              write_stream << displayMessage;
            } else if (path == "/en_side") {
              displayMessage = "Dette er en side";
              write_stream << "HTTP/1.1 200 OK\r\n";
              write_stream << "Content-Type: text/plain\r\n";
              write_stream << "Content-Length: " +
                                  std::to_string(displayMessage.length()) +
                                  "\r\n";
              write_stream << "\r\n";
              write_stream << "Dette er en side \r\n";
            } else {
              write_stream << "HTTP/1.1 404 Not Found\r\n";
              write_stream << "Content-Type: text/plain\r\n";
              write_stream << "Content-Length: 13\r\n";
              write_stream << "\r\n";
              write_stream << "404 Not found";
            }
            // Write to client
            async_write(connection->socket, *write_buffer,
                        [this, connection, write_buffer](
                            const boost::system::error_code &ec, size_t) {
                          // If not error:
                          if (!ec)
                            handle_request(connection);
                        });
          }
        });
  }
  void accept() {
    // The (client) connection is added to the lambda parameter and
    // handle_request in order to keep the object alive for as long as it is
    // needed.
    auto connection = std::make_shared<Connection>(io_context);

    // Accepts a new (client) connection. On connection, immediately start
    // accepting a new connection
    acceptor.async_accept(
        connection->socket,
        [this, connection](const boost::system::error_code &ec) {
          accept();
          // If not error:
          if (!ec) {
            handle_request(connection);
          }
        });
  }

public:
  Server()
      : endpoint(boost::asio::ip::tcp::v4(), 8081),
        acceptor(io_context, endpoint) {}

  void start() {
    accept();

    io_context.run();
  }
};

int main() {
  Server server;

  server.start();
}
