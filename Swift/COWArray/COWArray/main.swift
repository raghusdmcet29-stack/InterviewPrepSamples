//
//  main.swift
//  COWArray
//
//  Created by Anussha on 26/08/26.
//

import Foundation

final class Storage<Element>{
    var elements : [Element]
    
    init(elements: [Element]) {
        self.elements = elements
    }
    func copy()-> Storage<Element>{
        return Storage(elements: elements)
    }
}

struct COWArray<Element>{
    private var storage : Storage<Element>
    
    init(_ elements : [Element] = []) {
        self.storage = Storage(elements: elements)
    }
}

extension COWArray{
    var count : Int{
        storage.elements.count
    }
    
    subscript(index : Int) -> Element{
        get {
            storage.elements[index]
        }
        set {
            if !isKnownUniquelyReferenced(&storage){
                print("Copying COWArray")
                storage = storage.copy()
            }
            storage.elements[index] = newValue
        }
    }
}

var a = COWArray([1, 2, 3])
var b = a   // struct copy — but storage is still SHARED (same Storage instance)
var c = a
print("Before mutation:")
print("a[0] =", a[0])
print("b[0] =", b[0])
print("c[0] =", c[0])
b[0] = 99   // triggers isKnownUniquelyReferenced check -> false (shared) -> copy -> mutate copy
c[0] = 199
print("After mutating b[0] and c[0] :")
print("a[0] =", a[0])   // should still be 1
print("b[0] =", b[0])   // should be 99
print("c[0] =", c[0])
var d = COWArray([1, 2, 3])
d[0] = 50   // d is the SOLE owner from creation — no other variable shares it
d[1] = 51
d[2] = 52
print("d[0] =", d[0])   // should still be 1
print("d[1] =", d[1])   // should be 99
print("d[2] =", d[2])


