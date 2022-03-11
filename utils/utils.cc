#include "utils.h"

void badRequest(std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string err, drogon::HttpStatusCode code)
{
    Json::Value ret{};
    ret["error"] = err;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(code);
    callback(resp);
}

Json::Value makeErrResp(std::string err) {
    Json::Value ret{};
    ret["error"] = err;
    return ret;
}
