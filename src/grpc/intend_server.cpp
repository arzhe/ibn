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
        /* while(1) { */
        /*     auto content = fsm->GetReply(); */
        /*     reply.set_rep(content); */
        /*     stream->Write(reply); */
            
        /*     stream->Read(&request); */
        /*     fsm->Handle(request.req()); */
        /* } */
        
        while(stream->Read(&request)) {
            fsm->Handle(request.req());

            auto content = fsm->GetReply();
            reply.set_rep(content);
            stream->Write(reply);
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

/* #include <memory> */
/* #include <iostream> */
/* #include <string> */
/* #include <thread> */

/* #include <grpcpp/grpcpp.h> */
/* #include <grpc/support/log.h> */

/* #include "parse/fsm.h" */
/* #include "proto/intend.grpc.pb.h" */

/* using grpc::Server; */
/* using grpc::ServerAsyncReaderWriter; */
/* using grpc::ServerBuilder; */
/* using grpc::ServerContext; */
/* using grpc::ServerCompletionQueue; */
/* using grpc::Status; */
/* using intend::Request; */
/* using intend::Reply; */
/* using intend::Intend; */

/* class ServerImpl final { */
/* public: */
/*     ~ServerImpl() { */
/*         server_->Shutdown(); */
/*         // Always shutdown the completion queue after the server. */
/*         cq_->Shutdown(); */
/*     } */

/*     // There is no shutdown handling in this case. */
/*     void Run(std::string server_address) { */
/*         ServerBuilder builder; */
/*         // Listen on the given address without any authentication mechanism. */
/*         builder.AddListeningPort(server_address, grpc::InsecureServerCredentials()); */
/*         // Register "service_" as the instance through which we'll communicate with */
/*         // clients. In this case it corresponds to an *asynchronous* service. */
/*         builder.RegisterService(&service_); */
/*         // Get hold of the completion queue used for the asynchronous communication */
/*         // with the gRPC runtime. */
/*         cq_ = builder.AddCompletionQueue(); */
/*         // Finally assemble the server. */
/*         server_ = builder.BuildAndStart(); */
/*         std::cout << "Server listening on " << server_address << std::endl; */

/*         // Proceed to the server's main loop. */
/*         HandleRpcs(); */
/*     } */

/* private: */
/*     class CallData { */
/*     public: */
/*         CallData(Intend::AsyncService* service, ServerCompletionQueue* cq) */
/*             : service_(service), cq_(cq), stream_(&ctx_), status_(CREATE) { */
/*             fsm_ = ibn::Fsm::NewFsmFromFile("../../doc/ibn_policy.json"); */
/*             Proceed(); */
/*         } */

/*         void Proceed() { */
/*             if(status_ == CREATE) { */
/*                 status_ = INIT_READ; */
/*                 service_->RequestIntendChat(&ctx_, &stream_, cq_, cq_, this); */
/*             } */
/*             else if(status_ == INIT_READ) { */
/*                 new CallData(service_, cq_); */
/*                 stream_.Read(&request_, this); */
/*                 status_ = WRITE; */
/*             } */
/*             else if(status_ == READ) { */
/*                 stream_.Read(&request_, this); */
/*                 status_ = WRITE; */
/*             } */
/*             else if(status_ == WRITE) { */
/*                 Reply reply; */
/*                 auto content = fsm_->GetReply(); */
/*                 reply.set_rep(content); */
/*                 fsm_->Handle(request_.req()); */

/*                 status_ = READ; */
/*             } */
/*             else { */
/*                 stream_.Finish(Status::OK, this); */
/*                 delete this; */
/*             } */
/*         } */

/*     private: */
/*         Intend::AsyncService* service_; */
/*         ServerCompletionQueue* cq_; */
/*         ServerContext ctx_; */
/*         Request request_; */
/*         ServerAsyncReaderWriter<Reply, Request> stream_; */
/*         std::shared_ptr<ibn::Fsm> fsm_; */

/*         enum CallStatus { CREATE, INIT_READ, READ, WRITE }; */
/*         CallStatus status_; */
/*     }; */
    
/*     void HandleRpcs() { */
/*         new CallData(&service_, cq_.get()); */
/*         void* tag; */
/*         bool ok; */
/*         while(true) { */
/*             GPR_ASSERT(cq_->Next(&tag, &ok)); */
/*             GPR_ASSERT(ok); */
/*             static_cast<CallData*>(tag)->Proceed(); */
/*         } */
/*     } */

/*     std::unique_ptr<ServerCompletionQueue> cq_; */
/*     Intend::AsyncService service_; */
/*     std::unique_ptr<Server> server_; */
/* }; */

/* int main(int argc, char** argv) { */
/*     ServerImpl server; */
/*     std::string server_address(argv[1]); */
/*     server.Run(server_address); */
/* } */
