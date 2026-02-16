#include "../include/components/files.h"

auto liboai::Files::list() const & noexcept(false) -> liboai::Response {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/files", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

auto liboai::Files::list_async() const & noexcept(false) -> liboai::FutureResponse {
	return std::async(std::launch::async, &liboai::Files::list, this);
}

auto liboai::Files::create(const std::filesystem::path& file, const std::string& purpose) const & noexcept(false) -> liboai::Response {
	if (!this->Validate(file)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Files::create(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "purpose", purpose },
		{ "file", netimpl::components::File{file.generic_string()} }
	};
	
	Response res;
	res = this->Request(
		Method::HTTP_POST, this->openai_root_, "/files", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

auto liboai::Files::create_async(const std::filesystem::path& file, const std::string& purpose) const & noexcept(false) -> liboai::FutureResponse {
	return std::async(std::launch::async, &liboai::Files::create, this, file, purpose);
}

auto liboai::Files::remove(const std::string& file_id) const & noexcept(false) -> liboai::Response {
	Response res;
	res = this->Request(
		Method::HTTP_DELETE, this->openai_root_, "/files/" + file_id, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

auto liboai::Files::remove_async(const std::string& file_id) const & noexcept(false) -> liboai::FutureResponse {
	return std::async(std::launch::async, &liboai::Files::remove, this, file_id);
}

auto liboai::Files::retrieve(const std::string& file_id) const & -> liboai::Response {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/files/" + file_id, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

auto liboai::Files::retrieve_async(const std::string& file_id) const & noexcept(false) -> liboai::FutureResponse {
	return std::async(std::launch::async, &liboai::Files::retrieve, this, file_id);
}

auto liboai::Files::download(const std::string& file_id, const std::string& save_to) const & noexcept(false) -> bool {
	return Network::Download(
		save_to,
		("https://api.openai.com/v1/files/" + file_id + "/content"),
		this->auth_.GetAuthorizationHeaders()
	);
}

auto liboai::Files::download_async(const std::string& file_id, const std::string& save_to) const & noexcept(false) -> std::future<bool> {
	return std::async(std::launch::async, &liboai::Files::download, this, file_id, save_to);
}
