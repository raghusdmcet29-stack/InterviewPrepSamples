//
//  main.cpp
//  FlyweightPattern
//
//  Created by Anussha on 17/09/26.
//

#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>

class TreeType {
public:
    std::string species;
    std::string texture;
    std::string color;
    
    TreeType(std::string species, std::string texture, std::string color)
    : species(std::move(species)),texture(std::move(texture)),color(std::move(color)){
        std::cout << "Created new TreeType: " << this->species << "\n";
    }
};

class TreeFactory {
private:
    std::unordered_map<std::string, std::unique_ptr<TreeType>> treeTypes;
public:
    TreeType* getTreeType(const std::string& species, const std::string& texture, const std::string& color) {
        auto it = treeTypes.find(species);
        if (it != treeTypes.end()) {
            return it->second.get();
        }
            auto newType = std::make_unique<TreeType>(species, texture, color);
            TreeType* rawPtr = newType.get();
            treeTypes[species] = std::move(newType);
            return rawPtr;
        }
};

class Tree {
private:
    int x,y;
    TreeType* type; // non-owning, borrowed pointer
    
public:
    Tree(int x, int y, TreeType* type) : x(x), y(y), type(type){}
    void render() const {
        std::cout << "Rendering " << type->species << " tree at (" << x << ", " << y
                       << ") with texture " << type->texture << "\n";
        }
};

class Forest {
private:
    std::vector<Tree>trees;
    TreeFactory factory;
public:
    void plantTree(int x, int y, const std::string& species, const std::string& texture, const std::string& color) {
        TreeType* type = factory.getTreeType(species, texture, color);
        trees.emplace_back(x, y, type);
    }

    void render() const {
        for (const auto& tree : trees) {
            tree.render();
        }
    }
};

int main() {
    Forest forest;

    forest.plantTree(1, 2, "Oak", "oak.png", "green");
    forest.plantTree(5, 8, "Pine", "pine.png", "darkgreen");
    forest.plantTree(3, 3, "Oak", "oak.png", "green");
    forest.plantTree(9, 1, "Oak", "oak.png", "green");
    forest.plantTree(7, 6, "Pine", "pine.png", "darkgreen");

    forest.render();

    return 0;
}

/*int main() {
    TreeFactory factory;

    TreeType* oak1 = factory.getTreeType("Oak", "oak.png", "green");
    TreeType* pine1 = factory.getTreeType("Pine", "pine.png", "darkgreen");
    TreeType* oak2 = factory.getTreeType("Oak", "oak.png", "green");
    TreeType* oak3 = factory.getTreeType("Oak", "oak.png", "green");
    TreeType* pine2 = factory.getTreeType("Pine", "pine.png", "darkgreen");

    std::cout << "oak1 == oak2: " << (oak1 == oak2) << "\n";
    std::cout << "oak1 == oak3: " << (oak1 == oak3) << "\n";
    std::cout << "oak1 == pine1: " << (oak1 == pine1) << "\n";

    return 0;
}*/
