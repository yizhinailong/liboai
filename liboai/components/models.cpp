#include "../include/components/models.h"

auto liboai::Models::list() const & noexcept(false) -> liboai::Response {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/models", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

auto liboai::Models::list_async() const & noexcept(false) -> liboai::FutureResponse {
	return std::async(std::launch::async, &liboai::Models::list, this);
}

auto liboai::Models::retrieve(const std::string& model) const & noexcept(false) -> liboai::Response {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

auto liboai::Models::retrieve_async(const std::string& model) const & noexcept(false) -> liboai::FutureResponse {
	return std::async(std::launch::async, &liboai::Models::retrieve, this, model);
}
