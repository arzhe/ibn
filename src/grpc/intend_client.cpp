#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "parse/fsm.h"
#include "proto/intend.grpc.pb.h"
#include "base/util/util.h"

#include "indicators/indeterminate_progress_bar.hpp"
#include "indicators/cursor_control.hpp"
#include "indicators/termcolor.hpp"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;
using intend::IntendRequest;
using intend::IntendReply;
using intend::Intend;

class IntendClient {
public:
    IntendClient(std::shared_ptr<Channel> channel) : stub_(Intend::NewStub(channel)) {}

    void IntendChat() {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<IntendRequest, IntendReply>> stream(stub_->IntendChat(&context));

        std::cout << ibn::ColorString<GREEN>("<IBN>") << "Hello and welcome to IBN!" << std::endl;
        std::cout << ibn::ColorString<BLUE>("<IBN>") << "Please tell me which domain you want to access." << std::endl;
        /* std::thread writer([stream]() { */
        /*     std::string cmd; */
        /*     Request request; */
        /*     while(true) { */
        /*         //sleep(1); */
        /*         //std::cout << "\033[32mUser>\033[0m"; */
        /*         if(std::getline(std::cin, cmd, '\n')) { */
        /*             if(!cmd.compare("")) { */
        /*                 std::cout << std::endl; */
        /*                 continue; */
        /*             } */
        /*         } */
        /*         else break; */
        /*         request.set_req(cmd); */
        /*         //std::cout << std::endl; */
        /*         if(!stream->Write(request)) { */
        /*             break; */
        /*         } */
        /*     } */
        /* }); */

        IntendReply reply;
        IntendRequest request;
        do {
            if(reply.reply() != "" && reply.reply() != "indicators") {
                std::cout << reply.reply() << std::endl;
            }
            /* else if(reply.reply() == "indicators") { */
            /*     indicators::IndeterminateProgressBar bar{ */
            /*         indicators::option::BarWidth{40}, */
            /*         indicators::option::Start{"["}, */
            /*         indicators::option::Fill{"·"}, */
            /*         indicators::option::Lead{"<=>"}, */
            /*         indicators::option::End{"]"}, */
            /*         indicators::option::PostfixText{"Updates IBN-Net Policy"}, */
            /*         indicators::option::ForegroundColor{indicators::Color::Yellow}, */
            /*         indicators::option::FontStyles{ */
            /*             std::vector<indicators::FontStyle>{indicators::FontStyle::Bold}} */
            /*     }; */

            /*     indicators::ShowConsoleCursor(false); */

            /*     auto job = [&bar]() { */
            /*     std::this_thread::sleep_for(std::chrono::milliseconds(10000)); */
            /*     bar.MarkAsCompleted(); */
            /*     std::cout << termcolor::Bold << termcolor::Green */ 
            /*         << "System is up to date!\n" << termcolor::Reset; */
            /*     }; */
            /*     std::thread job_completion_thread(job); */

            /*     // Update bar state */
            /*     while (!bar.IsCompleted()) { */
            /*     bar.Tick(); */
            /*     std::this_thread::sleep_for(std::chrono::milliseconds(100)); */
            /*     } */

            /*     job_completion_thread.join(); */

            /*     indicators::ShowConsoleCursor(true); */
            /*     request.set_request("OK"); */
            /*     stream->Write(request); */
            /*     continue; */
            /* } */
            
            std::cout << ibn::ColorString<GREEN>("<User>");
            // std::cout << "\033[32mUser>\033[0m";
            std::string cmd;
            if(std::getline(std::cin, cmd, '\n')) {
                request.set_request(cmd);
            }
            if(!stream->Write(request)) {
                continue;
            }
        } while(stream->Read(&reply));

        /* Reply reply; */
        /* while(stream->Read(&reply)) { */
        /*     std::cout << reply.rep() << std::endl; */
        /*     std::cout << ibn::ColorString<GREEN>("<User>"); */
        /*     // std::cout << "\033[32mUser>\033[0m"; */
        /*     std::string cmd; */
        /*     Request request; */
        /*     if(std::getline(std::cin, cmd, '\n')) { */
        /*         request.set_req(cmd); */
        /*     } */
        /*     if(!stream->Write(request)) { */
        /*         continue; */
        /*     } */
        /* } */

        //writer.join();
        Status status = stream->Finish();
        if(!status.ok()) {
            std::cout << "rpc failed!" << std::endl;
        }
    }

private:
    std::unique_ptr<Intend::Stub> stub_;

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
    else {
        server_address = "localhost:8888";
    }
    IntendClient intend(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    intend.IntendChat();

    return 0;
}

/* #include <iostream> */
/* #include <memory> */
/* #include <string> */

/* #include <grpcpp/grpcpp.h> */
/* #include <grpc/support/log.h> */

/* #include "parse/fsm.h" */
/* #include "proto/intend.grpc.pb.h" */
/* #include "base/util/util.h" */

/* using grpc::Channel; */
/* using grpc::ClientAsyncReaderWriter; */
/* using grpc::ClientContext; */
/* using grpc::CompletionQueue; */
/* using grpc::Status; */
/* using intend::Request; */
/* using intend::Reply; */
/* using intend::Intend; */

/* class IntendClient { */
/* public: */
/*     explicit IntendClient(std::shared_ptr<Channel> channel) */
/*         : stub_(Intend::NewStub(channel)) {} */

/*     std::string IntendChat(const std::string& cmd) { */
/*         Request request; */
/*         request.set_req(cmd); */

/*         Reply reply; */

/*         ClientContext context; */

/*         CompletionQueue cq; */

/*         Status status; */

/*         std::unique_ptr<ClientAsyncReaderWriter<Request, Reply>> rpc( */
/*             stub_->PrepareAsyncIntendChat(&context, request, &cq)); */

/*         rpc->StartCall(); */

/*         rpc->Finish(&reply, &status, (void*)1); */
/*         void* got_tag; */
/*         bool ok = false; */

/*         GPR_ASSERT(cq.Next(&got_tag, &ok)); */
/*         GPR_ASSERT(got_tag == (void*)1); */
/*         GPR_ASSERT(ok); */

/*         if(status.ok()) { */
/*             return reply.rep(); */
/*         } */
/*         else { */
/*             return "RPC failed"; */
/*         } */
/*     } */

/* private: */
/*     std::unique_ptr<Intend::Stub> stub_; */
/* }; */

/* int main(int argc, char** argv) { */
/*     std::string address(argv[1]); */
/*     IntendClient intend(grpc::CreateChannel( */
/*         address, grpc::InsecureChannelCredentials())); */

/*     std::cout << "\033[32mIBN>\033[0mHello and welcome to IBN!" << std::endl; */
/*     std::cout << ibn::ColorString<BLUE>("<IBN>") << "Please tell me which domain you want to access." << std::endl; */
/*     std::cout << ibn::ColorString<GREEN>("<User>"); */
/*     std::string cmd; */
/*     while(std::getline(std::cin, cmd, '\n')) { */
/*         std::string reply = intend.IntendChat(cmd); */
/*         std::cout << reply << std::endl; */
/*         std::cout << ibn::ColorString<GREEN>("<User>"); */
/*     } */
    
/*     return 0; */
/* } */
