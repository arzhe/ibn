#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "parse/fsm.h"
#include "proto/intend.grpc.pb.h"
#include "base/util/util.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;
using intend::Request;
using intend::Reply;
using intend::Intend;

class IntendClient {
public:
    IntendClient(std::shared_ptr<Channel> channel) : stub_(Intend::NewStub(channel)) {}

    void IntendChat() {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<Request, Reply>> stream(stub_->IntendChat(&context));

        std::cout << "\033[32mIBN>\033[0mHello and welcome to IBN!" << std::endl;
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

        Reply reply;
        while(stream->Read(&reply)) {
            std::cout << reply.rep() << std::endl;
            std::cout << ibn::ColorString<GREEN>("<User>");
            // std::cout << "\033[32mUser>\033[0m";
            std::string cmd;
            Request request;
            if(std::getline(std::cin, cmd, '\n')) {
                request.set_req(cmd);
            }
            if(!stream->Write(request)) {
                continue;
            }
        }

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
    IntendClient intend(grpc::CreateChannel("localhost:8888", grpc::InsecureChannelCredentials()));

    intend.IntendChat();

    return 0;
}


