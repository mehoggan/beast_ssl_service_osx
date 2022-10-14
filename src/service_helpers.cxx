#include "service_helpers.h"

boost::beast::string_view mime_type(boost::beast::string_view path)
{
  const auto ext = [&path] {
    const auto pos = path.rfind(".");
    boost::beast::string_view ret;
    if (pos != boost::beast::string_view::npos) {
      ret = path.substr(pos);
    }
    return ret;
  }();
  boost::beast::string_view ret;
  if (boost::beast::iequals(ext, ".htm")) {
    ret = "text/html";
  } else if (boost::beast::iequals(ext, ".html")) {
    ret = "text/html";
  } else if (boost::beast::iequals(ext, ".php")) {
    ret = "text/html";
  } else if (boost::beast::iequals(ext, ".css")) {
    ret = "text/css";
  } else if (boost::beast::iequals(ext, ".txt")) {
    ret = "text/plain";
  } else if (boost::beast::iequals(ext, ".js")) {
    ret = "application/javascript";
  } else if (boost::beast::iequals(ext, ".json")) {
    ret = "application/json";
  } else if (boost::beast::iequals(ext, ".xml")) {
    ret = "application/xml";
  } else if (boost::beast::iequals(ext, ".swf")) {
    ret = "application/x-shockwave-flash";
  } else if (boost::beast::iequals(ext, ".flv")) {
    ret = "video/x-flv";
  } else if (boost::beast::iequals(ext, ".png")) {
    ret = "image/png";
  } else if (boost::beast::iequals(ext, ".jpe")) {
    ret = "image/jpeg";
  } else if (boost::beast::iequals(ext, ".jpeg")) {
    ret = "image/jpeg";
  } else if (boost::beast::iequals(ext, ".jpg")) {
    ret = "image/jpeg";
  } else if (boost::beast::iequals(ext, ".gif")) {
    ret = "image/gif";
  } else if (boost::beast::iequals(ext, ".bmp")) {
    ret = "image/bmp";
  } else if (boost::beast::iequals(ext, ".ico")) {
    ret = "image/vnd.microsoft.icon";
  } else if (boost::beast::iequals(ext, ".tiff")) {
    ret = "image/tiff";
  } else if (boost::beast::iequals(ext, ".tif")) {
    ret = "image/tiff";
  } else if (boost::beast::iequals(ext, ".svg")) {
    ret = "image/svg+xml";
  } else if (boost::beast::iequals(ext, ".sggz")) {
    ret = "image/svg+xml";
  } else {
    ret = "application/text";
  }
  return ret;
}

std::string path_cat(
  boost::beast::string_view base,
  boost::beast::string_view path)
{
  std::string ret;
  if (base.empty()) {
    ret = std::string(path);
  } else {
    ret = std::string(base);
    constexpr char path_separator = '/';
    if (ret.back() == path_separator) {
      ret.resize(ret.size() - 1);
    }
    ret.append(path.data(), path.size());
  }
  return ret;
}

void load_server_cert(
  boost::asio::ssl::context& ctx,
  const boost::filesystem::path &pem_path_)
{
  ctx.set_password_callback([](std::size_t,
    boost::asio::ssl::context_base::password_purpose) {
      return "test";
    });

  ctx.set_options(
    boost::asio::ssl::context::default_workarounds |
    boost::asio::ssl::context::no_sslv2 |
    boost::asio::ssl::context::single_dh_use);

  ctx.use_certificate_chain_file(pem_path_.string());
  ctx.use_private_key_file(
    pem_path_.string(),
    boost::asio::ssl::context::file_format::pem);
}

boost::beast::http::response<boost::beast::http::string_body>
bad_request_handler(
  boost::beast::string_view why,
  boost::beast::http::request<boost::beast::http::string_body>& req)
{
  boost::beast::http::response<boost::beast::http::string_body> res(
    boost::beast::http::status::bad_request, req.version());
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_script_type, "text/html");
  res.keep_alive(req.keep_alive());
  res.body() = std::string(why);
  res.prepare_payload();
  return res;
}

 boost::beast::http::response<boost::beast::http::string_body>
 not_found_handler(
   boost::beast::string_view target,
   boost::beast::http::request<boost::beast::http::string_body>& req)
{
   boost::beast::http::response<boost::beast::http::string_body> res(
     boost::beast::http::status::not_found, req.version());
   res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
   res.set(boost::beast::http::field::content_type, "text/html");
   res.keep_alive(req.keep_alive());
   res.body() = "The resource '" + std::string(target) + "' was not found.\n";
   res.prepare_payload();
   return res;
}

 boost::beast::http::response<boost::beast::http::string_body>
 server_error_handler(
   boost::beast::string_view what,
   boost::beast::http::request<boost::beast::http::string_body>& req)
{
   boost::beast::http::response<boost::beast::http::string_body> res(
     boost::beast::http::status::internal_server_error, req.version());
   res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
   res.set(boost::beast::http::field::content_type, "text/html");
   res.keep_alive(req.keep_alive());
   res.body() = "An error occurred: '" + std::string(what) + "'\n";
   return res;
}

