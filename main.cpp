#include <Windows.h>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

Fl_PNG_Image* covered_png = new Fl_PNG_Image("Resources/Covered.png");
Fl_PNG_Image* blank_png = new Fl_PNG_Image("Resources/Blank.png");
Fl_PNG_Image* one_png = new Fl_PNG_Image("Resources/One.png");
Fl_PNG_Image* two_png = new Fl_PNG_Image("Resources/Two.png");
Fl_PNG_Image* three_png = new Fl_PNG_Image("Resources/Three.png");
Fl_PNG_Image* four_png = new Fl_PNG_Image("Resources/Four.png");
Fl_PNG_Image* five_png = new Fl_PNG_Image("Resources/Five.png");
Fl_PNG_Image* six_png = new Fl_PNG_Image("Resources/Six.png");
Fl_PNG_Image* seven_png = new Fl_PNG_Image("Resources/Seven.png");
Fl_PNG_Image* eight_png = new Fl_PNG_Image("Resources/Eight.png");
Fl_PNG_Image* mine_png = new Fl_PNG_Image("Resources/Mine.png");
Fl_PNG_Image* mine_wrong_png = new Fl_PNG_Image("Resources/MineWrong.png");
Fl_PNG_Image* mine_lost_png = new Fl_PNG_Image("Resources/MineLost.png");
Fl_PNG_Image* flag_png = new Fl_PNG_Image("Resources/Flag.png");
Fl_PNG_Image* question_png = new Fl_PNG_Image("Resources/Question.png");

enum STATE {_covered, 
	_blank, 
	_flagged, 
	_question,
	_mine,
};

STATE board[9][9];
Fl_Button* cell[9][9];

vector<Fl_Button*> mine_cell;
queue<Fl_Button*> free_cell;

struct coordinate
{
	int row;
	int col;
};
vector<coordinate> c;

int board_traversal(const int& row, const int& col) 
{
	/* Example (5,5)
	*
	*
	*     [4,4][4,5][4,6]
	*     [5,4](5,5)[5,6]
	*     [6,4][6,5][6,6]
	*
	*
	*/

	int row_start;
	int row_end;
	int col_start;
	int col_end;
	int mine_counter = 0;

	if (row > 0 && col > 0 && row < 8 && col < 8)
	{
		row_start = row - 1;
		row_end = row + 1;
		col_start = col - 1;
		col_end = col + 1;
	}
	else if (row > 0 && col > 0)
	{
		row_start = row - 1;
		row_end = row;
		col_start = col - 1;
		col_end = col;
	}
	else
	{
		row_start = row;
		row_end = row + 1;
		col_start = col;
		col_end = col + 1;
	}

	cout << "(" << row << "," << col << ")" << endl;
	cout << "row start : " << row_start << endl;
	cout << "row end : " << row_end << endl;
	cout << "col start : " << col_start << endl;
	cout << "col end : " << col_end << endl;

	for (int _row = row_start; _row <= row_end; _row++)
	{
		for (int _col = col_start; _col <= col_end; _col++)
		{
			//				cout << "_row " << _row << endl;
			//				cout << "_col " << _col << endl;
			if (board[_row][_col] == _mine && row != _row && col != _col)
			{
				mine_counter++;
			}
		}
	}
	cout << mine_counter << endl;
	if (mine_counter == 0)
	{
		for (int _row = row_start; _row <= row_end; _row++)
		{
			for (int _col = col_start; _col <= col_end; _col++)
			{
//				free_cell.push(cell[_row][_col]);
				board_traversal(_row, _col);
			}
		}
	}
	return mine_counter;
}
void click_callback(Fl_Widget* w, void* client)
{
	int rowcol = (int)client;
	int row = rowcol >> 8;
	int col = rowcol & 255;
	if (Fl::event_button() == FL_RIGHT_MOUSE)
	{
		cout << "Right-Click (" << row << "," << col << ")" << endl;
		if (board[row][col] == _flagged)
		{
			board[row][col] = _question;
			w->image(question_png);
			return;
		}
		else if (board[row][col] == _question)
		{
			board[row][col] = _covered;
			w->image(covered_png);
			return;
		}
		board[row][col] = _flagged;
		w->image(flag_png);
	}
	if (Fl::event_button() == FL_LEFT_MOUSE)
	{
		cout << "Left-Click (" << row << "," << col << ")" << endl;
		int mine_counter = board_traversal(row, col);
		cout << "Mines in adjacent tile of (" << row << ","
			<< col << ") : " << mine_counter << endl << endl;
		switch(mine_counter)
		{
		case 0:
			cout << "There are " << free_cell.size() << " buttons in the free cell vector." << endl;
			if (board[row][col] == _mine)
			{
				for (unsigned int i = 0; i < mine_cell.size(); i++)
				{
					mine_cell.at(i)->image(mine_lost_png);
				}
				Fl::redraw();
				return;
			}
			if (!free_cell.empty())
			{
				while(!free_cell.empty()) {
					free_cell.front()->image(blank_png);
					free_cell.pop();
				}
				Fl::redraw();
				break;
			}
			board[row][col] = _blank;
			w->image(blank_png);
			break;
		case 1:
			w->image(one_png);
			break;
		case 2:
			w->image(two_png);
			break;
		case 3:
			w->image(three_png);
			break;
		case 4:
			w->image(four_png);
			break;
		case 5:
			w->image(five_png);
			break;
		case 6:
			w->image(six_png);
			break;
		case 7:
			w->image(seven_png);
			break;
		case 8:
			w->image(eight_png);
			break;
		}
	}
}

class Mywin : public Fl_Window {
	void resize(int X, int Y, int W, int H) {
		Fl_Window::resize(X, Y, W, H);
		redraw();
		Fl::check();
	}
public:
	Mywin(int x, int y, int w, int h) : Fl_Window(x, y, w, h) {
	}
};

coordinate generator(int row, int col)
{
	coordinate random;
	int rrow;
	int rcol;
	cout << "Contents of C vector : ";
	for (unsigned int i = 0; i < c.size(); i++)
	{
		cout << "(" << c.at(i).row << "," << c.at(i).col << ") ";
		if(c.at(i).row == row && c.at(i).col == col)
		{
			rrow = rand() % 9;
			rcol = rand() % 9;
			generator(rrow, rcol);
			exit(0);
		}
	}
	cout << endl;
	random.row = row;
	random.col = col;
	c.push_back(random);
	return random;
}

void populateBoard()
{
	int rows = 9;
	int columns = 9;
	int width = 15;
	int height = 15;
	coordinate rowcol[10];

	srand((unsigned int)(NULL));

	for (int i = 0; i < 10; i++)
	{
		int rrow = rand() % 9;
		int rcol = rand() % 9;
		rowcol[i] = generator(rrow, rcol);
		cout << "\nMine placed at (" << rowcol[i].row << "," << rowcol[i].col << ")" << endl;
		board[rowcol[i].row][rowcol[i].col] = _mine;
	}

	for (int rr = 0; rr < rows; rr++)
	{
		for (int cc = 0; cc < columns; cc++)
		{
			Fl_Button* b = new Fl_Button(cc*width, rr*height, 15, 15);
			if (board[rr][cc] == _mine)
			{
				mine_cell.push_back(b);
			}
			cell[rr][cc] = b;
			b->image(covered_png);
			int rowcol = rr << 8 | cc;
			b->callback(click_callback, (void*)rowcol);
		}
	}
	cout << "The game board should have 10 mines... ";
	cout << "\t>> " << mine_cell.size() << endl << endl;
}

int main() {
	Mywin* win = new Mywin(20, 20, 800, 800);

	populateBoard();

	win->end();
	win->show();
	return (Fl::run());
}
