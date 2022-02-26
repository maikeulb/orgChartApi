#include "Jwt.h"
#include <drogon/drogon.h>

Jwt::Jwt(const std::string &secret, const int sessionTime, const std::string &issuer) {
    this->secret = secret;
    this->sessionTime = sessionTime;
    this->issuer = issuer;
}

std::string Jwt::encode(const std::string &field, const int value) {
    auto time = std::chrono::system_clock::now();
    auto expiresAt = std::chrono::duration_cast<std::chrono::seconds>((time + std::chrono::seconds{(sessionTime ? 30 : 1) * 24}).time_since_epoch()).count();
    auto token = jwt::create()
        .set_issuer(issuer)
        .set_type("JWS")
        .set_issued_at(time)
        .set_expires_at(std::chrono::system_clock::from_time_t(expiresAt))
        .set_payload_claim(field, jwt::claim(std::to_string(value)))
        .sign(jwt::algorithm::hs256{secret});
    return token;
}

std::optional<jwt::decoded_jwt<jwt::traits::kazuho_picojson>> Jwt::decode(const std::string& token) {
    auto verifier = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{secret})
        .with_issuer(issuer);
    try {
        auto decoded = jwt::decode(token);
        verifier.verify(decoded);
        return decoded;
    } catch (const std::runtime_error &e) {
        LOG_ERROR << e.what();
        return std::nullopt;
    }
}
