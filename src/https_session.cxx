#include "https_session.h"

#include "service_helpers.h"

#include <boost/asio/placeholders.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/beast/core/string_type.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/beast/version.hpp>

#include <iostream>
#include <utility>

HTTPSSession::SessionResponder::SessionResponder(HTTPSSession &self) :
  self_(self)
{}

void HTTPSSession::SessionResponder::send(StringMessageType&& msg)
{
  auto sp = std::make_shared<StringMessageType>(std::move(msg));
  self_.res_ = sp;
  boost::beast::http::async_write(
    self_.stream_,
    *sp,
    boost::beast::bind_front_handler(
      &HTTPSSession::on_write,
      self_.shared_from_this(),
      sp->need_eof()));
}

void HTTPSSession::SessionResponder::send(EmptyMessageType&& msg)
{
  auto sp = std::make_shared<EmptyMessageType>(std::move(msg));
  self_.res_ = sp;
  boost::beast::http::async_write(
    self_.stream_,
    *sp,
    boost::beast::bind_front_handler(
      &HTTPSSession::on_write,
      self_.shared_from_this(),
      sp->need_eof()));
}

HTTPSSession::HTTPSSession(
  boost::asio::ip::tcp::socket &&socket,
  boost::asio::ssl::context &ctx,
  std::shared_ptr<const std::string> doc_root,
  Service::HTTPSListener &owner) :
  stream_(std::move(socket), ctx),
  doc_root_(std::move(std::move(doc_root))),
  responder_(new SessionResponder(*this)),
  owner_(owner)
{}

void HTTPSSession::run()
{
  boost::asio::dispatch(
    stream_.get_executor(),
    boost::beast::bind_front_handler(
      &HTTPSSession::on_run, shared_from_this()));
}

void HTTPSSession::on_run()
{
  boost::beast::get_lowest_layer(stream_);
  stream_.async_handshake(
    boost::asio::ssl::stream_base::server,
    boost::beast::bind_front_handler(
      &HTTPSSession::on_handshake,
      shared_from_this()));
}

void HTTPSSession::on_handshake(boost::beast::error_code ec)
{
  if (ec) {
    std::cerr << "Failed to handshake with: " << ec.message() << std::endl;
    do_close();
  } else {
    do_read();
  }
}

void HTTPSSession::do_read()
{
  req_ = {};
  boost::beast::get_lowest_layer(stream_);
  boost::beast::http::async_read(stream_, buffer_, req_,
    boost::beast::bind_front_handler(
      &HTTPSSession::on_read, shared_from_this()));
}

void HTTPSSession::on_read(
  boost::beast::error_code ec,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (ec == boost::beast::http::error::end_of_stream) {
    do_close();
  } else if (ec) {
    std::cerr << "Failed to read in session with: " << ec.message() <<
      std::endl;
  } else {
    try {
      
    } catch (std::runtime_error& rte) {
      std::cerr << "Failed to handle " << req_.target() << "with: " <<
          rte.what() << std::endl;;
      responder_->send(not_found_handler(req_.target(), req_));
    }
  }
}

bool HTTPSSession::handle_request(SessionResponder::StringRequestType &&req)
{
  SessionResponder::StringMessageType response;
  if (req.method() != boost::beast::http::verb::get &&
  req.method() != boost::beast::http::verb::head) {
    response = bad_request_handler("Unknown HTTP-method\n", req);
  } else if (req.target().empty() || req.target()[0] != '/' ||
    static_cast<long>(req.target().find("..")) !=
    static_cast<long>(boost::beast::string_view::npos)) {
    response = bad_request_handler("Illegal request-target\n", req);
  } else {
    std::string path = path_cat(*doc_root_, req.target());
    boost::beast::error_code ec;
    boost::beast::http::file_body::value_type body;
    body.open(path.c_str(), boost::beast::file_mode::scan, ec);
    if (ec == boost::beast::errc::no_such_file_or_directory) {
      response = not_found_handler(req.target(), req);
    } else if (ec) {
      response = server_error_handler(ec.message(), req);
    } else {
      const std::size_t body_size = body.size();
      if (req.method() == boost::beast::http::verb::head) {
        boost::beast::http::response<boost::beast::http::empty_body> res(
          boost::beast::http::status::ok, req.version());
        res.set(boost::beast::http::field::server,
          BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type,
          mime_type(path));
        res.content_length(body_size);
        res.keep_alive(req.keep_alive());
        responder_->send(std::move(res));
        return true;
      } else if (req.method() == boost::beast::http::verb::get) {
        std::string body_str;
        body_str.resize(body.size());
        boost::beast::error_code read_ec;
        body.file().read(&body_str[0], body.size(), read_ec);
        if (not read_ec) {
          response = boost::beast::http::response<
            boost::beast::http::string_body>(
              std::piecewise_construct,
              std::make_tuple(std::move(body_str)),
              std::make_tuple(boost::beast::http::status::ok,
                req.version()));
        } else {
          std::cerr << "Could not read body!" << std::endl;
          response = server_error_handler(ec.message(), req);
        }
      }
    }
  }
  responder_->send(std::move(response));
  return true;
}

void HTTPSSession::do_close()
{
  boost::beast::get_lowest_layer(stream_).expires_after(
    std::chrono::seconds(30));

  stream_.async_shutdown(boost::beast::bind_front_handler(
    &HTTPSSession::on_shutdown, shared_from_this()));
}

void HTTPSSession::on_shutdown(boost::beast::error_code ec)
{
  if (ec) {
    std::cerr << "Failed to shutdown with: " << ec.message() << std::endl;
  }
  owner_.reset();
}

void HTTPSSession::on_write(
  bool close,
  boost::beast::error_code ec,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    std::cerr << "Failed to write response with: " << ec.message() <<
      std::endl;
  } else if (close) {
    do_close();
  } else {
    res_ = nullptr;
    do_read();
  }
}


