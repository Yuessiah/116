x = -5:0.1:5;
i = polyint([3,-4,-1], 2);
v = polyval(i, x);

hold on;
plot(x, v);
plot(roots(i), 0, 'ro');
hold off;

%%

x = 12:0.1:15;
v = sym('v');
f = exp(2*v) + 3*(v.^2);
F = int(f) + 3 - subs(int(f), v, 1);
G = int(F) + 1 - subs(int(F), v, 3);
hold on;
plot(x, subs(f, v, x), 'r');
plot(x, subs(diff(f), v, x), 'g');
plot(x, subs(F, v, x), 'b');
plot(x, subs(G, v, x), 'k');
hold off;

%%

h = 0.1;
v = sym('x');
t = sym('y');
f = 2*v - 3*t + 1;
g = t + h*f; % euler's method

ans = zeros(10, 2);
yn = 5;
x = [1:0.1:2];
for n = 1:1:10
    ans(n,:) = [double(x(n)), double(yn)];
    yn = subs(g, [v, t], [x(n), yn]); % calculate y_{n+1}
end

disp(ans);