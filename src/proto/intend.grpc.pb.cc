// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: intend.proto

#include "intend.pb.h"
#include "intend.grpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace intend {

static const char* Intend_method_names[] = {
  "/intend.Intend/IntendChat",
};

std::unique_ptr< Intend::Stub> Intend::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Intend::Stub> stub(new Intend::Stub(channel));
  return stub;
}

Intend::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_IntendChat_(Intend_method_names[0], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReaderWriter< ::intend::Request, ::intend::Reply>* Intend::Stub::IntendChatRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::intend::Request, ::intend::Reply>::Create(channel_.get(), rpcmethod_IntendChat_, context);
}

::grpc::ClientAsyncReaderWriter< ::intend::Request, ::intend::Reply>* Intend::Stub::AsyncIntendChatRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::intend::Request, ::intend::Reply>::Create(channel_.get(), cq, rpcmethod_IntendChat_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::intend::Request, ::intend::Reply>* Intend::Stub::PrepareAsyncIntendChatRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::intend::Request, ::intend::Reply>::Create(channel_.get(), cq, rpcmethod_IntendChat_, context, false, nullptr);
}

Intend::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Intend_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< Intend::Service, ::intend::Request, ::intend::Reply>(
          std::mem_fn(&Intend::Service::IntendChat), this)));
}

Intend::Service::~Service() {
}

::grpc::Status Intend::Service::IntendChat(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::intend::Reply, ::intend::Request>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace intend
