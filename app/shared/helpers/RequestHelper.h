#pragma once 
#include <string>
#include <json/json.h>
#include <drogon/HttpRequest.h> // Incluye el header específico de Request

// Usamos el namespace de drogon para simplificar
using namespace drogon;

class RequestHelper {
public:
    /**
     * Extrae datos de un objeto JSON según el tipo T solicitado.
     */
    template <typename T>
    static inline T getDataFromKey(const Json::Value& json, const std::string& key) {
        if (!json.isNull() && json.isMember(key)) {
            if constexpr (std::is_same_v<T, std::string>) {
                return json[key].asString();
            }
            else if constexpr (std::is_same_v<T, int>) {
                return json[key].asInt();
            }
            else if constexpr (std::is_same_v<T, bool>) {
                return json[key].asBool();
            }
        }
        // Devuelve el valor por defecto del tipo (string vacío, 0, o false)
        return T();
    }

    /**
     * Obtiene un encabezado específico de la petición.
     * Pasamos por referencia (const HttpRequestPtr &) para mayor eficiencia.
     */
    static std::string getHeader(const HttpRequestPtr& req, const std::string& k) {
        // req ya es reconocido gracias a <drogon/HttpRequest.h>
        return req->getHeader(k);
    }

   
};