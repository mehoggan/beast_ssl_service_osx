#ifndef ENDPOINTS_H_INCLUDED
#define ENDPOINTS_H_INCLUDED

#include <boost/beast.hpp>

struct Responder
{
public:
  typedef boost::beast::http::empty_body EmptyBodyType;
  typedef boost::beast::http::string_body StringBodyType;
  typedef boost::beast::http::basic_fields<std::allocator<char>> FieldsType;
  typedef boost::beast::http::message<false, StringBodyType, FieldsType>
    StringMessageType;
  typedef boost::beast::http::message<false, EmptyBodyType, FieldsType>
    EmptyMessageType;

  virtual ~Responder();

  virtual void send(StringMessageType &&) = 0;
  virtual void send(EmptyMessageType &&) = 0;
};

struct EndpointHandler
{
public:
  typedef boost::beast::http::string_body StringBodyType;
  typedef boost::beast::http::request<StringBodyType> StringRequestType;

public:
  EndpointHandler();

  explicit EndpointHandler(const boost::beast::string_view& endpoint);

  virtual ~EndpointHandler();

  virtual bool operator()(StringRequestType&& req,
    std::unique_ptr<Responder>&& responder);

  virtual bool operator()(StringRequestType&& req,
    std::unique_ptr<Responder>&& responder) const;

private:
  boost::beast::string_view endpoint_;
};

struct DocRootEndpoint :
  public EndpointHandler
{
public:
  explicit DocRootEndpoint(const boost::beast::string_view& doc_root);

  virtual ~DocRootEndpoint();

  bool operator()(StringRequestType&& req,
    std::unique_ptr<Responder>&& responder) final;

  bool operator()(StringRequestType&& req,
    std::unique_ptr<Responder>&& responder) const final;

private:
  bool handle_request(StringRequestType&& req,
    std::unique_ptr<Responder>&& responder) const;

private:
  boost::beast::string_view doc_root_;
};
#endif
