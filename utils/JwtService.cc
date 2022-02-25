#include <jwt-cpp/jwt.h>
#include "JwtService.h"

const std::string JwtService::secret = drogon::app().getCustomConfig()["jwt-secret"].asString();
const int JwtService::duration = drogon::app().getCustomConfig()["jwt-sessionTime"].asInt();

std::string JwtService::generateFromUser(const User& user) {
    auto time = std::chrono::system_clock::now();
    auto expires_at = std::chrono::duration_cast<std::chrono::seconds>((time + std::chrono::seconds{duration}).time_since_epoch()).count();
    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_issued_at(time)
        .set_expires_at(std::chrono::system_clock::from_time_t(expires_at))
        .set_payload_claim("user_id", jwt::claim(std::to_string(user.getValueOfId())))
        .sign(jwt::algorithm::hs256{secret});
    return token;
}

optional<int> JwtService::getCurrentUserIdFromRequest(const HttpRequestPtr &req) {
    if (req->getHeader("Authorization").empty()) {
        return std::nullopt;
    }
    auto token = req->getHeader("Authorization").substr(7);
    return getUserIdFromJwt(token);
}

optional<int> JwtService::getUserIdFromJwt(const std::string& token) {
    auto verifier = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{JwtService::secret})
        .with_issuer("auth0");
    try {
        auto decoded = jwt::decode(token);
        verifier.verify(decoded);
        auto id = decoded.get_payload_claim("user_id").as_string();
        return stoi(id);
    } catch (const std::runtime_error &e) {
        LOG_ERROR << e.what();
        return std::nullopt;
    }
}
