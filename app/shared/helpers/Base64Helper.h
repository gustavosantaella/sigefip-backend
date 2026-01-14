#pragma once
#include <string>
#include <json/json.h>
#include <drogon/utils/Utilities.h> // Necesario para utils::base64Decode

class Base64Helper {
public:
    static Json::Value getJsonFromJson64(const std::string& bs64) {
        // 1. Decodificar la cadena Base64 a un std::string normal
        std::string decodedStr = drogon::utils::base64Decode(bs64);

        Json::Value jsonResult;
        Json::CharReaderBuilder readerBuilder;
        std::string errs;

        // 2. Convertir el string decodificado en un objeto Json::Value
        std::unique_ptr<Json::CharReader> const reader(readerBuilder.newCharReader());
        bool parsingSuccessful = reader->parse(
            decodedStr.c_str(),
            decodedStr.c_str() + decodedStr.size(),
            &jsonResult,
            &errs
        );

        if (!parsingSuccessful) {
            // Si el JSON es inválido tras decodificar, retornamos un objeto vacío o con el error
            Json::Value errorNode;
            errorNode["error"] = "Invalid JSON after Base64 decode";
            errorNode["details"] = errs;
            return errorNode;
        }

        return jsonResult;
    }
};