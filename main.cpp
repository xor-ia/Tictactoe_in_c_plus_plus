#include <iostream>
#include <Windows.h>
#include <vector>
#include <math.h>
using namespace std;

class tictactoe {
	private:
		int turn = 0; // 0 = x, 1 = o
		vector<int> lastmov;
		vector<int> board = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	public:
		void move(int id);
		void display();
		vector<int> getvalid();
		int getstate(); // -1 = x win, 0 = tie, 1 = o win, -2 = ongoing
		void undo();
		int minimax(int alpha, int beta);
		int generatebestmov();
};

void tictactoe::move(int id) {
	(*this).board[id] = (*this).turn == 0 ? 1 : 2;
	(*this).lastmov.push_back(id);
	(*this).turn = !((*this).turn);
}
void tictactoe::undo() {
	(*this).board[(*this).lastmov.back()] = 0;
	(*this).turn = !((*this).turn);
	(*this).lastmov.pop_back();
}
void tictactoe::display() {
	int i = 0;
	char charmap[3] = {' ', 'x', 'o'};
	printf("  a b c");
	for (int v : (*this).board) {
		if (i%3==0) {
			printf("\n%d|", (i/3)+1);
		}
		printf("%c|", charmap[v]);
		i++;
	}
	printf("\n");
}
vector<int> tictactoe::getvalid() {
	vector<int> validmov;
	for ( int i = 0; i < 9; i++ ) {
		if ( (*this).board[i] == 0 ) {
			validmov.push_back(1);
		} else {
			validmov.push_back(0);
		}
	}
	return validmov;
}
int tictactoe::getstate() {

	// check for horizontal win
	for ( int i = 0; i < 3; i++ ) {
		int val = i*3;
		if ( (*this).board[val] == (*this).board[val+1] && 
			 (*this).board[val] == (*this).board[val+2] && 
			 (*this).board[val] != 0 ) {
			return (((*this).board[val]-1) * 2) - 1;
		}
	}
	// check for vertical win
	for ( int i = 0; i < 3; i++ ) {
		if ( (*this).board[i] == (*this).board[i+3] && 
			 (*this).board[i] == (*this).board[i+6] && 
			 (*this).board[i] != 0 ) {
			return (((*this).board[i]-1) * 2) - 1;
		}
	}
	// check for diagonal win
	// ¯\_(ツ)_/¯
	if ( (*this).board[0] == (*this).board[4] &&
		 (*this).board[0] == (*this).board[8] &&
		 (*this).board[0] != 0 ) {
		return (((*this).board[0]-1) * 2) - 1;
	}
	if ( (*this).board[2] == (*this).board[4] &&
		 (*this).board[2] == (*this).board[6] &&
		 (*this).board[2] != 0 ) {
		return (((*this).board[2]-1) * 2) - 1;
	}
	// check for tie
	int isvalid = 0;
	for ( int i = 0; i < 9; i++ ) {
		isvalid += ((*this).board[i]) != 0;
	}
	if ( isvalid == 9 ) {
		return 0;
	}
	return -2; // The game isn't over yet
}
int tictactoe::minimax(int alpha, int beta) {
	int gamestate = (*this).getstate();
	if ( gamestate != -2 ) {
		return gamestate*((*this).turn*2-1);
	}
	vector<int> validmov = (*this).getvalid();
	for ( int i = 0; i < 9; i++ ) {
		if ( validmov[i] == 0 ) {
			continue;
		}
		(*this).move(i);
		int score = -((*this).minimax(-beta, -alpha));
		(*this).undo();
		if (score >= beta) {
			return beta;
		}
		alpha = (score > alpha) ? score : alpha;
	}
	return alpha;

}
int tictactoe::generatebestmov() {
	vector<int> validmov = (*this).getvalid();
	int curbestmov = -1;
	int curbestscore = INT_MAX;
	for ( int i = 0; i < 9; i++ ) {
		if ( validmov[i] == 0 ) {
			continue;
		}
		(*this).move(i);
		int score = (*this).minimax(-INT_MAX, INT_MAX);
		(*this).undo();
		if ( score <= curbestscore ) {
			curbestmov = i;
			curbestscore = score;
		}
	}
	return curbestmov;
}

int coord2mov(string *coord) {
	int n1 = ((*coord)[0]-97);
	int n2 = ((*coord)[1]-49);
	if ( n1 < 0 || n1 > 2 || n2 < 0 || n2 > 2 ) {
		return -1;
	}
	return n1 + n2*3;
}

int main() {
	tictactoe game;
	string plmov;
	game.display();
	while ( game.getstate() == -2 ) { // main loop, check if game is still on going
		printf("Move : ");
		cin >> plmov;
		int mov = coord2mov(&plmov);
		vector<int> validmov = game.getvalid();
		if ( mov == -1 || validmov[mov] == 0 ) {
			printf("Invalid!\n");
			continue;
		}
		game.move(mov);
		game.display();
		if (game.getstate() != -2) { // if game ended after the last move then exit loop
			break;
		}
		game.move(game.generatebestmov());
		game.display();
		printf("-----------------\n");
	}
	printf("ENDSTATE : %d\n", game.getstate());
	printf("END");
	return 0;
}