
#include "../entities/User.h"
#include "../repositories/UserRepository.h"
#include <drogon/utils/coroutine.h>
class UserService {


	public: UserService() {
	}

public:
	Task<std::optional<User>> findByEmail(std::string email) {

		const auto r = co_await UserRepository::findByEmail(email);
	
		co_return r;
	}


	Task<void> newUserWithPassword(User u) {
		co_await UserRepository::newUserWithPassword(u);
	}

};