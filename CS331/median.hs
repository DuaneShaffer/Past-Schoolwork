-- median.hs
-- Duane Shaffer
-- 25 Mar 2018
--
-- For CS F331 Spring 2018
-- Solution to Assignment 5 Exercise C

-- Program returns the median value of a list of input numbers.
-- NOTE: 
--    Does not work for floating point numbers
--    Will not parse non numeric input
--    Uses 9223372036854775806 (the max possible integer value -1) 
--      as an error code. If the median is this value the program 
--      will think there's an empty list.
--    Given an even amount of inputs, the lower of the two middle
--      values is chosen as the median.

import System.IO
import Data.List
import Data.Char

main = do
    putStrLn "Enter a list of integers, one on each line."
    putStrLn "I will compute the median of the list."
    ans <- median
    hFlush stdout
    if ans == 9223372036854775806
        then
            putStrLn ("Empty list - no median")
        else do
            putStr "Median is :"
            print (ans)
            
    hFlush stdout

    putStrLn "Compute another median? (y/n) "
    startOver <- getLine
    if map toLower startOver == "y"
        then 
            main
        else do
            putStrLn "Bye!"

median = do
    nums <- numlist
    if length nums == 0
        then
            -- Return special value indicating error
            return 9223372036854775806
        else do 
            -- Sort the list
            sortedNums <- listsort (nums)

            -- Get length of the list
            lengthOfList <- listlength (nums)

            -- Use length to find index of the middle
            mid <- mid (lengthOfList)

            -- Return value indexed at the middle
            return (sortedNums !! mid)

numlist = do
    putStrLn "Enter number (blank line to end):"
    input <- getLine
    if input == "" 
        then 
            return []
        else do
            let num = read input :: Int
            nextNum <- numlist
            return (num:nextNum)


listsort inputNums = do
    return (sort inputNums)
    
listlength inputNums = do
    return (length inputNums )

mid lengthOfList = do
    return (div lengthOfList 2)
