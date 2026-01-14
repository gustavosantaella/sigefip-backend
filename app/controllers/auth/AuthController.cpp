#include "../../shared/BaseController.h"
#include "../../shared/helpers/Sha256Helper.h"
#include "../../config/constants/constants.h"
#include "../../services/UserService.h"
#include "../../entities/User.h"
#include <drogon/utils/coroutine.h>
#include <jwt-cpp/jwt.h>


class AuthController : public BaseController<AuthController> {

	UserService userService;

public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(AuthController::login, API_PREFIX "/auth/login", Post);
	ADD_METHOD_TO(AuthController::signUp, API_PREFIX "/auth/sign-up", Post);
	METHOD_LIST_END;

	AuthController() : userService() {
	};

public:
	Task<void> login(
		HttpRequestPtr req,
		std::function<void(const HttpResponsePtr&)> callback
	)
	{
		auto basicHeader = this->getAuthorization(req);
		auto jsonData = this->getDataFromBasicAuthorization(basicHeader);

		std::string email = jsonData["email"].asString();
		std::string pwd = jsonData["pwd"].asString();

		std::optional<User> user = co_await userService.findByEmail(email);
		if (!user) {
			this->response(callback, "User Not Found", k404NotFound, true);
			co_return;
		}

		if (!Sha256Helper::verify(pwd, user->getPwd())) {
			this->response(callback, "Incorrect password, please try again.", k400BadRequest, true);
			co_return;
		}

		if (user.has_value()) {
			auto token = jwt::create()
				.set_type("JWS")
				.set_issuer("auth0")
				.set_payload_claim("sample", jwt::claim(std::string("test")))
				.sign(jwt::algorithm::hs256{ "secret" });
				
			this->response(callback, "");
		}
		else {
			auto resp = HttpResponse::newHttpJsonResponse({ {"error", "Unauthorized"} });
			resp->setStatusCode(k401Unauthorized);
			callback(resp);
		}

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