#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "proto/intend.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;
using intend::Intend;
using intend::RealtimeInfo;
using intend::PolicyInfo;

class IntClient {
public:
    IntClient(std::shared_ptr<Channel> channel) : stub_(Intend::NewStub(channel)) {}

    void IntChat() {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<RealtimeInfo, PolicyInfo>> stream(stub_->IntChat(&context));


        PolicyInfo p_info;
        int count = 0;
        do {
            if(p_info.pinfo() != "") {
                std::cout << p_info.pinfo() << std::endl;
            }
            RealtimeInfo rt_info;
            std::string res = std::to_string(count);
            ++count;
            rt_info.set_rtinfo(res);
            stream->Write(rt_info);
            sleep(2);
        } while(stream->Read(&p_info));

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
    if(argc != 1) {
        server_address = argv[1];
    }
    else {
        server_address = "localhost:8888";
    }
    IntClient intend(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    intend.IntChat();

    return 0;
}
