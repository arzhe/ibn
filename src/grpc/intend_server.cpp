#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "parse/fsm.h"
#include "proto/intend.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using intend::Request;
using intend::Reply;
using intend::Intend;

class IntendImpl final : public Intend::Service {
public:
    Status IntendChat(ServerContext* context,
                      ServerReaderWriter<Reply, Request>* stream) override {
        (void) context;
        Request request;
        Reply reply;
        auto fsm = ibn::Fsm::NewFsmFromFile("../../doc/ibn_policy.json");
        while(1) {
            auto content = fsm->GetReply();
            reply.set_rep(content);
            stream->Write(reply);
            
            stream->Read(&request);
            fsm->Handle(request.req());
        }

        /* while(stream->Read(&request)) { */
        /*     if(!(request.req()).compare("domain")) { */
        /*         reply.set_rep("\033[32mIBN>\033[0mTell me what do you want the net to do?\n\033[31m[HINT]>\033[0mI want to \033[31m[connect][disconnect]\033[0m from Wuhan \033[31m[to]\033[0m Guangdong."); */
        /*     } */
        /*     else if(!(request.req()).compare("connect wuhan to guangdong")) { */
        /*         reply.set_rep("\033[32mIBN>\033[0mI'm setting these up for you.\n....\n....\nSuccess!\n"); */
        /*     } */
        /*     stream->Write(reply); */
        /* } */
        return Status::OK;
    }
};

int main(int argc, char** argv) {
    std::string server_address("0.0.0.0:8888");
    IntendImpl service;
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

    return 0;
}
