#include<cstdio>
#include<cstring>
#include<queue>
using std::queue;
const int maxn = 100;
const int rdir[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
const int cdir[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
FILE *fptr = NULL;

struct pos {
	int row, col, cnt;
	pos(int r=0, int c=0):cnt(0),row(r),col(c){}
};
int n, m, vis[maxn][maxn], route[maxn][maxn];
char maze[maxn][maxn];
pos start, end;

void input()
{
	fptr = fopen("in.txt", "r");

	char line[110];
	while(fgets(line, 110, fptr) != NULL) {
		strcpy(maze[n], line);
		for(int i = 0; i < strlen(line); i++) if(line[i] == 's') { start.row = n; start.col = i; }
		n++;
	}
	m = strlen(line)-1;

	fclose(fptr);
}
void output()
{
	fptr = fopen("out.txt", "w");

	//backtracking
	int r = end.row, c = end.col, rnxt, cnxt;
	for(int i = 0; i < 8; i++) {
		rnxt = r+rdir[i]; cnxt = c+cdir[i];
		if(route[rnxt][cnxt] > route[r][c]) { r = rnxt; c = cnxt; }
	}
	while(1) {
		if(route[r][c] == 1) break;
		maze[r][c] = '*';
		for(int i = 0; i < 8; i++) {
			rnxt = r+rdir[i]; cnxt = c+cdir[i];
			if(route[rnxt][cnxt] == route[r][c]-1 && rnxt >= 0 && cnxt >= 0 && rnxt < n && cnxt < m) { r = rnxt; c = cnxt; break; }
		}
	}

	//answer
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) fprintf(fptr, "%c", maze[i][j]);
		fprintf(fptr, "\n");
	}
}

int bfs(int r, int c)
{
	queue<pos> q;
	pos p(r, c);
	q.push(p);

	int rnxt, cnxt;
	while(!q.empty()) {
		p = q.front(); q.pop();
		r = p.row, c = p.col;
		if(maze[r][c] == 'd') { end.row = r; end.col = c; return p.cnt; }
		p.cnt++;
		for(int i = 0; i < 8; i++) {
			rnxt = r+rdir[i]; cnxt = c+cdir[i];
			if(!vis[r][c] && maze[rnxt][cnxt] != '1' && rnxt >= 0 && cnxt >= 0 && rnxt < n && cnxt < m) {
				route[r][c] = p.cnt;
				p.row = rnxt; p.col = cnxt;
				q.push(p);
			}
		}
		vis[r][c] = 1;
	}
	return 0;
}

int main()
{
	input();
	int step;
	if(step = bfs(start.row, start.col)) output();
	fprintf(fptr, "%d steps", step);
	fclose(fptr);
	return 0;
}
