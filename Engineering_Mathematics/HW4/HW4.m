h = 0.1;
syms v t
dy = symfun(v.^2 + t.^2, [v, t]);
e = symfun(t + h*dy, t); % euler's method

y = zeros(1, 6);
x = 0:h:0.5;
y(1) = 1;

for n = 1:5
    y(n+1) = subs(e, [v, t], [x(n), y(n)]); % calculate y_{n+1}
end

disp([x', y']);
%%

h = 0.1;
syms t f;
e = symfun(t + h*f, [t f]); % euler's method

dy = zeros(1, 6);
y = zeros(1, 6);
dy(1) = 1;
y(1) = -2;

for n = 1:5
    y(n+1) = subs(e, [t, f], [y(n), dy(n)]); % calculate y_{n+1}
    dy(n+1) = subs(e, [t, f], [dy(n), 4*dy(n)-4*y(n)]);
end

disp([x', y']);
%%

h = 0.1;
syms v t;
f = symfun(2*v - 3*t + 1, [v, t]);

y = zeros(1,6);
x = 0:h:0.5;
dy = zeros(1,6);
y(1) = 1;

for i = 1:4
    dy(i) = subs(f, [v, t], [x(i), y(i)]);
    K1 = subs(f, [v, t], [x(i), y(i)]);
    K2 = subs(f, [v, t], [x(i) + h/2, y(i) + h*K1/2]);
    K3 = subs(f, [v, t], [x(i) + h/2, y(i) + h*K2/2]);
    K4 = subs(f, [v, t], [x(i) + h, y(i) + h*K3]);
    y(i+1) = y(i) + h*(K1 + 2*K2 + 2*K3 + K4)/6;
end

dy(5) = subs(f, [v, t], [x(5), y(5)]);
y6_s = y(5) + h*(55*dy(5) - 59*dy(4) + 37*dy(3) - 9*dy(2))/24; 
dy(6) = subs(f, [v, t],[x(6), y6_s]);
y(6) = y(5) + h*(9*dy(6) + 19*dy(5) - 5*dy(4) + dy(3))/24;

disp([x' y'])