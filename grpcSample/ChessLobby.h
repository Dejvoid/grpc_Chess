#pragma once
#include <gtkmm/window.h>
#include<gtkmm/button.h>
#include<gtkmm/combobox.h>
#include <vector>
#include <gtkmm.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"

class ChessLobby : public Gtk::Window
{
public:
	ChessLobby();
	virtual ~ChessLobby();
	
protected:
	Gtk::Button createRoom_btn, joinRoom_btn;
	Gtk::ComboBoxText rooms_cbox;
	Gtk::Box room_box;
	Gtk::Entry newRoom_txt;
	std::shared_ptr<GrpcGameService::RoomService::Stub> Client;
	google::protobuf::RepeatedPtrField<GrpcGameService::Room> Rooms;
	void onCreate_clicked();
	void onJoin_clicked();
	void refreshRooms();
};

