#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

std::string secret_token = "";
std::string auth_prefix = "";

std::string auth = auth_prefix + secret_token;

std::string discord_webhook_url;
std::string proxmox_node_api_url;

//returns a formatted timestamp of the current local time
std::string getTimestamp() {
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}


//send POST (currently only used for discord webhook)
cpr::Response sendPost(std::string url, json j) {
  std::string json_string = j.dump();
  cpr::Response res =
    cpr::Post(cpr::Url{url}, cpr::Body{json_string},
                cpr::Header{{"Content-Type", "application/json"}});
  return res;
}

//send GET (currently only used for proxmox api)
std::string sendGet(std::string url, cpr::Header header, bool verifySSL) {
  cpr::Response r = cpr::Get(cpr::Url{url}, header, cpr::VerifySsl{verifySSL});

  if (r.status_code != 200) {
    return "";
  }

  return r.text;
}

int main() {
  std::cout << "Server Status staprted...\n";

  const char *secret_val = std::getenv("PROXMOX_SECRET");
  if (secret_val == nullptr) {
    std::cerr << "Run without PROXMOX_SECRET environment variable set.\n";
    
    return -1;
  }
  secret_token = secret_val;

  const char *prefix_val = std::getenv("AUTH_PREFIX");
  if (prefix_val == nullptr) {
    std::cerr << "Run without AUTH_PREFIX environment variable set.\n";
    
    return -1;
  }
  auth_prefix = prefix_val;

  auth = auth_prefix + secret_token;

  const char *discord_url = std::getenv("DISCORD_WEBHOOK");
  if (discord_url == nullptr) {
    std::cerr << "Run without DISCORD_WEBHOOK environment variable set\n";
    return -1;
  }
  discord_webhook_url = discord_url;

  const char *proxmox_url = std::getenv("PROXMOX_API_URL");
  if (proxmox_url == nullptr) {
    std::cerr << "Run without PROXMOX_API_URL environment variable set\n";
    return -1;
  }
  proxmox_node_api_url = proxmox_url;

  
  std::string text = sendGet("proxmox_node_api_url", cpr::Header{{"Authorization", auth}}, false);

  if (text == "") {
    std::cout << "GET responded with status_code not 200\n";

    json j = {{"content", getTimestamp() + "\nERROR with GET. responded with non-200 status."}};
    cpr::Response r = sendPost("discord_webhook_url", j);
    std::cout << "Response with status code: " << r.status_code << "\n";
    
  } else {
    bool bad_status = false;
    json response = json::parse(text);

    std::vector<std::string> node_responses;
    
    for (const auto &node : response["data"]) {
      std::string name = node["node"];
      std::string status = node["status"];
      uint64_t uptime = node["uptime"];

      std::string my_response = "Name: " + name + ", Status: " + status + ", Uptime: " + std::to_string(uptime);
      std::cout << my_response << "\n";
      
      node_responses.push_back(my_response);
      
      if (status != "online") {
        bad_status = true;
      }
    }

    if (bad_status) {
      std::string string_responses = "";
      for (std::string str : node_responses) {
        string_responses += str;
        string_responses += "\n";
      }
      
      std::string payload = getTimestamp() + "\nERROR with status. Response:\n" + string_responses;
      json j = {{"content", payload}};
      cpr::Response r = sendPost("discord_webhook_url", j);
      std::cout << "Response with status code: " << r.status_code << "\n";
    }
  }
  
  return 0;
}
