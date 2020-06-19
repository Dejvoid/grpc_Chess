#include <iostream>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <gtkmm.h>

#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"
#include "ChessLobby.h"

using namespace std;
using namespace grpc;
using namespace google::protobuf;
using namespace GrpcGameService;


int main(int    argc,
	char** argv){
	auto app = Gtk::Application::create(argc, argv);
	ChessLobby window;
	return app->run(window);

}