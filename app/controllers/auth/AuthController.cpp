#pragma once

#include "../../shared/BaseController.h"
#include "../../shared/helpers/Sha256Helper.h"
#include "../../config/constants/constants.h"
#include "../../services/UserService.h"
#include "../../entities/User.h"
#include <drogon/utils/coroutine.h>
#include <jwt-cpp/jwt.h>
#include "../../shared/helpers/EnvHelper.h"
// Importante: Incluir el trait antes de usarlo

class AuthController : public BaseController<AuthController> {
	UserService userService;

public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(AuthController::login, API_PREFIX "/auth/login", Post);
	ADD_METHOD_TO(AuthController::signUp, API_PREFIX "/auth/sign-up", Post);
	METHOD_LIST_END;

	AuthController() : userService() {};

public:
	Task<void> login(
		HttpRequestPtr req,
		std::function<void(const HttpResponsePtr&)> callback
	)
	{
		const auto& basicHeader = this->getAuthorization(req);
		if (basicHeader.substr(0, 5) != "Basic") {
			this->response(callback, "Invalid Header", k400BadRequest);
			co_return;
		}
		const auto& jsonData = this->getDataFromBasicAuthorization(basicHeader);

		const std::string email = jsonData["email"].asString();
		const std::string pwd = jsonData["pwd"].asString();

		std::optional<User> user = co_await userService.findByEmail(email);
		if (!user) {
			this->response(callback, "User Not Found", k404NotFound, true);
			co_return;
		}

		if (!Sha256Helper::verify(pwd, user->getPwd())) {
			this->response(callback, "Incorrect password, please try again.", k400BadRequest, true);
			co_return;
		}

		const User& u = user.value();
		using namespace std::chrono_literals;
		auto s = 3600s * 3;


		const auto token = jwt::create()
			.set_issuer("auth0")
			.set_type("JWT")
			.set_payload_claim("user", jwt::claim(u.email))
			.set_payload_claim("id", jwt::claim(std::to_string(u.id)))
			.set_expires_in(s)
			.sign(jwt::algorithm::hs256{ EnvHelper::getSecretJWT() });
		// ------------------------

		this->response(callback, token);


		co_return;
	}

public:
	Task<void> signUp(
		HttpRequestPtr req,
		std::function<void(const HttpResponsePtr&)> callback
	) {
		std::string name = this->getDataFromKey<std::string>(req, "name");
		std::string email = this->getDataFromKey<std::string>(req, "email");
		std::string password = this->getDataFromKey<std::string>(req, "password");

		User u;
		u.name = name;
		u.setPwd(Sha256Helper::encrypt(password));
		u.email = email;
		co_await userService.newUserWithPassword(u);
		this->response(callback, "User has been created", k201Created);
		co_return;
	}
};