#ifndef SERVER_H 
#define SERVER_H

#include <string>

#include <beauty/beauty.hpp>
#include <boost/json.hpp>

#include "logic/PersonLogic.h"

    class Server {
public: 
    Server(std::shared_ptr<PersonLogic> &logic, int port): logic(logic), port(port){}

    void get_handler(const beauty::request& req, beauty::response& res) {
        boost::json::object test_object = {
            {"test", "it works and deployed from hook!"}
        };

        res.body() = boost::json::serialize(test_object);
        res.set_header(beast::http::field::content_type, "application/json");
    }

    void run() {
        this->server
            .add_route("/api/v1/persons")
            .get([this](const auto& req, auto& res){
                boost::json::array array;

                for (auto &person : this->logic->getAll()) {
                    array.push_back(boost::json::object{
                        {"id", person->id},
                        {"name", person->name},
                        {"work", person->work},
                        {"address", person->address},
                        {"age", person->age}
                    });
                }

                res.body() = boost::json::serialize(array);
                res.set_header(beast::http::field::content_type, "application/json");
            })
            .post([this](const auto& req, auto& res) {
                auto body = req.body();
                auto json_body = boost::json::parse(body);

                if (
                    !json_body.is_object() ||
                    !json_body.contains("name") ||
                    !json_body.contains("work") ||
                    !json_body.contains("address") ||
                    !json_body.contains("age")
                ) {
                    throw beauty::http_error::client::bad_request("Bad Request");
                }

                auto name = json_body["name"].as<std::string>();
                auto work = json_body["work"].as<std::string>();
                auto address = json_body["address"].as<std::string>();
                auto age = json_body["age"].as<int>();

                auto person = this->logic->Create(name, work, address, age);

                if (!person) {
                    throw beauty::http_error::client::bad_request("Bad Request");
                }

                res.result(beast::http::status::created);
            
                res.set_header(beast::http::field::location, "/api/v1/persons/" + std::to_string(person->id));
            });

        this->server
            .add_route("/api/v1/persons/:id")
            .get([this](const auto& req, auto& res) {
                auto id = req.a("id").as_integer();

                auto person = this->logic->getById(id);

                if (!person) {
                    throw beauty::http_error::client::not_found("Not found");
                }

                boost::json::object person_object = {
                    {"id", person->id},
                    {"name", person->name},
                    {"work", person->work},
                    {"address", person->address},
                    {"age", person->age}
                };

                res.body() = boost::json::serialize(person_object);
                res.set_header(beast::http::field::content_type, "application/json");
            });

        this->server.listen(this->port);

        this->server.run();
    }

private: 
    beauty::server server;
    std::shared_ptr<PersonLogic> logic;
    int port;
};
#endif
