#pragma once
#include <drogon/orm/DbClient.h>
#include <drogon/utils/coroutine.h> 
#include <drogon/drogon.h>
#include <string>
#include <optional>
#include "../entities/User.h"
#include <iostream>

using namespace drogon;
using namespace drogon::orm;

class UserRepository {
public:
	static Task<std::optional<User>> findByEmail(std::string email) {
		auto dbClient = drogon::app().getDbClient();
		auto sql = "SELECT id, name, email, password FROM users WHERE email = $1";

		try {
			// Use co_await directly on the future returned by execSqlAsyncFuture
			const auto& r = co_await dbClient->execSqlCoro(sql, email);


			if (r.empty()) {
				std::cout << "User not found" << std::endl;

				co_return std::nullopt;

			}
			const auto& row = r[0];
			User user;
			user.id = row["id"].as<int>();
			user.name = row["name"].as<std::string>();
			user.email = row["email"].as<std::string>();
			user.setPwd(row["password"].as<std::string>());

			co_return user;
		}
		catch (const DrogonDbException& e) {
			LOG_ERROR << "Error en base de datos: " << e.base().what();
			std::cout << "ERRO TO GET USER" + email << std::endl;

		}

		co_return std::nullopt;
	}


	static Task<void> newUserWithPassword(User u) {
		auto sql = "INSERT INTO users (name, email, password ) VALUES ($1, $2, $3)";
		auto dbClient = drogon::app().getDbClient();

		co_await dbClient->execSqlCoro(sql, u.name, u.email, u.getPwd());

		co_return;
	}
};