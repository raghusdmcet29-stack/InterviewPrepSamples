//
//  main.swift
//  UndefinedBehavior1-SignedOverflow
//
//  Created by Anussha on 09/10/26.
//
//he &+= operator wraps the value around the range, so 127 + 1 becomes -128.
var big: Int8 = 127
big &+= 1
print(big)


//crash integer operation for overflow, and when big += 1 goes past 127
/*var big: Int8 = 127
big += 1
print(big)*/

