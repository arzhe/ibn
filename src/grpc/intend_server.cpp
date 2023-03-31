#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "parse/fsm.h"
#include "log/logfile.h"
#include "log/file_util.h"
#include "log/logging.h"
#include "proto/intend.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using intend::IntendRequest;
using intend::IntendReply;
using intend::Intend;
using intend::RealtimeInfo;
using intend::PolicyInfo;

std::unique_ptr<ibn::LogFile> g_log_file;

void OutputFunc(const char* msg, int len) {
    g_log_file->Append(msg, len);
}

void FlushFunc() {
    g_log_file->Flush();
}

class IntendImpl final : public Intend::Service {
public:
    Status IntendChat(ServerContext* context,
                      ServerReaderWriter<IntendReply, IntendRequest>* stream) override {
        (void) context;
        IntendRequest request;
        IntendReply reply;
        std::string completed = "indicators";
        auto fsm = ibn::Fsm::NewFsmFromFile("../../doc/ibn_policy.json");
        
        while(stream->Read(&request)) {
            if(request.request() == "OK") {
                auto content = fsm->GetReply();
                reply.set_reply(content);
            }
            else {
                fsm->Handle(request.request());
                if(fsm->IsCompleted()) {
                    reply.set_reply(completed);
                }
                else {
                    auto content = fsm->GetReply();
                    reply.set_reply(content);
                }
            }
            stream->Write(reply);

            /* if(data % 2 == 0) { */
            /*     std::cout << "recv from INT" << std::endl; */
            /* } */
        }

        return Status::OK;
    }

    /* Status IntChat(ServerContext* context, */
    /*                ServerReaderWriter<PolicyInfo, RealtimeInfo>* stream) override { */
    /*     (void) context; */
    /*     RealtimeInfo rt_info; */
    /*     PolicyInfo p_info; */
        
    /*     while(stream->Read(&rt_info)) { */
    /*         data = stoi(rt_info.rtinfo()); */
    /*         auto content = rt_info.rtinfo() + " from INT"; */
    /*         std::cout << content << std::endl; */
    /*         p_info.set_pinfo(content); */
    /*         stream->Write(p_info); */
    /*     } */

    /*     return Status::OK; */
    /* } */

/* private: */
/*     int data; */
};

int main(int argc, char** argv) {
    std::string server_address;
    std::string address, port;
    if(argc == 3) {
        address = argv[1];
        port = argv[2];
        server_address = address + ":" + port;
    }
    else if(argc == 2) {
        address = argv[1];
        server_address = address + ":8888";
    }
    else if(argc == 1) {
        server_address = "0.0.0.0:8888";
    }
    else {
        std::cout << "Wrong server_address parameters!";
        return 0;
    }

    char name[256] = { '\0' };
    strncpy(name, argv[0], sizeof name - 1);
    g_log_file.reset(new ibn::LogFile(::basename(name), 200 * 1000));
    ibn::Logger::SetOutput(OutputFunc);
    ibn::Logger::SetFlush(FlushFunc);

    IntendImpl service;
    
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

    return 0;
}
