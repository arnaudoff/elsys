% Calculates the roots of a quadratic equation

function [x1, x2] = quadratic(a, b, c)
    d = disc_private(a, b, c);
    x1 = (-b + d) / (2 * a);
    x2 = (-b - d) / (2 * a);
    
    function dis = disc_nested(a, b, c)
        dis = sqrt(b^2 - 4 * a * c);
    end
end