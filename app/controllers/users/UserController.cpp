#include "../../shared/BaseController.h"

#include "../../config/constants/constants.h"


    class UserController : public BaseController<UserController> {
    public:
        METHOD_LIST_BEGIN
            ADD_METHOD_TO(UserController::test, API_PREFIX "/users/test", Get);
        METHOD_LIST_END;

    public:
        void test(const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value data;
            data["status"] = "Success";
            data["message"] = "Architecture is working!";
            this->response(callback, data);
        }

  
    };
