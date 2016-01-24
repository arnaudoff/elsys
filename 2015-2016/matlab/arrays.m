zero_arr = zeros(5);
ones_arr = ones(2, 3);
identity_matrix = eye(4);

rand_arr = rand(3, 2);
magic_square = magic(3); % such magic wow

% Multidimensional arrays
a = [1 2 3; 4 5 6; 7 8 9];
a(:, :, 2) = [10, 11, 12; 13 14 15; 16 17 18];
a(:, :, 3) = magic(3);

b = rand(3, 3, 2); % Multidimensional rand()
c = cat(3, a, b, [4 3 1; 4 4 1; 4 1 5]);

is_c_matrix = ismatrix(c);
c_size = size(c);
c_len = length(c);
c_ndims = ndims(c);

s_arr = ['Gosho', 'Pesho', 'Mitko'];
s_numels = numel(s_arr);
s_sorted = sort(s_arr);