% Simple function to show the use of global vars

function avg = average(nums)
    global TOTAL
    avg = sum(nums) / TOTAL;
end