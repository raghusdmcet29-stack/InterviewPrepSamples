//
//  main.swift
//  FlyweightPattern
//
//  Created by Anussha on 17/09/26.
//

import Foundation

class TreeType{
    let species : String
    let texture : String
    let color : String
    
    init(species: String, texture: String, color: String) {
        self.species = species
        self.texture = texture
        self.color = color
        print("Created new TreeType: \(species)")  // so we can SEE when creation happens
    }
}

class TreeFactory {
    private var treeTypes:[String:TreeType] = [:]
    func getTreeType(species: String, texture: String, color: String) -> TreeType {
         if let existing = treeTypes[species] {
                return existing
         }
         let newType = TreeType(species: species, texture: texture, color: color)
         treeTypes[species] = newType
         return newType
    }
}

class Tree {
    let x : Int
    let y : Int
    let type : TreeType // shared reference, not owned/copied
    
    init(x: Int, y: Int, type: TreeType) {
        self.x = x
        self.y = y
        self.type = type
    }
    func render() {
            print("Rendering \(type.species) tree at (\(x), \(y)) with texture \(type.texture)")
        }
}

class Forest {
    private var trees : [Tree] = []
    private let factory = TreeFactory()
    
    func plantTree(x: Int, y:Int, species:String, texture: String, color: String){
        let type = factory.getTreeType(species: species, texture: texture, color: color)
        let tree = Tree(x: x, y: y, type: type)
        trees.append(tree)
    }
    func render() {
            for tree in trees {
                tree.render()
            }
        }
}

let forest = Forest()

forest.plantTree(x: 1, y: 2, species: "Oak", texture: "oak.png", color: "green")
forest.plantTree(x: 5, y: 8, species: "Pine", texture: "pine.png", color: "darkgreen")
forest.plantTree(x: 3, y: 3, species: "Oak", texture: "oak.png", color: "green")
forest.plantTree(x: 9, y: 1, species: "Oak", texture: "oak.png", color: "green")
forest.plantTree(x: 7, y: 6, species: "Pine", texture: "pine.png", color: "darkgreen")

forest.render()
/*let factory = TreeFactory()

let oak1 = factory.getTreeType(species: "Oak", texture: "oak.png", color: "green")
let pine1 = factory.getTreeType(species: "Pine", texture: "pine.png", color: "darkgreen")
let oak2 = factory.getTreeType(species: "Oak", texture: "oak.png", color: "green")
let oak3 = factory.getTreeType(species: "Oak", texture: "oak.png", color: "green")
let pine2 = factory.getTreeType(species: "Pine", texture: "pine.png", color: "darkgreen")

print("oak1 === oak2:", oak1 === oak2)   // should be true — same object
print("oak1 === oak3:", oak1 === oak3)   // should be true
print("oak1 === pine1:", oak1 === pine1) // should be false — different species
*/
