#pragma once

#include <string>
#include <unordered_map>

namespace cppnet {

class HttpHeader {
public:
  /*! \enum class Cont
   * @brief: content type
   */
  enum class ContentType {
    kTextPlain,
    kTextHtml,
    kTextCss,
    kTextJavascript,
    kApplicationJson,
    kApplicationOctetStream,
    kImagePng,
    kImageJpeg,
    kImageGif,
    kImageWebp,
    kUnknown
  };
  ContentType ConvertToContentType(const std::string &content_type) const;
  std::string ConvertToStr(const ContentType &content_type) const;

public:
  HttpHeader() = default;

  /**
   * @brief: parse header
   * @param origin_header: origin header
   * @return: 0: success, -1: invalid header
   */
  int Parse(const std::string &origin_header);

  /**
   * @brief: add header
   * @param key: header key
   * @param value: header value
   */
  void Add(const std::string &key, const std::string &value);

  /**
   * @brief: get header
   * @param key: header key
   * @return: header value
   */
  std::string Get(const std::string &key) const;

  /**
   * @brief: convert to string
   * @return: header string
   */
  std::string ToString() const;

  /**
   * @brief: get header
   * @param key: header key
   * @return: header value
   */
  std::string &operator[](const std::string &key);

  /**
   * @brief: get error message
   */
  inline std::string err_msg() { return err_msg_; }

  /**
   * @brief: clear header
   */
  void Clear();

public:
  /**
   * @brief: common header control
   */
  int GetContentLength() const;
  void SetContentLength(int content_length);
  ContentType GetContentType() const;
  void SetContentType(ContentType content_type);
  std::string GetHost() const;
  void SetHost(const std::string &host);
  void SetLongConnection(bool is_long_connect = true);

  /**
   * @brief: get cookie control, TODO
   */
  std::string GetCookie() const;
  void SetCookie(const std::string &cookie);

private:
  std::unordered_map<std::string, std::string> headers_;

  std::string err_msg_;
};

} // namespace cppnet
