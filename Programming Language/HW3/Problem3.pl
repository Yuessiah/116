reachable(A, A).
reachable(A, B) :- edge(X, B),	check(X), reachable(A, X).

check(X) :- not(visited(X)), asserta(visited(X)).

edge_input(N) :-
	N > 0,
	read(U), read(V),
	asserta(edge(U, V)), asserta(edge(V, U)),
	M is N-1, edge_input(M);
	true.

yes_or_no(A, B) :- reachable(A, B) -> write('Yes'), nl; write('No'), nl.

query(M) :-
	M > 0,
	read(A), read(B),
	asserta(visited(B)),
	yes_or_no(A, B),
	retractall(visited(_)),
	T is M-1, query(T);
	true.

:-
	read(N), read(E), edge_input(E),
	read(M), query(M), halt.
