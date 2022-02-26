#include "LoginFilter.h"
#include "../utils/JwtService.h"

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
        auto decoded = JwtService::decode(token).value();

        if (req->getHeader("Authorization").empty()) {
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k400BadRequest);
            fcb(res);
            return;
        }
        if (decoded.get_payload_claim("user_id").as_string().empty()) {
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k400BadRequest);
            fcb(res);
            return;
        }
        auto userId = stoi(decoded.get_payload_claim("user_id").as_string());
        fccb();
    } catch (const std::runtime_error &e) {
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k500InternalServerError);
        fcb(res);
    }
}
