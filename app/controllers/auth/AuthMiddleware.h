#pragma once

#include <drogon/HttpMiddleware.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <jwt-cpp/jwt.h>
#include "../../shared/helpers/EnvHelper.h"
#include <iostream>

using namespace drogon;

class AuthMiddleware : public HttpMiddleware<AuthMiddleware> {
public:
    AuthMiddleware() = default;

    void invoke(const HttpRequestPtr& req,
        MiddlewareNextCallback&& nextCb,
        MiddlewareCallback&& mcb) override
    {
        const std::string& authHeader = req->getHeader("Authorization");

        if (authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
            this->sendErrorResponse(mcb, "Authorization token is missing or malformed", k401Unauthorized);
            return;
        }

        std::string token = authHeader.substr(7);

        try {
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{ EnvHelper::getSecretJWT() })
                .with_issuer("auth0");

            auto decoded = jwt::decode(token);
            verifier.verify(decoded);

            nextCb([mcb = std::move(mcb)](const HttpResponsePtr& resp) {
                mcb(resp);
                });
        }
        // Solución a E0135: Usamos runtime_error que es la base de jwt-cpp
        catch (const std::runtime_error& e) {
            this->sendErrorResponse(mcb, std::string("Invalid Token: ") + e.what(), k401Unauthorized);
        }
        catch (...) {
            this->sendErrorResponse(mcb, "Undefined error", k401Unauthorized);
        }
    }

private:
    void sendErrorResponse(MiddlewareCallback& mcb, const std::string& message, HttpStatusCode code) {
        Json::Value root;
        root["status"] = static_cast<int>(code);
        root["message"] = message;
        root["error"] = true;

        auto resp = HttpResponse::newHttpJsonResponse(root);
        resp->setStatusCode(code);
        mcb(resp);
    }
};

