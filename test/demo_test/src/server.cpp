#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "parse/fsm.h"
#include "proto/demo.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReadWriter;
using grpc::Status;
using demo::Request;
using demo::Reply;
using demo::Diag;
using demo::INT;
using demo::Feedback;
using demo::Issue;



class ServerImpl final {
public:
    bool update;

    void Run() {
        std::string server_address = "0.0.0.0:8888";
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;
        server->Wait();
    }
    
private:
    class ChatImp final : public Demo::Service {
    public:
        Status Diag(ServerContext* context,
                    ServerReadWriter<Reply, Request>* stream) override {
            (void) context;
            Request request;
            Reply reply;
            auto fsm = ibn::Fsm::NewFsmFromFile("../../doc/policy.json");

            while(stream->Read(&request)) {
                fsm->Handle(request.req());

                auto content = fsm->GetReply();
                if(update) {
                    std::cout << "update from grpc!" << std:endl;
                }
                repply.set_rep(content);
                stream->Write(reply);
            }

            return Status::OK;
        }

        Status INT(ServerContext* context,
                   ServerReadWriter<Issue, Feedback>* stream) override {
            (void) context;
            Feedback fb;
            Issue    is;
            while(stream->Read(&fb)) {
                if(fb.count() % 5 == 0) {
                    is.set_update = true;
                    update = true;
                }
                else {
                    is.set_update = false;
                }
                stream->Write(is);
            }
            return Status::OK;
            
        }
    };

    ChatImp service;
    ServerBuilder builder;
};

int main() {
    ServerImpl server;
    server.Run();
    return 0;
}
