-- Duane Shaffer
-- pa2.lua
-- Lua program for assignment 2

local pa2 = {}

     function pa2.mapArray(inputfunc, givenArray)
        for key,val in ipairs(givenArray) do
            givenArray[key] = inputfunc(val)
        end
        return givenArray
    end

    function pa2.concatMax(s,i)
        return s:rep(math.floor(i/s:len()))
    end

    function pa2.collatz(k)
        local returns = { k }
        while k ~= 1 do
            if k % 2 == 0 then
                k = k/2
            else
                k = 3*k + 1
            end
            table.insert(returns, k)
        end
    
        for key, value in ipairs(returns) do 
            coroutine.yield(returns[key])
        end
    end


return pa2