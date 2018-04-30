gold_bach(A, B) :-
	is_prime(A), is_prime(B),
	write('Output: '), write(A), write(' '), write(B), nl.
gold_bach(A, B) :-
	B > 2,
	C is A+1, D is B-1,
	gold_bach(C, D).
gold_bach(A) :- T is A-2, gold_bach(2, T).

is_prime(N, 1) :- N >= 2.
is_prime(N, T) :- not(0 is N mod T), M is T-1, is_prime(N, M).
is_prime(N) :- S is floor(sqrt(N)), is_prime(N, S).

:- write('Input: '), read(Input), gold_bach(Input), halt.
