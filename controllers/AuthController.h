#pragma once

#include <drogon/HttpController.h>
#include <string>
#include "../models/User.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::org_chart;

class AuthController : public drogon::HttpController<AuthController> {
 public:
    METHOD_LIST_BEGIN
      ADD_METHOD_TO(AuthController::registerUser, "/auth/register", Post);
      ADD_METHOD_TO(AuthController::loginUser, "/auth/login", Post);
    METHOD_LIST_END

    void registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, User &&pUser) const;
    void loginUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, User &&pUser) const;

 private:
    struct UserWithToken {
        std::string username;
        std::string password;
        std::string token;
        explicit UserWithToken(const User &user);
        Json::Value toJson();
    };

    bool areFieldsValid(const User &user) const;
    bool isUserAvailable(const User &user, Mapper<User> &mp) const;
    bool isPasswordValid(const std::string &text, const std::string &hash) const;
};
