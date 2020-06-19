#include "ChessLobby.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"
#include <iostream>
#include "ChessWindow.h"

using namespace grpc;
using namespace google::protobuf;
using namespace GrpcGameService;
using namespace std;

ChessLobby::ChessLobby()
	:createRoom_btn("Create room"),
	joinRoom_btn("Join room")
{
	set_title("Chess Lobby");
	set_border_width(10);
	add(room_box);

	auto ssl_options = grpc::SslCredentialsOptions();
	auto ssl = grpc::SslCredentials(ssl_options);
	auto channel = CreateChannel("wiki.kyberna.cz:5002", ssl);
	RoomService* rs = new RoomService();
	Client = rs->NewStub(channel, grpc::StubOptions());
	ClientContext ctx;
	RoomListRequest req;
	RoomListResponse resp;
	auto conStatus = Client->List(&ctx, req, &resp);
	if (conStatus.ok()) {
		Rooms = resp.rooms();
		for (auto item : Rooms) {
			cout << item.name() << endl << flush;
			rooms_cbox.append(item.name());
		}
	}
	room_box.pack_start(rooms_cbox);
	rooms_cbox.show();

	joinRoom_btn.signal_clicked().connect(
		sigc::mem_fun(*this, &ChessLobby::onJoin_clicked));
	room_box.pack_start(joinRoom_btn);
	joinRoom_btn.show();

	room_box.pack_start(newRoom_txt);
	newRoom_txt.set_text("NewRoomName");
	newRoom_txt.show();

	createRoom_btn.signal_clicked().connect(
		sigc::mem_fun(*this, &ChessLobby::onCreate_clicked));
	room_box.pack_start(createRoom_btn);
	createRoom_btn.show();
	
	room_box.show();
}

ChessLobby::~ChessLobby()
{
}

void ChessLobby::onCreate_clicked()
{
	string roomName = newRoom_txt.get_text();
	cout << roomName << endl << flush;
	ClientContext ctx;
	RoomCreateRequest req1;
	RoomCreateResponse resp;
	req1.set_name(roomName);
	auto connStat = Client->Create(&ctx, req1, &resp);
	if (connStat.ok()) {
		auto room = resp.room();
		ChessWindow* cw = new ChessWindow;
		cw->room = new Room();
		cw->Client = Client;
		cw->room->set_name(room.name());
		cw->room->set_roomid(room.roomid());
		cout << "successfully created room: " + cw->room->name() << endl << flush;
		refreshRooms();
		cw->show();
		cw->joinRoom();
	}
	
}
void ChessLobby::onJoin_clicked()
{
	string roomName = rooms_cbox.get_active_text().c_str();
	cout << roomName << endl << flush;
	ChessWindow* cw = new ChessWindow;
	cw->room = new Room();
	cw->Client = Client;
	for (int i = 0; i < Rooms.size(); i++)
	{
		if (Rooms[i].name() == roomName) {
			cw->room->set_name(roomName);
			cw->room->set_roomid(Rooms[i].roomid());
		}
	}
	cw->show();
	cw->joinRoom();
}

void ChessLobby::refreshRooms()
{
	ClientContext ctx;
	RoomListRequest req;
	RoomListResponse resp;
	auto conStatus = Client->List(&ctx, req, &resp);
	if (conStatus.ok()) {
		Rooms = resp.rooms();
		for (auto item : Rooms) {
			cout << item.name() << endl << flush;
			rooms_cbox.append(item.name());
		}
	}
}




