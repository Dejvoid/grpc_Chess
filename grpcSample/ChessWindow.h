#pragma once
#include <gtkmm/window.h>
#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"
#include <gtkmm.h>
#include "Sector.h"
#include <gtkmm/stylecontext.h>
#include <gtkmm/cssprovider.h>

class ChessWindow : public Gtk::Window
{
public:
	ChessWindow();
	virtual ~ChessWindow();
	void joinRoom();
	GrpcGameService::Room* room;
	std::string gameId;
	std::shared_ptr<GrpcGameService::RoomService::Stub> Client;
	void recieveMoves();
	
protected:
	bool onTurn;
	GrpcGameService::Color color;
	bool afterStart;
	void moveRecieved();
	void buttonClicked(int x, int y);
	Sector **buttons;
	Gtk::Grid btnGrid;
	bool clickedOn;
	Sector* lastClicked;
	Glib::RefPtr<Gtk::CssProvider> css_red;
	Glib::RefPtr<Gtk::CssProvider> css_green;
	Glib::RefPtr<Gtk::CssProvider> css_white;
	Glib::RefPtr<Gtk::CssProvider> css_black;

	Glib::Dispatcher dispatcher;
	std::thread* listenerThread;
	GrpcGameService::GameMove lastMove;

	void showIcons();
	void drawIcon(int y, int x);
};
