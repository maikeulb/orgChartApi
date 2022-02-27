#include "JwtPlugin.h"
#include <drogon/drogon.h>

using namespace drogon;

void JwtPlugin::initAndStart(const Json::Value &config) {
    LOG_DEBUG << "JWT initialized and Start";
    this->config = config;
}

void JwtPlugin::shutdown() {
    LOG_DEBUG << "JWT shuut down";
}

auto JwtPlugin::init() -> Jwt {
    auto secret = config.get("secret", "secret").asString();
    auto sessionTime = config.get("sessionTime", 3600).asInt();
    auto issuer = config.get("issuer", "auth0").asString();
    Jwt jwt(secret, sessionTime, issuer);
    return jwt;
}
