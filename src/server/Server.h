#ifndef SERVER_H 
#define SERVER_H

#include <string>

#include <beauty/beauty.hpp>
#include <boost/json.hpp>

    class Server {
public: 
    Server(int port): port(port){}

    void get_handler(const beauty::request& req, beauty::response& res) {
        boost::json::object test_object = {
            {"test", "it works and deployed from hook!"}
        };

        res.body() = boost::json::serialize(test_object);
        res.set_header(beast::http::field::content_type, "application/json");
    }

    void run() {
        // '/'
        this->server.add_route("/").get([this](const auto& req, auto& res){
            this->get_handler(req, res);
        });

        // '/person/:id'
        this->server.add_route("/person/:id")
        // Get id information
        .get([](const auto& req, auto& res) {
            auto id = req.a("id").as_string();
            std::string response = "Id: " + id + " not found";
            res.body() = response;
        });

        this->server.listen(this->port);

        this->server.run();
    }

private: 
    beauty::server server;
    int port;
};
#endif
