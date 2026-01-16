#pragma once
#include <drogon/HttpController.h>
#include "helpers/RequestHelper.h"
#include "helpers/Base64Helper.h"

using namespace drogon;

template <typename T>
class BaseController : public HttpController<T> {



protected: void response(const std::function<void(const HttpResponsePtr&)>& callback,
	const Json::Value& data,
	HttpStatusCode code = k200OK, bool error =false)
{
	Json::Value json;
	json["data"] = data;
	json["code"] = (int)code;
	json["error"] = error;

	auto res = HttpResponse::newHttpJsonResponse(json);
	res->setStatusCode(code);
	callback(res);
}

protected:
	template <typename A>
	A getDataFromKey(HttpRequestPtr req, std::string k) {

		return RequestHelper::getDataFromKey<A>(*req->getJsonObject(), k);

	}

protected:
	std::string getHeader(HttpRequestPtr r, std::string k) {
		return RequestHelper::getHeader(r, k);
	}

protected:
	std::string getAuthorization(HttpRequestPtr r) {
		return r->getHeader("Authorization");
	}

protected :
	Json::Value getDataFromBasicAuthorization(const std::string& basic){
		auto token = basic.substr(6);

		return Base64Helper::getJsonFromJson64(token);
	}


};