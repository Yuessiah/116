#include<stdio.h>
#include<string.h>

const int maxn = 100;

int n, t;
int E[maxn][maxn], topo[maxn];
int status[maxn];
int earliest[maxn], latest[maxn];

bool toposort(int u)
{
	status[u] = -1;
	for(int v = 0; v < n; v++) if(status[v] <= 0 && E[u][v] > 0) {
		if(earliest[v] < earliest[u] + E[u][v]) earliest[v] = earliest[u] + E[u][v];
		if(status[v] == -1) return false;
		if(!toposort(v)) return false;
	}

	status[u] = 1; topo[--t] = u;
	return true;
}

void topobackward(int u)
{
	for(int v = 0; v < n; v++) if(E[v][u] > 0) {
		if(latest[v] > latest[u] - E[v][u]) latest[v] = latest[u] - E[v][u];
	}
}

int e(int k, int l) { return earliest[k]; }
int l(int k, int l) { return latest[l] - E[k][l]; }

void solve()
{
	t = n;
	for(int u = 0; u < n; u++) if(!status[u] && !toposort(u)) {
		fprintf(stderr, "The graph have a circle! wrong input.\n");
		return;
	}

	for(int i = 0; i < n; i++) latest[i] = earliest[n-1];
	for(int i = n-1; i >= 0; i--) topobackward(topo[i]);

	//print result:
	printf("( x, y ) | early time | late time | slack\n");
	for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) {
		if(E[i][j] > 0) {
			printf("(%2d, %-2d) | %10d | %9d | %5d\n",\
					i, j, e(i, j), l(i, j), l(i, j) - e(i, j));
		}
	}
}

int main()
{
	memset(E, 0, sizeof(E));
	memset(earliest, 0, sizeof(earliest));
	memset(status, 0, sizeof(status));

	scanf("%d", &n);
	for(int i = 0; i < n; i++) for(int j = 0; j < n; j++)
		scanf("%d", &E[i][j]);

	solve();

	return 0;
}
