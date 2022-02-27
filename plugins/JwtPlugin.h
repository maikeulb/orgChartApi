#pragma once

#include <drogon/plugins/Plugin.h>
#include "Jwt.h"

class JwtPlugin : public drogon::Plugin<JwtPlugin> {
 public:
    virtual void initAndStart(const Json::Value &config) override;
    virtual void shutdown() override;
    auto init() -> Jwt;

 private:
    Json::Value config;
};
