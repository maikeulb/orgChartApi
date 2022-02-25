#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;

class LoginFilter : public HttpFilter<LoginFilter>
{
  public:
    LoginFilter() {}
    virtual void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override;
};
