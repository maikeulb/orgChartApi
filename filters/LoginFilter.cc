#include <drogon/drogon.h>
#include "LoginFilter.h"
#include "../plugins/JwtPlugin.h"

using namespace drogon;

void LoginFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    try {
        if (req->getHeader("Authorization").empty()) {
            Json::Value ret;
            ret["error"] = "missing Authorization header";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k400BadRequest);
            fcb(resp);
            return;
        }

        auto token = req->getHeader("Authorization").substr(7);
        auto *jwtPtr = drogon::app().getPlugin<JwtPlugin>();
        auto jwt = jwtPtr->init();
        auto decoded = jwt.decode(token);
        auto userId = stoi(decoded.get_payload_claim("user_id").as_string());
        fccb();
    } catch (jwt::token_verification_exception &e) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        LOG_ERROR << e.what();
        resp->setStatusCode(k400BadRequest);
        fcb(resp);
    } catch (const std::runtime_error &e) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        LOG_ERROR << e.what();
        resp->setStatusCode(k500InternalServerError);
        fcb(resp);
    }
}
