m1 = [1 2 3; 4 5 6; 7 8 9];
m2 = [1 2 3; 4 5 6; 7 8 9];

% Works just like in linear algebra
sum = m1 + m2;
dif = m1 - m2;
product = m1 * m2;

scalar_mult = m1 * 42;

v = [3 5 8];
v_mult = v .* 3;

m_trans = m1';
v_trans = v';

v_power = v .^ 3;
m_power = m .^ 2;

v_sum = sum(v);
pi = 3.14;
c_pi = ceil(pi);
f_pi = floor(pi);

if (pi > 3)
    display('Real pi!');
else
    display('Fake pi');
end

if (5 && 6)
    display('Ok.');
end

ind = find(v, 1);

a = 60; % 60 = 0011 1100
b = 13; % 13 = 0000 1101

c = bitand(a, b); % 12 = 0000 1100
c = bitor(a, b); % 61 = 0011 1101
c = bitxor(a, b); % 49 = 0011 0001
c = bitshift(a, 2); % 240 = 1111 0000 */
c = bitshift(a,-2); % 15 = 0000 1111 */

v1 = [1 2 3];
v2 = [2 4 5];
inters = intersect(v1, v2);
u = union(v1, v2);