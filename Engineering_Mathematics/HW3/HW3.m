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
syms v t;
f = symfun(2*v - 3*t + 1, [v, t]);
e = t + h*f; % euler's method

y = zeros(1, 11);
x = [1:0.1:2];
y(1) = 5;
for n = 1:1:10
    y(n+1) = subs(e, [v, t], [x(n), y(n)]); % calculate y_{n+1}
end

disp([x', y']);
