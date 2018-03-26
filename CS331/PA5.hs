-- PA5.hs
-- Duane Shaffer
-- 25 Mar 2018
--
-- For CS F331 Spring 2018
-- Solutions to Assignment 5 Exercise B

-- Based off a skeleton file from Glen G. Chappell

module PA5 where

import Data.List
-- findIndex
-- isPrefixOf
-- transpose

--------------------------------------------------------------
-- Part 1: Variable collatzCounts. This is a list of Integer values. 
-- Item k (counting from zero) of collatzCounts tell how many iterations 
-- of the Collatz function are required to take the number k+1 to 1.
--------------------------------------------------------------

-- collatzCounts
collatzCounts :: [Integer]
collatzCounts = map numTimes_CC_Called [1..]

-- numTimes_CC_Called
-- Counter for number of times that the collatz function was needed to get inputNum + 1 down to 1
numTimes_CC_Called inputNum
  | inputNum == 1                     = 0
  | collatzFunction inputNum == 1     = 1
  | otherwise                         = 1 + numTimes_CC_Called (collatzFunction inputNum)

-- collatzFunction
-- special function used for exercise B part 1. Implemented using guards
collatzFunction inputNum
  | inputNum == 0       = 0
  | mod inputNum 2 == 0 = div inputNum 2
  | otherwise           = 3 * inputNum + 1



--------------------------------------------------------------
-- Part 2: Function findList. This takes two lists of the same type. 
-- It returns a Maybe Int. It the first list is found as a continguous 
-- sublist of the second list, then the return value is Just n, where n 
-- is the earlier index (starting from zero) at which a copy of the first 
-- list begins. If the first list is not found as a contiguous sublist of 
-- the second, then the return value is Nothing.
--------------------------------------------------------------

-- findList
findList :: Eq a => [a] -> [a] -> Maybe Int
findList list1 list2 = findIndex (isPrefixOf list1) (tails list2)



--------------------------------------------------------------
-- Part 3: Infix operator ##. The two operands are lists of the same 
-- type. The return value is an Int giving the number of indices at 
-- which the two lists contain equal values.
--------------------------------------------------------------

-- operator ##
(##) :: Eq a => [a] -> [a] -> Int
list1 ## list2 = n where
    n = countEquals (transpose [list1, list2])

-- countEquals
-- helper function for ## that returns the number of indices that
-- have equal values
countEquals list
  | length list == 0                      = 0
  | length (head list) <= 1               = 0
  | head (head list) == last (head list)  = 1 + countEquals (tail list)
  | otherwise                             = countEquals (tail list)

--------------------------------------------------------------
-- Part 4: Function filterAB. This takes a boolean function and two lists. 
-- It returns a list of all items in the second list for which the 
-- corresponding item in the first list makes the boolean function true.
--------------------------------------------------------------

-- filterAB
filterAB :: (a -> Bool) -> [a] -> [b] -> [b]
filterAB fn list1 list2 = returnList where 
    listToFilter = zip list1 list2
    filteredList = filter (fn.fst) listToFilter
    returnList = getTuplesecond (unzip filteredList)

-- Helper function for part 4 and 5
-- given a tuple, retrieve the first or second element
getTuplefirst (first,_) = first
getTuplesecond (_,second) = second

--------------------------------------------------------------
-- Part 5: Function sumEvenOdd. This takes a list of numbers. It returns a 
-- tuple of two numbers: the sum of the even-index items in the given list, 
-- and the sum of the odd-index items in the given list. Indices are zero-based.
--------------------------------------------------------------

-- sumEvenOdd
sumEvenOdd :: Num a => [a] -> (a, a)
sumEvenOdd list = tuple where
    tupleParts = parts list
    evenSum = foldr (+) 0 (getTuplefirst tupleParts)
    oddSum = foldr (+) 0 (getTuplesecond tupleParts)
    tuple = (evenSum, oddSum)

parts [] = ([], [])
parts [x] = ([x],[])
parts (x:y:xs) = (x:xp, y:yp ) where (xp, yp) = parts xs