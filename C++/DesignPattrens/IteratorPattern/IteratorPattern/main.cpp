//
//  main.cpp
//  IteratorPattern
//
//  Created by Anussha on 10/09/26.
//

#include <iostream>

template <typename T>
class PlaylistIterator {
public:
    PlaylistIterator(const std::vector<T>& items) : items(items) {}

    bool hasNext() const {
        return currentIndex < items.size();
    }

    T next() {
        return items[currentIndex++];
    }

private:
    std::vector<T> items;
    size_t currentIndex = 0;
};

template <typename T>
class PlaylistCollection {
public:
    void add(const T& item) {
        items.push_back(item);
    }

    PlaylistIterator<T> makeIterator() const {
        return PlaylistIterator<T>(items);
    }

private:
    std::vector<T> items;
};


int main() {
    PlaylistCollection<std::string> playlist;
    playlist.add("Bohemian Rhapsody");
    playlist.add("Stairway to Heaven");
    playlist.add("Hotel California");

    auto iterator = playlist.makeIterator();

    while (iterator.hasNext()) {
        std::cout << iterator.next() << "\n";
    }
    
    PlaylistCollection<int> playlistInt;
    playlistInt.add(1);
    playlistInt.add(2);
    playlistInt.add(3);

    auto intIterator = playlistInt.makeIterator();

    while (intIterator.hasNext()) {
        std::cout << intIterator.next() << "\n";
    }

    return 0;
}
