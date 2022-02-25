#include "LoginFilter.h"
#include "../utils/JwtService.h"

using namespace drogon;

void LoginFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb)
{
    try {
        auto userId = JwtService::getCurrentUserIdFromRequest(req);
        if (!userId.has_value()) {
            auto res = drogon::HttpResponse::newHttpResponse();
            res->setStatusCode(k400BadRequest);
            fcb(res);
        } else {
            fccb();
        }
    } catch (const DrogonDbException & e) {
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k500InternalServerError);
        fcb(res);
    }
}
