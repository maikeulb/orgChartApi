#include <drogon/drogon.h>
#include <utility>
#include "Jwt.h"

Jwt::Jwt(const std::string &secret, const int sessionTime, const std::string &issuer) :
  secret{std::move(secret)}, sessionTime{sessionTime}, issuer{std::move(issuer)} {}

auto Jwt::encode(const std::string &field, const int value) -> std::string {
    auto time = std::chrono::system_clock::now();
    auto expiresAt = std::chrono::duration_cast<std::chrono::seconds>((time + std::chrono::seconds{sessionTime}).time_since_epoch()).count();
    auto token = jwt::create()
        .set_issuer(issuer)
        .set_type("JWS")
        .set_issued_at(time)
        .set_expires_at(std::chrono::system_clock::from_time_t(expiresAt))
        .set_payload_claim(field, jwt::claim(std::to_string(value)))
        .sign(jwt::algorithm::hs256{secret});
    return token;
}

auto Jwt::decode(const std::string& token) -> jwt::decoded_jwt<jwt::traits::kazuho_picojson> {
    auto verifier = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{secret})
        .with_issuer(issuer);
    auto decoded = jwt::decode(token);
    verifier.verify(decoded);
    return decoded;
}
