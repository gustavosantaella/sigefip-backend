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
        if(authHeader.empty()){
            this->sendErrorResponse(mcb, "Missing Authorization Header", k401Unauthorized);

            return;
        }

        // 1. VALIDACIÓN SEGURA DE LONGITUD
        // Evita el crash "invalid string position" si el header es corto o vacío
        if (authHeader.size() < 7 || authHeader.compare(0, 7, "Bearer ") != 0) {
            this->sendErrorResponse(mcb, "Missing Token or Token is malformed", k401Unauthorized);
            return;
        }

        // 2. EXTRACCIÓN DEL TOKEN
        std::string token = authHeader.substr(7);

        try {
            // 3. VERIFICACIÓN CON TRAITS POR DEFECTO (Para evitar errores de plantilla)
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{ EnvHelper::getSecretJWT() })
                .with_issuer("auth0");

            auto decoded = jwt::decode(token);
            verifier.verify(decoded);

            // 4. CONTINUAR FLUJO
            nextCb([mcb = std::move(mcb)](const HttpResponsePtr& resp) {
                mcb(resp);
                });

        }
        catch (const std::runtime_error& e) {
            // Captura errores de expiración o firma inválida de jwt-cpp
            this->sendErrorResponse(mcb, std::string("Invalid Token: ") + e.what(), k401Unauthorized);
        }
        catch (...) {
            this->sendErrorResponse(mcb, "An Error ocurred", k401Unauthorized);
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


