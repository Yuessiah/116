lca(A, B) :-
	ancestor(A, B) -> write(A), nl;
	parent(X, A), lca(X, B).

ancestor(A, A).
ancestor(A, B) :- parent(X, B), ancestor(A, X).

parent_input(N) :-
	N > 1,
	read(U), read(V),
	Fact =.. [parent, U, V], asserta(Fact),
	M is N-1, parent_input(M);
	true.

query(M) :-
	M > 0,
	read(U), read(V),
	lca(U, V),
	N is M-1, query(N);
	true.

:- read(N), parent_input(N), read(M), query(M), halt.
