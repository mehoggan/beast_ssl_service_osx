#ifndef HTTPS_SESSION_H_INCLUDED
#define HTTPS_SESSION_H_INCLUDED

#include "service.h"

#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include <memory>

class HTTPSSession :
  public std::enable_shared_from_this<HTTPSSession>
{
  private:
    friend class HTTPSListener;

  private:
    class SessionResponder
    {
    public:
      typedef boost::beast::http::empty_body EmptyBodyType;
      typedef boost::beast::http::string_body StringBodyType;
      typedef boost::beast::http::basic_fields<std::allocator<char>>
        FieldsType;
      typedef boost::beast::http::message<false, StringBodyType, FieldsType>
        StringMessageType;
      typedef boost::beast::http::message<false, EmptyBodyType, FieldsType>
        EmptyMessageType;
      typedef boost::beast::http::request<StringBodyType> StringRequestType;

    public:
      explicit SessionResponder(HTTPSSession &self);

      void send(StringMessageType &&);
      void send(EmptyMessageType &&);

    private:
      HTTPSSession &self_;
    };

  public:
    HTTPSSession(
      boost::asio::ip::tcp::socket &&socket,
      boost::asio::ssl::context &ctx,
      std::shared_ptr<const std::string> doc_root,
      Service::HTTPSListener &owner);

    void run();

  private:
    void on_run();
    void on_handshake(boost::beast::error_code ec);
    void do_read();
    void on_read(boost::beast::error_code ec,
      std::size_t bytes_transferred);
    bool handle_request(SessionResponder::StringRequestType &&req);
    void do_close();
    void on_shutdown(boost::beast::error_code ec);
    void on_write(bool close, boost::beast::error_code ec, std::size_t);


  private:
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;
    boost::beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_;
    SessionResponder::StringRequestType req_;
    std::shared_ptr<void> res_;
    std::shared_ptr<SessionResponder> responder_;
    Service::HTTPSListener &owner_;
};
#endif
