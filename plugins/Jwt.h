#pragma once

#include <jwt-cpp/jwt.h>
#include <string>

class Jwt {
 public:
    Jwt(const std::string &secret, const int sessionTime, const std::string &issuer);
    auto encode(const std::string &field, const int value) -> std::string;
    auto decode(const std::string& token) -> jwt::decoded_jwt<jwt::traits::kazuho_picojson>;

 private:
    std::string secret;
    int sessionTime;
    std::string issuer;
};
