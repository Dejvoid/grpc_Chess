#include "ChessWindow.h"
#include <gtkmm/stylecontext.h>
#include <gtkmm/cssprovider.h>

using namespace grpc;
using namespace google::protobuf;
using namespace GrpcGameService;
using namespace std;

ChessWindow::ChessWindow() :
	dispatcher()
{
	afterStart = false;
	css_red = Gtk::CssProvider::create();
	css_green = Gtk::CssProvider::create();
	css_white = Gtk::CssProvider::create();
	css_black = Gtk::CssProvider::create();
	css_red->load_from_data("button {background-image: image(red);}");
	css_green->load_from_data("button {background-image: image(green);}");
	css_white->load_from_data("button {background-image: image(white);}");
	css_black->load_from_data("button {background-image: image(black);}");
	this->set_default_size(100, 100);
	this->set_title("Title");
	dispatcher.connect(sigc::mem_fun(*this, &ChessWindow::moveRecieved));
	btnGrid.set_row_homogeneous(true);
	btnGrid.set_column_homogeneous(true);
	add(btnGrid);
	buttons = new Sector * [8];
	for (int i = 0; i < 8; i++) {
		buttons[i] = new Sector[8];
		for (int j = 0; j < 8; j++) {
			buttons[i][j].button = new Gtk::Button();
			buttons[i][j].figurine = GrpcGameService::None;
			buttons[i][j].color = GrpcGameService::NONE;
			if (((i + j) % 2) != 0) {
				buttons[i][j].button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
			}
			else {
				buttons[i][j].button->get_style_context()->add_provider(css_black, GTK_STYLE_PROVIDER_PRIORITY_USER);
			}
			buttons[i][j].button->signal_clicked().connect(sigc::bind<int, int>(sigc::mem_fun(*this, &ChessWindow::buttonClicked), j, i));
			btnGrid.attach(*buttons[i][j].button, j, i, 1, 1);
		}
	}
	clickedOn = false;
	this->show_all_children();

}
ChessWindow::~ChessWindow() {

}

void ChessWindow::joinRoom() {
	listenerThread = new std::thread(&ChessWindow::recieveMoves, this/*, &ctx, req*/);
}
void ChessWindow::recieveMoves() {
	ClientContext ctx;
	RoomJoinRequest req;
	req.set_allocated_room(room);
	GameCommandResponse gcr;
	auto resp = Client->Join(&ctx, req);
	cout << "successfully joined room: " + room->name() << endl << flush;
	while (resp.get()->Read(&gcr))
	{
		if (gcr.has_end()) {
			gcr.end();
			//process end
			this->dispatcher();
		}
		else if (gcr.has_move()) {
			lastMove = gcr.move();
			cout << "RUM1" << endl << flush;
			this->dispatcher();
		}
		else if (gcr.has_start()) {
			this->set_title(GrpcGameService::Color_Name(gcr.start().color()));
			this->color = gcr.start().color();
			if (color == GrpcGameService::WHITE)
				onTurn = true;
			else
				onTurn = false;
			gameId = gcr.start().gameid();
			cout << "RUM2" << endl << flush;
			for (int i = 0; i < gcr.start().state().tiles_size(); i++)
			{
				auto tile = gcr.start().state().tiles(i);
				int y = tile.position().row();
				int x = tile.position().column();
				buttons[y][x].color = tile.figurinecolor();
				buttons[y][x].figurine = tile.figurine();
				buttons[y][x].posY = y;
				buttons[y][x].posX = x;
			}
			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					if (!(buttons[y][x].color == GrpcGameService::BLACK || buttons[y][x].color == GrpcGameService::WHITE)) {
						buttons[y][x].color = GrpcGameService::NONE;
						buttons[y][x].posX = x;
						buttons[y][x].posY = y;
					}
				}
			}
			this->dispatcher();
		}
	}
}
void ChessWindow::moveRecieved() {
	cout << "recieved" << endl << flush;
	if (afterStart) {
		int fromX = lastMove.from().column();
		int fromY = lastMove.from().row();
		int toX = lastMove.to().column();
		int toY = lastMove.to().row();

		buttons[toY][toX].color = buttons[fromY][fromX].color;
		buttons[toY][toX].figurine = buttons[fromY][fromX].figurine;
		buttons[fromY][fromX].color = GrpcGameService::NONE;
		buttons[fromY][fromX].figurine = GrpcGameService::None;

		onTurn = true;
	}
	afterStart = true;
	showIcons();
}
void ChessWindow::buttonClicked(int x, int y) {
	cout << y << x << endl << flush;
	cout << GrpcGameService::Color_Name(buttons[y][x].color) << endl << flush;
	cout << GrpcGameService::Figurine_Name(buttons[y][x].figurine) << endl << flush;
	Sector* clicked = &buttons[y][x];
	if (clickedOn) {

		bool isAvailable = false;
		for (int i = 0; i < lastClicked->availableMoves.size(); i++) {
			int y2 = std::get<0>(lastClicked->availableMoves.at(i));
			int x2 = std::get<1>(lastClicked->availableMoves.at(i));
			if (x == x2 && y == y2)
				isAvailable = true;
		}
		if (isAvailable) {
			GrpcGameService::GameMoveResponse gmresp;
			grpc::ClientContext ctx;
			GrpcGameService::GameMoveRequest gmreq;
			GrpcGameService::GameMove* mov = new GrpcGameService::GameMove();
			GrpcGameService::Position* from = new GrpcGameService::Position();
			GrpcGameService::Position* to = new GrpcGameService::Position();
			from->set_column(lastClicked->posX);
			from->set_row(lastClicked->posY);
			to->set_column(x);
			to->set_row(y);
			mov->set_allocated_from(from);
			mov->set_allocated_to(to);
			gmreq.set_gameid(gameId);
			gmreq.set_allocated_move(mov);
			Client->Move(&ctx, gmreq, &gmresp);

			std::cout << "can Move to" << y << x << endl << flush;

			buttons[to->row()][to->column()].color = buttons[from->row()][from->column()].color;
			buttons[to->row()][to->column()].figurine = buttons[from->row()][from->column()].figurine;
			buttons[to->row()][to->column()].moved = true;
			drawIcon(to->row(), to->column());;

			
			buttons[from->row()][from->column()].color = GrpcGameService::NONE;
			buttons[from->row()][from->column()].figurine = GrpcGameService::None;
			drawIcon(from->row(), from->column());

			onTurn = false;
		}
		for (int i = 0; i < lastClicked->availableMoves.size(); i++)
		{
			int x3 = std::get<1>(lastClicked->availableMoves.at(i));
			int y3 = std::get<0>(lastClicked->availableMoves.at(i));
			if ((x3 + y3) % 2 == 0)
				buttons[y3][x3].button->get_style_context()->add_provider(css_black, GTK_STYLE_PROVIDER_PRIORITY_USER);
			else
				buttons[y3][x3].button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
		}
		if ((lastClicked->posX + lastClicked->posY) % 2 == 0)
			lastClicked->button->get_style_context()->add_provider(css_black, GTK_STYLE_PROVIDER_PRIORITY_USER);
		else
			lastClicked->button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
		lastClicked = nullptr;
		clickedOn = false;
	}
	else if (!clickedOn && clicked->figurine != GrpcGameService::None && clicked->color == this->color && onTurn) {
		clicked->button->get_style_context()->add_provider(css_green, GTK_STYLE_PROVIDER_PRIORITY_USER);
		clicked->getMoves(buttons, x, y);
		for (int i = 0; i < clicked->availableMoves.size(); i++) {
			int y2 = std::get<0>(clicked->availableMoves.at(i));
			int x2 = std::get<1>(clicked->availableMoves.at(i));
			cout << y2 << x2 << endl << flush;
			buttons[y2][x2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
		}
		lastClicked = clicked;
		clickedOn = true;
	}
	//auto f = buttons;

}
void ChessWindow::showIcons() {
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			drawIcon(y,x);
		}
	}
}
void ChessWindow::drawIcon(int y, int x) {
	Gtk::Image* image;
	switch (buttons[y][x].color) {
	case GrpcGameService::WHITE:
		switch (buttons[y][x].figurine) {
		case GrpcGameService::King:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("white_king.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Queen:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("white_queen.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Rook:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("white_rook.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Knight:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("white_knight.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Bishop:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("white_bishop.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Pawn:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("white_pawn.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		}
		break;
	case GrpcGameService::NONE:
		buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
		image = new Gtk::Image("empty.png");
		buttons[y][x].button->set_image(*image);
		image->show();
		break;
	case GrpcGameService::BLACK:
		switch (buttons[y][x].figurine) {
		case GrpcGameService::King:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("black_king.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Queen:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("black_queen.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Rook:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("black_rook.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Knight:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("black_knight.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Bishop:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("black_bishop.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		case GrpcGameService::Pawn:
			buttons[y][x].button->set_image_position(Gtk::POS_LEFT);
			image = new Gtk::Image("black_pawn.png");
			buttons[y][x].button->set_image(*image);
			image->show();
			break;
		}
		break;

	}
}