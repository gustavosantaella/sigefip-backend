#pragma once

#include <drogon/HttpMiddleware.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <jwt-cpp/jwt.h>

using namespace drogon;

class AuthMiddleware : public HttpMiddleware<AuthMiddleware> {
public:
    AuthMiddleware() {}

    void invoke(const HttpRequestPtr& req,
        MiddlewareNextCallback&& nextCb,
        MiddlewareCallback&& mcb) override
    {
        std::cout << "IN MIDDLEWARE" << std::endl;

        // 1. Obtener el token del encabezado Authorization
        const std::string& authHeader = req->getHeader("Authorization");

        // El formato estándar es "Bearer <token>"
        if (authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
            auto resp = HttpResponse::newHttpJsonResponse(k401Unauthorized);
            mcb(resp);
            return;
        }

        std::string token = authHeader.substr(7);

        try {
            // 2. Validar el token usando tu JwtHelper
            jwt::decode(token);

           
            nextCb([mcb = std::move(mcb)](const HttpResponsePtr& resp) {
                // Aquí puedes añadir cabeceras globales de respuesta si quieres
                mcb(resp);
                });

        }
        catch (const std::exception& e) {
            // Si el token expiró o es inválido, jwt-cpp lanzará una excepción
            auto resp = HttpResponse::newHttpJsonResponse(k401Unauthorized);
            resp->setBody("Token invalido o expirado");
            mcb(resp);
        }
    }
};