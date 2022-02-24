#include "AuthController.h"
#include "../models/User.h"

using namespace drogon::orm;
using namespace drogon_model::org_chart;

namespace drogon {
    template<>
    inline User fromRequest(const HttpRequest &req) {
        auto jsonPtr = req.getJsonObject();
        auto json = (*jsonPtr)["user"];
        auto user = User(json);
        return user;
    }
}

void AuthController::registerUser(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, User &&pUser) const
{
    LOG_DEBUG << "registerUser";
    try {

        auto dbClientPtr = drogon::app().getDbClient();
        Mapper<User> mp(dbClientPtr);

        if (!areFieldsValid(pUser)) {
            Json::Value ret;
            ret["error"] = "missing fields";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
            return;
        }

        if (!isUserTaken(pUser, mp)) {
            Json::Value ret;
            ret["error"] = "username is taken";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
            return;
        }

        auto newUser = pUser;
        LOG_DEBUG << newUser.toJson().toStyledString();

        mp.insertFuture(newUser).get();

        auto userWithToken = AuthController::UserWithToken(newUser);
        Json::Value ret = userWithToken.toJson();
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k201Created);
        callback(resp);
    } catch (const DrogonDbException & e) {
        LOG_ERROR << e.base().what();
        Json::Value ret;
        ret["error"] = "database error";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(HttpStatusCode::k500InternalServerError);
        callback(resp);
    }
}

bool AuthController::areFieldsValid(const User &user) const {
    return user.getUsername() == nullptr || user.getPassword() == nullptr;
}

bool AuthController::isUserTaken(const User &user, Mapper<User>& mp) const {
    auto ret = mp.findFutureBy(Criteria(User::Cols::_username, CompareOperator::EQ, user.getValueOfUsername())).get();
    return !ret.empty();
}

AuthController::UserWithToken::UserWithToken(const User &user) {
    token = "TOKEN";
    username = user.getValueOfUsername();
}

Json::Value AuthController::UserWithToken::toJson() {
    Json::Value ret;
    ret["username"] = username;
    ret["token"] = token;
    return ret;
}
