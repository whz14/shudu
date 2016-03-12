#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <assert.h>

using namespace std;

int cells[10][10];

void read(char* filename) {
	freopen(filename, "r", stdin);
	char s[9];
	for(int i = 0; i < 9; i++) {
		cin >> s;
		for(int j = 0; j < 9; ++j) {
			cells[i][j] = s[j] - '0';
		}
	}
	fclose(stdin);
}

void write(const char* filename) {
	freopen(filename, "w", stdout);
	for(int i = 0; i < 9; ++i) {
		for(int j = 0; j < 9; ++j) {
			printf("%d", cells[i][j]);
		}
		printf("\n");
	}
	fclose(stdout);
}

bool ok2FillAt(int i0, int j0, int n) {
	if(cells[i0][j0]) {
		return false;
	}
	for(int k = 0; k < 9; ++k) {
		if(cells[k][j0] == n || cells[i0][k] == n) {
			return false;
		}
	}
	int x = i0/3, y = j0/3;
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < 3; ++j) {
			if(cells[3*x + i][3*y + j] == n) {
				return false;
			}
		}
	}
	return true;
}

int candidateNum(int i0, int j0) {
	if(cells[i0][j0]) {
		return 0;
	}
	int ans = 0;
	for(int n = 1; n < 10; ++n) {
		ans += ok2FillAt(i0, j0, n);
	}
	return ans;
}

int getEasiest() {
	int x = 0, y = 0;
	int tmpMin = 10;
	for(int i = 0; i < 9; ++i) {
		for(int j = 0; j < 9; ++j) {
			int t = candidateNum(i, j);
			if(t && t < tmpMin) {
				tmpMin = t;
				x = i, y = j;
			}
		}
	}
	return 100*tmpMin + 10*x + y;
}

bool backTrace(int depth) {
	int easiest = getEasiest();
	int searchNum = easiest / 100;
	if(searchNum == 10) {
		for(int i = 0; i < 9; ++i) {
			for(int j = 0; j < 9; ++j) {
				if(!cells[i][j]) {	// unfilled but cannot be filled. search FAILED
					return false;
				}
			}
		}
		// printf("finisted!\n");
		return true;				// search finished and answer got.
	}
	int x = (easiest % 100) / 10;
	int y = easiest % 10;
	for(int n = 1; n < 10; n++) {
		if(ok2FillAt(x, y, n)) {
			// printf("@(%d, %d) with %d. depth %d \n", x, y, n, depth);
			cells[x][y] = n;
			if(backTrace(depth+1)){	// got answer!
				return 1;
			}
			else {
				cells[x][y] = 0;
			}
		}
	}
	return 0;					// search finished but failed
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("<app_name> <input_filename> <output_filename>\n");
		return 1;
	}
	read(argv[1]);
	backTrace(0);
	write(argv[2]);
	return 0;
}