#pragma once

#include <cstdlib>
#include <string>

class EnvHelper {


public:
	static bool isProductionEnv() {}

public:
	static std::string getEnvVar(const std::string& varName) {
		const char* val = std::getenv(varName.c_str());
		if (val == nullptr) {
			return "";
		}
		return std::string(val);
	}

public:
	static std::string getSecretJWT() {
		return getEnvVar("SECRET_JWT");
	}

};