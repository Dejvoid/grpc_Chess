#pragma once
#include "Grpc/GameService.pb.h"
#include <gtkmm.h>
class Sector{
public:
	Sector();
	~Sector();
	int posX;
	int posY;
	
	GrpcGameService::Color color;
	GrpcGameService::Figurine figurine;
	void getMoves(Sector**buttons,int x, int y);
	bool moved;
	std::vector<std::tuple<int, int>> availableMoves; //y,x
	Gtk::Button* button;
};
