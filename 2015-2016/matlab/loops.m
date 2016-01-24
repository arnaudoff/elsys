% Steps
for a = 1:2:10
end

v = [1, 2, 3, 4, 5];

for a = v
    % disp(a)
end

% Nested loop (prime checker)
for i = 2:20
    for j = 2:20
        if(~mod(i, j))
            break;
        end
    end
    
    if(j > (i / j))
        fprintf('%d is prime\n', i);
    end
end
