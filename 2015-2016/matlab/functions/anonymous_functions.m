power = @(x, n) x.^ n;

first_res = power(5, 2);
second_res = power(3, 3);

sin_cos_sum = @(a) sin(a) + cos(a);
sp1 = sin_cos_sum(0);

power_x = @(x) x .^ x;
power_x(5)
