#ifndef SERVICE_HELPERS_H_INCLUDED
#define SERVICE_HELPERS_H_INCLUDED

#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/filesystem/path.hpp>

namespace std
{
  template <>
  struct hash<boost::beast::string_view>
  {
    std::size_t operator()(const boost::beast::string_view& sv) const
    {
      return std::hash<string>()(sv);
    }
  };
}

boost::beast::string_view mime_type(boost::beast::string_view path);

std::string path_cat(
  boost::beast::string_view base,
  boost::beast::string_view path);

void load_server_cert(
  boost::asio::ssl::context& ctx,
  const boost::filesystem::path &pem_path_);

boost::beast::http::response<boost::beast::http::string_body>
bad_request_handler(
  boost::beast::string_view why,
  boost::beast::http::request<boost::beast::http::string_body>& req);

boost::beast::http::response<boost::beast::http::string_body>
not_found_handler(
  boost::beast::string_view target,
  boost::beast::http::request<boost::beast::http::string_body>& req);

boost::beast::http::response<boost::beast::http::string_body>
server_error_handler(
  boost::beast::string_view what,
  boost::beast::http::request<boost::beast::http::string_body>& req);
#endif
