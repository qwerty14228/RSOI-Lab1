#ifndef SERVER_H 
#define SERVER_H

#include <string>

#include <boost/json.hpp>

#include "third_party/beauty_server.hpp"

#include "logic/PersonLogic.h"

    class Server {
public: 
    Server(std::shared_ptr<PersonLogic> &logic, int port): logic(logic), port(port){}

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

                auto name = boost::json::value_to<std::string>(json_body.at("name"));
                auto work = boost::json::value_to<std::string>(json_body.at("work"));
                auto address = boost::json::value_to<std::string>(json_body.at("address"));
                auto age = boost::json::value_to<int>(json_body.at("age"));

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
            })
            .del([this](const auto& req, auto& res) {
                auto id = req.a("id").as_integer();

                this->logic->Delete(id);

                res.result(beast::http::status::no_content);
            })
            .patch([this](const auto& req, auto& res) {
                auto id = req.a("id").as_integer();
                
                auto person = this->logic->getById(id);

                if (!person) {
                    throw beauty::http_error::client::not_found("Not found");
                }

                auto name = person->name;
                auto work = person->work;
                auto address = person->address;
                auto age = person->age;

                auto body = req.body();
                auto json_body = boost::json::parse(body).as_object();

                if (json_body.contains("name")) {
                    name = boost::json::value_to<std::string>(json_body.at("name"));
                }

                if (json_body.contains("work")) {
                    work = boost::json::value_to<std::string>(json_body.at("work"));
                }

                if (json_body.contains("address")) {
                    address = boost::json::value_to<std::string>(json_body.at("address"));
                }

                if (json_body.contains("age")) {
                    age = boost::json::value_to<int>(json_body.at("age"));
                }

                this->logic->Update(id, name, work, address, age);

                boost::json::object person_object = {
                    {"id", id},
                    {"name", name},
                    {"work", work},
                    {"address", address},
                    {"age", age}
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
