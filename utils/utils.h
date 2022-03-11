#pragma once

#include <drogon/drogon.h>

void badRequest (
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    std::string err,
    drogon::HttpStatusCode code = drogon::k400BadRequest
);

Json::Value makeErrResp(std::string err);
