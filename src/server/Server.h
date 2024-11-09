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
        this->server.add_route("/persons/").get([this](const auto& req, auto& res){
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
        });

        this->server.add_route("/person/:id").get([this](const auto& req, auto& res) {
            auto id = req.a("id").as_int();

            auto person = this->logic->getById(id);

            if (person) {
                boost::json::object person_object = {
                    {"id", person->id},
                    {"name", person->name},
                    {"work", person->work},
                    {"address", person->address},
                    {"age", person->age}
                };

                res.body() = boost::json::serialize(person_object);
                res.set_header(beast::http::field::content_type, "application/json");
                return;
            }

            res.set(beast::http::status::not_found);
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
