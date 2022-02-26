#include "JwtService.h"

std::string JwtService::encode(const std::string &field, const int value) {
    auto secret = drogon::app().getCustomConfig()["jwt-secret"].asString();
    auto duration = drogon::app().getCustomConfig()["jwt-sessionTime"].asInt();
    auto time = std::chrono::system_clock::now();
    auto expires_at = std::chrono::duration_cast<std::chrono::seconds>((time + std::chrono::seconds{(duration ? 30 : 1) * 24}).time_since_epoch()).count();
    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_issued_at(time)
        .set_expires_at(std::chrono::system_clock::from_time_t(expires_at))
        .set_payload_claim(field, jwt::claim(std::to_string(value)))
        .sign(jwt::algorithm::hs256{secret});
    return token;
}

optional<jwt::decoded_jwt<jwt::traits::kazuho_picojson>> JwtService::decode(const std::string& token) {
    auto secret = drogon::app().getCustomConfig()["jwt-secret"].asString();
    auto verifier = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{secret})
        .with_issuer("auth0");
    try {
        auto decoded = jwt::decode(token);
        verifier.verify(decoded);
        return decoded;
    } catch (const std::runtime_error &e) {
        LOG_ERROR << e.what();
        return std::nullopt;
    }
}
