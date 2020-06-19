#include "Sector.h"
#include <gtkmm/cssprovider.h>

Sector::Sector()
{
	moved = false;
	availableMoves = std::vector<std::tuple<int, int>>();
}

Sector::~Sector()
{
}

void Sector::getMoves(Sector **buttons, int x, int y)
{
	availableMoves.clear();
	switch (figurine) {

#pragma region King
	case GrpcGameService::King://///////////////////////////////////////////////////////////////
		if (!moved) {
			//TODO: ROSADA
		}
		if (x+1 < 8 && buttons[y][x + 1].figurine == GrpcGameService::None || buttons[y][x + 1].color!=color)
			availableMoves.push_back(std::tuple<int, int>(y, x + 1));
		if (y+1 < 8 && buttons[y + 1][x].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y + 1, x));
		if (x-1 >= 0 && buttons[y][x - 1].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y, x - 1));
		if (y-1 >= 0 && buttons[y - 1][x].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y - 1, x));
		if (y+1 < 8 && x+1 < 8 && buttons[y + 1][x + 1].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y + 1, x + 1));
		if (y-1 >= 0 && x+1 < 8 && buttons[y - 1][x + 1].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y - 1, x + 1));
		if (y+1 < 8 && x-1 >= 0 && buttons[y + 1][x - 1].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y + 1, x - 1));
		if (y-1 >= 0 && x-1 >= 0 && buttons[y - 1][x - 1].figurine == GrpcGameService::None || buttons[y][x + 1].color != color)
			availableMoves.push_back(std::tuple<int, int>(y - 1, x - 1));
		break;
#pragma endregion

#pragma region Queen
	case GrpcGameService::Queen:////////////////////////////////////////////////////////////
		for (int x2 = x + 1, y2 = y + 1; x2 < 8 && y2 < 8; x2++, y2++) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = 9;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = 9;
			}
		}
		for (int x2 = x + 1, y2 = y - 1; x2 < 8 && y2 >= 0; x2++, y2--) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = -1;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = -1;
			}
		}
		for (int x2 = x - 1, y2 = y + 1; x2 >= 0 && y2 < 8; x2--, y2++) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = 9;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = 9;
			}
		}
		for (int x2 = x - 1, y2 = y - 1; x2 >= 0 && y2 >= 0; x2--, y2--) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = -1;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = -1;
			}
		}
		for (int x2 = x + 1; x2 < 8; x2++) {
			std::cout << "y" << y << " x" << x2 << std::endl << std::flush;
			if (buttons[y][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
			}
			else if (buttons[y][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
				x2 = 9;
			}
			else if (buttons[y][x2].color == color)
				x2 = 9;
		}
		for (int y2 = y + 1; y2 < 8; y2++) {
			std::cout << "y" << y2 << " x" << x << std::endl << std::flush;
			if (buttons[y2][x].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
			}
			else if (buttons[y2][x].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
				y2 = 9;
			}
			else if (buttons[y2][x].color == color)
				y2 = 9;
		}
		for (int x2 = x - 1; x2 >= 0; x2--) {
			std::cout << "y" << y << " x" << x2 << std::endl << std::flush;
			if (buttons[y][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
			}
			else if (buttons[y][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
				x2 = -1;
			}
			else if (buttons[y][x2].color == color)
				x2 = -1;
		}
		for (int y2 = y - 1; y2 >= 0; y2--) {
			std::cout << "y" << y2 << " x" << x << std::endl << std::flush;
			if (buttons[y2][x].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
			}
			else if (buttons[y2][x].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
				y2 = -1;
			}
			else if (buttons[y2][x].color == color)
				y2 = -1;
		}
		break;
#pragma endregion

#pragma region Rook
	case GrpcGameService::Rook:////////////////////////////////////////////////////////////////
		for (int x2 = x + 1; x2 < 8; x2++) {
			std::cout << "y" << y << " x" << x2 << std::endl << std::flush;
			if (buttons[y][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
			}
			else if (buttons[y][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
				x2 = 9;
			}
			else if (buttons[y][x2].color == color)
				x2 = 9;
		}
		for (int y2 = y + 1; y2 < 8; y2++) {
			std::cout << "y" << y2 << " x" << x << std::endl << std::flush;
			if (buttons[y2][x].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
			}
			else if (buttons[y2][x].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
				y2 = 9;
			}
			else if (buttons[y2][x].color == color)
				y2 = 9;
		}
		for (int x2 = x - 1; x2 >= 0; x2--) {
			std::cout << "y" << y << " x" << x2 << std::endl << std::flush;
			if (buttons[y][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
			}
			else if (buttons[y][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y, x2));
				x2 = -1;
			}
			else if (buttons[y][x2].color == color)
				x2 = -1;
		}
		for (int y2 = y - 1; y2 >= 0; y2--) {
			std::cout << "y" << y2 << " x"<< x <<std::endl << std::flush;
			if (buttons[y2][x].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
			}
			else if (buttons[y2][x].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x));
				y2 = -1;
			}
			else if (buttons[y2][x].color == color)
				y2 = -1;
		}
		break;
#pragma endregion

#pragma region Knight
	case GrpcGameService::Knight://///////////////////////////////////////////////////////////////
		//x+=2 y+=1
		if (x + 2 < 8 && y + 1 < 8 && buttons[y + 1][x + 2].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y + 1, x + 2));
		}
		else if (x + 2 < 8 && y + 1 < 8 && buttons[y + 1][x + 2].color != color) {
			std::cout << "selected color: "<< GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y + 1, x + 2));
		}
		//x+=2 y-=1
		if (x + 2 < 8 && y - 1 >= 0 && buttons[y - 1][x + 2].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y - 1, x + 2));
		}
		else if (x + 2 < 8 && y - 1 >= 0 && buttons[y - 1][x + 2].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y - 1, x + 2));
		}
		//x-=2 y+=1
		if (x - 2 >= 0 && y + 1 < 8 && buttons[y + 1][x - 2].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y + 1, x - 2));
		}
		else if (x - 2 >= 0 && y + 1 < 8 && buttons[y + 1][x - 2].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y + 1, x - 2));
		}
		//x-=2 y-=1
		if (x - 2 >= 0 && y - 1 >= 0 && buttons[y - 1][x - 2].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y - 1, x - 2));
		}
		else if (x - 2 >= 0 && y - 1 >= 0 && buttons[y - 1][x - 2].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y - 1, x - 2));
		}
		//x+=1 y+=2
		if (x + 1 < 8 && y + 2 < 8 && buttons[y + 2][x + 1].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y + 2, x + 1));
		}
		else if (x + 1 < 8 && y + 2 < 8 && buttons[y + 2][x + 1].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y + 2, x + 1));
		}
		//x+=1 y-=2
		if (x + 1 < 8 && y - 2 >= 0 && buttons[y - 2][x + 1].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y - 2, x + 1));
		}
		else if (x + 1 < 8 && y - 2 >= 0 && buttons[y - 2][x + 1].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y - 2, x + 1));
		}
		//x-=1 y+=2
		if (x - 1 >= 0 && y + 2 < 8 && buttons[y + 2][x - 1].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y + 2, x - 1));
		}
		else if (x - 1 >= 0 && y + 2 < 8 && buttons[y + 2][x - 1].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y + 2, x - 1));
		}
		//x-=1 y-=2
		if (x - 1 >= 0 && y - 2 >= 0 && buttons[y - 2][x - 1].figurine == GrpcGameService::None) {
			availableMoves.push_back(std::tuple<int, int>(y - 2, x - 1));
		}
		else if (x - 1 >= 0 && y - 2 >= 0 && buttons[y - 2][x - 1].color != color) {
			std::cout << "selected color: " << GrpcGameService::Color_Name(color) << std::endl << std::flush;
			std::cout << "destination color: " << GrpcGameService::Color_Name(buttons[y + 1][x + 2].color) << std::endl << std::flush;
			availableMoves.push_back(std::tuple<int, int>(y - 2, x - 1));
		}
		break;
#pragma endregion

#pragma region Bishop
	case GrpcGameService::Bishop:////////////////////////////////////////////////////////////////
		for (int x2 = x + 1, y2 = y + 1; x2 < 8 && y2 < 8; x2++, y2++) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = 9;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = 9;
			}
		}
		for (int x2 = x + 1, y2 = y - 1; x2 < 8 && y2 >= 0; x2++, y2--) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = -1;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = -1;
			}
		}
		for (int x2 = x - 1, y2 = y + 1; x2 >= 0 && y2 < 8; x2--, y2++) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = 9;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = 9;
			}
		}
		for (int x2 = x - 1, y2 = y - 1; x2 >= 0 && y2 >= 0; x2--, y2--) {
			if (buttons[y2][x2].figurine == GrpcGameService::None) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
			}
			else if (buttons[y2][x2].color != color) {
				availableMoves.push_back(std::tuple<int, int>(y2, x2));
				y2 = -1;
			}
			else if (buttons[y2][x2].color == color) {
				y2 = -1;
			}
		}
		break;
#pragma endregion

#pragma region Pawn
	case GrpcGameService::Pawn://////////////////////////////////////////////////////////////////////
		//tahy
		if (!moved) {
			switch (color) {
			case GrpcGameService::WHITE:
				if (buttons[y + 2][x].figurine == GrpcGameService::None)
					availableMoves.push_back(std::tuple<int, int>(y + 2, x));
				break;
			case GrpcGameService::BLACK:
				if (buttons[y - 2][x].figurine == GrpcGameService::None)
					availableMoves.push_back(std::tuple<int, int>(y - 2, x));
				break;
			}
		}
		switch (color) {
		case GrpcGameService::WHITE:
			if (buttons[y + 1][x].figurine == GrpcGameService::None)
				availableMoves.push_back(std::tuple<int, int>(y + 1, x));
			if (buttons[y + 1][x + 1].color == GrpcGameService::BLACK)
				availableMoves.push_back(std::tuple<int, int>(y + 1, x + 1));
			if (buttons[y + 1][x + 1].color == GrpcGameService::BLACK)
				availableMoves.push_back(std::tuple<int, int>(y + 1, x + 1));
			break;
		case GrpcGameService::BLACK:
			if (buttons[y - 1][x].figurine == GrpcGameService::None)
				availableMoves.push_back(std::tuple<int, int>(y - 1, x));
			if (buttons[y - 1][x - 1].color == GrpcGameService::WHITE)
				availableMoves.push_back(std::tuple<int, int>(y - 1, x - 1));
			if (buttons[y - 1][x - 1].color == GrpcGameService::WHITE)
				availableMoves.push_back(std::tuple<int, int>(y - 1, x - 1));
			break;
		}
		break;
	}
#pragma endregion
}
