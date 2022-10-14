#ifndef SERVICE_H_INCLUDED
#define SERVICE_H_INCLUDED

#include "endpoints.h"
#include "service_helpers.h"

#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core/tcp_stream.hpp>

#include <memory>
#include <unordered_map>

/*
 * Code in this module borrowed from:
 * https://www.boost.org/doc/libs/develop/libs/beast/example/http/server/
 * async-ssl/http_server_async_ssl.cpp
 */

class Service
{
public:
  struct ConnectionInfo
  {
    std::string address_;
    std::uint32_t port_;
    std::string doc_root_;
    std::uint16_t threads_;
    std::string pem_;
  };

  struct Router
  {
  public:
    bool register_endpoint(const boost::beast::string_view &endpoint,
      std::unique_ptr<EndpointHandler> handler);

    std::unique_ptr<EndpointHandler>& operator[](
      const boost::beast::string_view &endpoint);

  private:
    std::unordered_map<
      boost::beast::string_view,
      std::unique_ptr<EndpointHandler>> router_;
  };

  class HTTPSListener :
    public std::enable_shared_from_this<HTTPSListener>
  {
  public:
    HTTPSListener(
      boost::asio::io_context &ioc,
      boost::asio::ssl::context &ctx,
      boost::asio::ip::tcp::endpoint &endpoint,
      std::shared_ptr<const std::string> &doc_root,
      Router &router_);

    ~HTTPSListener();

    void run();

    void reset();

  private:
    void do_accept();

    void on_accept(boost::beast::error_code ec,
                   boost::asio::ip::tcp::socket socket);

    void create_acceptor();

  private:
    boost::asio::io_context& ioc_;
    boost::asio::ssl::context& ctx_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::endpoint endpoint_;
    std::shared_ptr<const std::string> doc_root_;
    Router& router_;
  };

public:
  /**
   * @brief default ctor
   */
  explicit Service(const ConnectionInfo &ci);

private:
  boost::asio::io_context ioc_;
  boost::asio::ssl::context ctx_;
  Router router_;
};
#endif
