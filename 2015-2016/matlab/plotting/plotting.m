x = 0:10:50;
y = x;
subplot(3, 3, 1);
plot(x, y)

x = 0:0.1:10;
y = sin(x);
g = cos(x);

subplot(3, 3, 2);
plot(x, y, x, g), legend('sin(x)', 'cos(x)');
grid on

subplot(3, 3, 3);
x = -10:0.01:10;
y = 3*x.^4 + 2 * x.^3 + 7 * x.^2 + 2 * x + 9;
g = 5 * x.^3 + 9 * x + 2;
plot(x, y, 'r', x, g, 'b') % Colors!
grid on

subplot(3, 3, 4);
x = [13, 27, 22, 29];
y = [2, 2, 6, 6];
bar(x, y), title('PPS 2016 marks distribution'), xlabel('Student number'), ylabel('Score')

x = 2:0.5:5;
disp(x);
y = 0:1:10;
disp(y);
[x_grid, y_grid] = meshgrid(x, y);
disp(x_grid);
disp(y_grid);
z_grid = x_grid + y_grid;

subplot(3, 3, 5);
[x, y] = meshgrid(-10:10);
z = x.^2 + y.^2;
surf(x, y, z);

subplot(3, 3, 6);
[x1_grid, x2_grid] = meshgrid(-2:.2:2);
g = x1_grid .* exp(-x1_grid.^2 - x2_grid.^2);
surf(x1_grid, x2_grid, g);

subplot(3, 3, 7)
[x, y] = meshgrid(-8:.5:8);
r = sqrt(x.^2 + y.^2) + eps;
z = sin(r) ./ r;
surf(x, y, z);

subplot(3, 3, 8);
[x_grid, y_grid] = meshgrid(-5:5);
z = cos(y_grid) * sin(x_grid);
surf(x_grid, y_grid, z);
res = sin(x_grid) * cos(y_grid);