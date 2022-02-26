#pragma once

#include <drogon/drogon.h>
#include <models/User.h>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

class JwtService
{
  public:
    static std::string generateFromUser(const User& user);
    static std::optional<int> getCurrentUserIdFromRequest(const HttpRequestPtr &req);
    static std::optional<int> getUserIdFromJwt(const std::string& token);
    JwtService() = default;
};
