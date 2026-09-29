//
//  main.swift
//  PluggableSortStrategy
//
//  Created by Anussha on 03/09/26.
//

import Foundation

protocol SortStrategy {
    func sort(_ array : [Int]) -> [Int]
}

struct BubbleSortStrategy: SortStrategy {
    func sort(_ array: [Int]) -> [Int] {
        var result = array
        for i in 0..<result.count {
            for j in 0..<result.count - i - 1 {
                if result[j] > result[j + 1]{
                    result.swapAt(j, j + 1)
                }
            }
        }
        return result
    }
}

struct QuickSortStrategy: SortStrategy {
    func sort(_ array: [Int]) -> [Int] {
        guard array.count > 1 else { return array }
        let pivot = array[array.count / 2]
        let less = array.filter { $0 < pivot }
        let equal = array.filter { $0 == pivot }
        let greater = array.filter { $0 > pivot }
        return sort(less) + equal + sort(greater)
    }
}


class Sorter {
    private var strategy: SortStrategy

    init(strategy: SortStrategy) {
        self.strategy = strategy
    }

    func setStrategy(_ strategy: SortStrategy) {
        self.strategy = strategy
    }

    func performSort(_ array: [Int]) -> [Int] {
        strategy.sort(array)
    }
}

// Usage — prove the swap on the SAME Sorter instance:
let sorter = Sorter(strategy: BubbleSortStrategy())
print(sorter.performSort([5, 2, 8, 1, 9]))   // [1, 2, 5, 8, 9] — via BubbleSort

sorter.setStrategy(QuickSortStrategy())
print(sorter.performSort([5, 2, 8, 1, 9]))   // [1, 2, 5, 8, 9] — via QuickSort, same
