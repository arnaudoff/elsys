A = [ 1 2 3 4 5; 2 3 4 5 6; 3 4 5 6 7; 4 5 6 7 8];

p = A(4, 5);

first_col = A(:, 1);
last_col = A(:, 5);

first_row = A(1, :);
last_row = A(4, :);

proj1 = A(:, 2:4);
sub_matrix = A(2:3, 2:3);

% Delete second row
A(2, :) = [];

B = [ 1 2 3 ; 4 5 6; 7 8 9];
sub_matrix_b = B([2,3], :);

D = [1 2 3; 4 5 6; 7 8 9];
E = [9 8 7; 6 5 4; 3 2 1];

matrix_sum = D + E;
matrix_diff = D - E;
matrix_product = D * E;
matrix_scalar_mult = D * 666;
matrix_transposed = D';

% Matrix concatenation
F = [D, E]; % Horizontal
G = [D; E]; % Vertical

% Determinant
d_det = det(D);

% Inverse
d_inv = inv(D);