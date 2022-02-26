#pragma once

#include <jwt-cpp/jwt.h>

class Jwt
{
  public:
    Jwt(const std::string &secret, const int sessionTime, const std::string &issuer);
    std::string encode(const std::string &field, const int value);
    std::optional<jwt::decoded_jwt<jwt::traits::kazuho_picojson>> decode(const std::string& token) ;

  private:
    std::string secret;
    int sessionTime;
    std::string issuer;
};
