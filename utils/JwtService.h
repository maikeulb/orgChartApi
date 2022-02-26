#pragma once

#include <drogon/drogon.h>
#include <jwt-cpp/jwt.h>

using namespace drogon;

class JwtService
{
  public:
    static std::string encode(const std::string &field, const int value);
    static optional<jwt::decoded_jwt<jwt::traits::kazuho_picojson>> decode(const std::string& token) ;
  private:
    JwtService() = default;
};
