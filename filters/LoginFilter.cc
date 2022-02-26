#include <drogon/drogon.h>
#include "LoginFilter.h"
#include "../plugins/JwtPlugin.h"

using namespace drogon;

void LoginFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb)
{
    try {
        if (req->getHeader("Authorization").empty()) {
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k400BadRequest);
            fcb(res);
            return;
        }

        auto token = req->getHeader("Authorization").substr(7);
        auto *jwtPtr = drogon::app().getPlugin<JwtPlugin>();
        auto jwt = jwtPtr->init();
        auto decoded = jwt.decode(token);

        if (!decoded.has_value()) {
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k400BadRequest);
            fcb(res);
        }

        if (decoded.value().get_payload_claim("user_id").as_string().empty()) {
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k400BadRequest);
            fcb(res);
            return;
        }
        auto userId = stoi(decoded.value().get_payload_claim("user_id").as_string());
        fccb();
    } catch (const std::runtime_error &e) {
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k500InternalServerError);
        fcb(res);
    }
}
