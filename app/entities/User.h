#pragma once

#include <string>
#include <json/json.h>

using std::string;

class User {
public:
    int id;
    string email;
    string name;
private:
    string password;

public:
    User() {}

    void setPwd(string pwd) {
        password = pwd;
    }

    string getPwd() {
        return password;
    }


public: 
    Json::Value toJson() const {
        Json::Value json;
        json["id"] = id;
        json["email"] = email;
        json["name"] = name;
        // No incluimos la contraseña en el JSON por seguridad
        return json;
	}
};