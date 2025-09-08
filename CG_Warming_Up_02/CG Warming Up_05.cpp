//화면에 30x30크기의 보드를 그린다(30x30의 각 칸에.을 찍어 보드를 표현한다).사용자에게 사각형의 두 개의 좌
//표값(x1, y1) (x2, y2) 값을 입력 받는다.
//• 총 2개의 도형을 그리고, 명령어에 따라 도형을 이동, 확대 / 축소, 한 좌표 측으로의 확대 / 축소하여 그리고 두 도형
//의 충돌도 체크한다.
//• 도형을 이동하다가 두 도형이 겹쳐지게 되면 그 부분을 다른 색으로 표시한다.
//• 입력 명령어 command format (input : x1 -> excute command x at first rectangle):
//• x / X : x축 우측 / 좌측으로 한 칸 이동(가장자리에 도달해도 계속 이동하고, 보드 밖으로 나가는 부분은 반대편에 나타난다.)
// ex command ) input : x -> move right 1, input : X -> move left 1
//• y / Y : y축 아래쪽 / 위쪽으로 한 칸 이동(가장자리에 도달해도 계속 이동하고, 보드 밖으로 나가는 부분은 반대편에 나타난다.)
// ex command ) input : y -> move down 1, input : Y -> move up 1
//• s / S : x와 y 모두 한 칸 축소 / 확대(범위를 넘어가는 축은 변경 안됨)
// ex command ) input : s -> shrink 1 at rectangle_1, input : S -> stretch 1 at rectangle_1
//• i / I : x축 한 칸 확대 / 축소
// ex command ) input : i -> stretch x 1 at rectangle_1, input : I -> shrink x 1 at rectangle_1
//• j / J : y축 한 칸 확대 / 축소
// ex command ) input : j -> stretch y 1 at rectangle_1, input : J -> shrink y 1 at rectangle_1
//• a / A : x축 한 칸 확대, y축 한 칸 축소 / x축 한 칸 축소, y축 한 칸 확대
// ex command ) input : a -> stretch x 1, shrink y 1  at rectangle_1, input : A -> shrink x 1, stretch y 1  at rectangle_1
//• b : 사각형의 면적 출력
// print the area of rectangle
//•  위 명령어를 두 도형에 모두 적용할 수 있도록 두 번째 도형에 대한 명령어를 스스로 설정한다.
// 
// 
//• c : 보드칸을 x축, y축 모두 한 칸씩 늘린다.최대 10칸만큼 늘릴 수 있다.
//• d : 보드칸을 x축, y축 모두 한 칸씩 줄인다.최대 20칸만큼 줄일 수 있다.도형은 보드칸 내에 있어야 한다.
//• r : 모든 것을 리셋 하

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#define NOMINMAX
#include <Windows.h>
using namespace std;

constexpr int DEFAULT_COLOR = 7;
// first rectangle color
constexpr int BRIGHT_GREEN_COLOR = 10;
// second rectangle color
constexpr int BRIGHT_BLUE_COLOR = 9;
// overlap color
constexpr int RED_COLOR = 4;
constexpr int BRIGHT_RED_COLOR = 12;
constexpr int Max_Board_Size{ 40 };
int Error_Flag{};

static void SetColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

class Board {
	int Board_Plate[Max_Board_Size][Max_Board_Size];
	int Current_Board_Size;
	vector<pair<pair<int, int>, pair<int, int>>> rectangles; // {{x1, y1}, {x2, y2}}

public:
	Board() : Current_Board_Size(30) {
		init_Board();
	}

	void init_Board() {
		for (int i = 0; i < Max_Board_Size; ++i) {
			for (int j = 0; j < Max_Board_Size; ++j) {
				Board_Plate[i][j] = 0;
			}
		}
		Current_Board_Size = 30;
		rectangles.clear();
		rectangles.resize(0);
	}

	void draw_Board() const {
		system("cls");
		for (int i = 0; i < Current_Board_Size; ++i) {
			for (int j = 0; j < Current_Board_Size; ++j) {
				if (Board_Plate[i][j] == 0) {
					SetColor(DEFAULT_COLOR);
					cout << ". ";
				}
				else if (Board_Plate[i][j] == 1) {
					SetColor(BRIGHT_GREEN_COLOR);
					cout << "■ ";
				}
				else if (Board_Plate[i][j] == 2) {
					SetColor(BRIGHT_BLUE_COLOR);
					cout << "■ ";
				}
				else {
					SetColor(RED_COLOR);
					cout << "■ ";
				}
			}
			cout << "\n";
		}
		SetColor(DEFAULT_COLOR);
	}

	void create_rectangle(int x1, int y1, int x2, int y2) {
		if (rectangles.size() == 2) {
			Error_Flag = 3;
			return;
		}
		if (x1 < 0 || x1 >= Current_Board_Size || x2 < 0 || x2 >= Current_Board_Size ||
			y1 < 0 || y1 >= Current_Board_Size || y2 < 0 || y2 >= Current_Board_Size) {
			Error_Flag = 4;
			return;
		}
		if (x1 > x2 || y1 > y2) {
			Error_Flag = 5;
			return;
		}
		rectangles.push_back({ {x1, y1}, {x2, y2} });
		update_Board();
	}

	void update_Board() {
		// Corrected the loop to iterate over the vector of rectangles
		for (int i = 0; i < Current_Board_Size; ++i) {
			for (int j = 0; j < Current_Board_Size; ++j) {
				Board_Plate[i][j] = 0;
			}
		}
		int weidth{ 1 };
		for (const auto& coordinate : rectangles) {
			// first rectangle area : 1
			// second rectangle area : 2
			// overlap area : 3
			for (int i = coordinate.first.second; i <= coordinate.second.second; ++i) {
				for (int j = coordinate.first.first; j <= coordinate.second.first; ++j) {
					Board_Plate[i][j] += weidth;
				}
			}
			++weidth;
		}
	}

	void extend_Board() {
		if (Current_Board_Size < Max_Board_Size) {
			++Current_Board_Size;
		}
		else {
			Error_Flag = 8;
		}
	}
	void reduce_Board() {
		if (Current_Board_Size > 10) {

			--Current_Board_Size;
		}
		else {
			Error_Flag = 6;
		}
	}
	int get_Current_Board_Size() const {
		return Current_Board_Size;
	}
	size_t get_Num_of_Rectangle() const {
		return rectangles.size();
	}
	int get_Rectangle_Area(int index) const {
		if (index < 0 || index >= rectangles.size()) {
			Error_Flag = 9;
			return 0;
		}
		int width = rectangles[index].second.first - rectangles[index].first.first + 1;
		int height = rectangles[index].second.second - rectangles[index].first.second + 1;
		return width * height;
	}

	// Movement and resizing functions to be implemented
	void move_Right(int index) {
		rectangles[index].first.first = (rectangles[index].first.first + 1) % Current_Board_Size;
		rectangles[index].second.first = (rectangles[index].second.first + 1) % Current_Board_Size;
		update_Board();
	}
	void move_Left(int index) {
		rectangles[index].first.first = (rectangles[index].first.first - 1 + Current_Board_Size) % Current_Board_Size;
		rectangles[index].second.first = (rectangles[index].second.first - 1 + Current_Board_Size) % Current_Board_Size;
		update_Board();
	}
	void move_Down(int index) {
		rectangles[index].first.second = (rectangles[index].first.second + 1) % Current_Board_Size;
		rectangles[index].second.second = (rectangles[index].second.second + 1) % Current_Board_Size;
		update_Board();
	}
	void move_Up(int index) {
		rectangles[index].first.second = (rectangles[index].first.second - 1 + Current_Board_Size) % Current_Board_Size;
		rectangles[index].second.second = (rectangles[index].second.second - 1 + Current_Board_Size) % Current_Board_Size;
		update_Board();
	}
	void shrink_Both(int index) {
		if (rectangles[index].second.first > rectangles[index].first.first)
			--rectangles[index].second.first;
		if (rectangles[index].second.second > rectangles[index].first.second)
			--rectangles[index].second.second;
		update_Board();
	}
	void stretch_Both(int index) {
		if (rectangles[index].second.first < Current_Board_Size - 1)
			++rectangles[index].second.first;
		if (rectangles[index].second.second < Current_Board_Size - 1)
			++rectangles[index].second.second;
		update_Board();
	}
	void stretch_X(int index) {
		if (rectangles[index].second.first < Current_Board_Size - 1)
			++rectangles[index].second.first;
		update_Board();
	}
	void shrink_X(int index) {
		if (rectangles[index].second.first > rectangles[index].first.first)
			--rectangles[index].second.first;
		update_Board();
	}
	void stretch_Y(int index) {
		if (rectangles[index].second.second < Current_Board_Size - 1)
			++rectangles[index].second.second;
		update_Board();
	}
	void shrink_Y(int index) {
		if (rectangles[index].second.second > rectangles[index].first.second)
			--rectangles[index].second.second;
		update_Board();
	}
	void stretch_X_shrink_Y(int index) {
		if (rectangles[index].second.first < Current_Board_Size - 1)
			++rectangles[index].second.first;
		if (rectangles[index].second.second > rectangles[index].first.second)
			--rectangles[index].second.second;
		update_Board();
	}
	void shrink_X_stretch_Y(int index) {
		if (rectangles[index].second.first > rectangles[index].first.first)
			--rectangles[index].second.first;
		if (rectangles[index].second.second < Current_Board_Size - 1)
			++rectangles[index].second.second;
		update_Board();
	}

	vector<pair<pair<int, int>, pair<int, int>>> get_Rectangles() const {
		return rectangles;
	}
	void Rect_locate() const {
		for (int i = 0; i < rectangles.size(); ++i) {
			cout << "Rectangle " << i + 1 << ": (" << rectangles[i].first.first << ", " << rectangles[i].first.second << ") to ("
				<< rectangles[i].second.first << ", " << rectangles[i].second.second << ")\n";
		}
	}
};

static void Error_Message();
static int Excute_command(string command, Board& board);

int main()
{
	Board board;
	string command;

	while (true) {
		//system("cls");
		board.draw_Board();
		Error_Message();
		if (command == "b") {
			// print area of rectangles
			for (int i = 0; i < board.get_Num_of_Rectangle(); ++i) {
				cout << "Area of rectangle " << i + 1 << ": " << board.get_Rectangle_Area(i) << "\n";
			}
		}

		board.Rect_locate();

		cout << "Command List: r : reset board, c : increase board size, d : decrease board size, q : quit, e : create rectangle\n";
		cout << "\n\nCommand Input : ";
		cin >> command;
		if (Excute_command(command, board) == -1) {
			break;
		}
	}
}

static int Excute_command(string command, Board& board) {
	if (command == "c") {
		// increase board size
		board.extend_Board();
	}
	else if (command == "d") {
		// decrease board size
		board.reduce_Board();
	}
	else if (command == "r") {
		// reset board
		board.init_Board();
	}
	else if (command == "e") {
		// create rectangle
		if (board.get_Num_of_Rectangle() == 2) {
			Error_Flag = 3;
			return 0;
		}

		int x1, y1, x2, y2;
		cout << "Enter coordinates (x1 y1 x2 y2): ";
		if (!(cin >> x1 >> y1 >> x2 >> y2)) {
			Error_Flag = 2;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return 0;
		}
		board.create_rectangle(x1, y1, x2, y2);
	}
	else if (command == "q") {
		// exit program
		exit(0);
	}
	else if (command.length() == 2) {
		int rect_index = command[1] - '1';
		char action = command[0];
		cout << "for Debug : " << action << ", " << rect_index << "\n";
		if (rect_index != 0 && rect_index != 1) {
			Error_Flag = 9;
			return 0;
		}
		if (rect_index > 1) {
			Error_Flag = 9;
			return 0;
		}
		if (rect_index > board.get_Num_of_Rectangle()) {
			Error_Flag = 10;
			return 0;
		}
		switch (action) {
		case 'x':
			// move right
			board.move_Right(rect_index);
			break;
		case 'X':
			// move left
			board.move_Left(rect_index);
			break;
		case 'y':
			// move down
			board.move_Down(rect_index);
			break;
		case 'Y':
			// move up
			board.move_Up(rect_index);
			break;
		case 's':
			// shrink x and y
			board.shrink_Both(rect_index);
			break;
		case 'S':
			// stretch x and y
			board.stretch_Both(rect_index);
			break;
		case 'i':
			// stretch x
			board.stretch_X(rect_index);
			break;
		case 'I':
			// shrink x
			board.shrink_X(rect_index);
			break;
		case 'j':
			// stretch y
			board.stretch_Y(rect_index);
			break;
		case 'J':
			// shrink y
			board.shrink_Y(rect_index);
			break;
		case 'a':
			// stretch x, shrink y
			board.stretch_X_shrink_Y(rect_index);
			break;
		case 'A':
			// shrink x, stretch y
			board.shrink_X_stretch_Y(rect_index);
			break;
		default:
			cout << "Invalid action command.\n";
			break;
		}
	}
	else {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Error_Flag = 1;
	}
	return 0;
}

static void Error_Message() {
	// Flag 1 : Invalid action Command
	// Flag 2 : Wrong format coordinates Command
	// Flag 3 : Already two rectangles exist
	// Flag 4 : Out of Range
	// Flag 5 : Invalid Coordinates
	// Flag 6 : Can't reduce board size (less than 10)
	// Flag 7 : Can't reduce board size (rectangles are out of range)
	// Flag 8 : Can't extend board size (more than 40)
	// Flag 9 : Invalid shape number
	// Flag 10 : No existing shape
	if (Error_Flag == 0) return;
	cout << "\n\n";
	switch (Error_Flag) {
	case 1:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Invalid action Command\n";
		break;
	case 2:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Wrong format coordinates Command\n";
		break;
	case 3:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Already two rectangles exist\n";
		break;
	case 4:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Out of Range\n";
		break;
	case 5:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Invalid Coordinates\n";
		break;
	case 6:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Can't reduce board size (less than 10)\n";
		break;
	case 7:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Can't reduce board size (rectangles are out of range)\n";
		break;
	case 8:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Can't extend board size (more than 40)\n";
		break;
	case 9:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: Invalid shape number\n";
		break;
	case 10:
		SetColor(BRIGHT_RED_COLOR);
		cout << "Error: No existing shape\n";
		break;
	default:
		break;
	}
	SetColor(DEFAULT_COLOR);
	Error_Flag = 0;
}
