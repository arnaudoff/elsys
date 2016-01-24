foo = 'Foo';
bar = 'Bar';
foobar = foo + bar; % No concatenation; sums the ASCII codes
str = 'Ivo''s house';
str_ascii_8 = uint8(str);
str_ascii_16 = uint16(str);
unsigned = uint32(432.32);

min_int64 = intmin('int64');
max_int64 = intmax('int64');
n = 100;
is_n_scalar = isscalar(n);
m = [1 2 3; 4 5 6; 7 8 9];
scalar_mult = n * m;

row_vector = [1 2 3 4 5];
col_vector = [1; 2; 3; 4];
second_row_vector = [1 2 3 4 5];
vector_sum = row_vector + second_row_vector;