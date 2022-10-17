#include "service.h"

#include "https_session.h"

#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>

#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include <iostream>

namespace
{
  std::shared_ptr<Service::HTTPSListener>
  make_https_listener(
    boost::asio::io_context &ioc,
    boost::asio::ssl::context &ctx,
    std::shared_ptr<const std::string> doc_root,
    const boost::asio::ip::address &address,
    const uint16_t port)
  {
    auto endpoint = boost::asio::ip::tcp::endpoint(address, port);
    return std::make_shared<
      Service::HTTPSListener>(ioc, ctx, endpoint, doc_root);
  }
}

Service::Service(
  const Service::ConnectionInfo &ci) :
  ioc_(ci.threads_),
  ctx_(boost::asio::ssl::context::tlsv12)
{
  load_server_cert(ctx_, ci.pem_);
  auto doc_root = std::make_shared<const std::string>(ci.doc_root_);
  auto const address = boost::asio::ip::make_address(ci.address_);
  auto const port = static_cast<uint16_t>(ci.port_);

  auto sig_handler = [&](
    const boost::system::error_code &ec, int signum) {
      std::cerr << ec.message() << std::endl;
      exit(signum);
    };
  boost::asio::signal_set signals(ioc_, SIGINT);
  signals.async_wait(sig_handler);

  std::shared_ptr<HTTPSListener> listener = make_https_listener(
    ioc_, ctx_, doc_root, address, port);
  std::vector<std::thread> v;
  v.reserve(ci.threads_ - 1);
  for (auto i = v.size(); i > 0; --i) {
    v.emplace_back([&]() { ioc_.run(); });
  }

  std::cout << "Server running..." << std::endl;
  listener->run();
  ioc_.run();
  for (auto &t : v) {
    if (t.joinable()) {
      t.join();
    }
  }
}

Service::HTTPSListener::HTTPSListener(
  boost::asio::io_context &ioc,
  boost::asio::ssl::context &ctx,
  boost::asio::ip::tcp::endpoint &endpoint,
  std::shared_ptr<const std::string> &doc_root) :
  ioc_(ioc),
  ctx_(ctx),
  acceptor_(ioc),
  endpoint_(endpoint),
  doc_root_(doc_root)
{
  create_acceptor();
}

Service::HTTPSListener::~HTTPSListener()
{}

void Service::HTTPSListener::run()
{
  do_accept();
}

void Service::HTTPSListener::reset()
{
  acceptor_.close();
  create_acceptor();
  run();
}

void Service::HTTPSListener::do_accept()
{
  acceptor_.async_accept(
    boost::asio::make_strand(ioc_),
    boost::beast::bind_front_handler(
      &HTTPSListener::on_accept,
      shared_from_this()));
}

void Service::HTTPSListener::on_accept(
  boost::beast::error_code ec,
  boost::asio::ip::tcp::socket socket)
{
  if (ec) {
    std::cerr << ec.message() << std::endl;
  } else {
    socket.set_option(boost::asio::socket_base::keep_alive(true), ec);
    if (not ec) {
      std::make_shared<HTTPSSession>(
        std::move(socket), ctx_, doc_root_, *this)->run();
    } else {
      std::cerr << "Failed to keep socket alive." << std::endl;
    }
  }
}

void Service::HTTPSListener::create_acceptor()
{
  boost::beast::error_code ec;
  acceptor_.open(endpoint_.protocol(), ec);
  if (ec) {
    std::cerr << "Failed to accept incoming connections with: " <<
      ec.message() << std::endl;
    throw boost::beast::system_error(ec);
  }

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
  if (ec) {
    std::cerr << "Failed to set options on acceptor with: " <<
      ec.message() << std::endl;
    throw boost::beast::system_error(ec);
  }

  acceptor_.set_option(boost::asio::socket_base::keep_alive(true), ec);
  if (ec) {
    std::cerr << "Failed to set options on acceptor with: " <<
      ec.message() << std::endl;
    throw boost::beast::system_error(ec);
  }

  acceptor_.bind(endpoint_, ec);
  if (ec) {
    std::cerr << "Failed to bind to endpoint with: " <<
      ec.message() << std::endl;
    throw boost::beast::system_error(ec);
  }

  acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    std::cerr << "Failed to listen to incoming connection!" <<
      ec.message() << std::endl;
    throw boost::beast::system_error(ec);
  }
}
