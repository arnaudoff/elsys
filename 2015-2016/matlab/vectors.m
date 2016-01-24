% Row vector
v1 = [1 2 3 4 5 6];

% Column vector
v2 = [2; 3; 5; 7; 11];

elem = v2(3);
elements = v2(:);

subset = v2(3:5);

v3 = [1, 2, 3, 4, 5, 6];
v_sum = v1 + v3;
v_diff = v1 - v3;
scalar_div = v3 / 42;

% Transpose, row -> col and vice versa
v1_t = v1';
v2_t = v2';

v4_append = [v1, v3];
v4_matrix_append = [v1; v3];

v5 = [13; 17; 19; 23; 29];
v_25_matrix_append = [v2, v5];

% Magnitude
sv = v3 .* v3;
sv_sum = sum(sv);
sv_sumsqrt = sqrt(sv_sum);

dot_product = dot(v2, v5);

new_vector = [0:5:20];
s_power = new_vector .^ 2;

v6 = 1:10;
v7 = 100:-10:50;
v8 = 0:pi/4:pi;