//
//  main.swift
//  IteratorPattern
//
//  Created by Anussha on 10/09/26.
//

// MARK: - Interfaces
import Foundation

/* Normal String version
 
 
class PlaylistIterator {
    private let songs: [String]
    private var currentIndex = 0

    init(songs: [String]) {
        self.songs = songs
    }

    func hasNext() -> Bool {
        return currentIndex < songs.count
    }

    func next() -> String {
        let song = songs[currentIndex]
        currentIndex += 1
        return song
    }
}

struct PlaylistCollection {
    private var songs: [String] = []

    mutating func add(_ song: String) {
        songs.append(song)
    }

    func makeIterator() -> PlaylistIterator {
        return PlaylistIterator(songs: songs)
    }
}

var playlist = PlaylistCollection()
playlist.add("Bohemian Rhapsody")
playlist.add("Stairway to Heaven")
playlist.add("Hotel California")

let iterator = playlist.makeIterator()

while iterator.hasNext() {
    print(iterator.next())
}
*/

// Generic version

class PlaylistIterator<T> {
    private let items: [T]
    private var currentIndex = 0

    init(items: [T]) {
        self.items = items
    }

    func hasNext() -> Bool {
        return currentIndex < items.count
    }

    func next() -> T {
        let item = items[currentIndex]
        currentIndex += 1
        return item
    }
}

struct PlaylistCollection<T> {
    private var items: [T] = []

    mutating func add(_ item: T) {
        items.append(item)
    }

    func makeIterator() -> PlaylistIterator<T> {
        return PlaylistIterator(items: items)
    }
}

var playlist = PlaylistCollection<String>()
playlist.add("Bohemian Rhapsody")
playlist.add("Stairway to Heaven")
playlist.add("Hotel California")

let iterator = playlist.makeIterator()

while iterator.hasNext() {
    print(iterator.next())
}

var playlistInt = PlaylistCollection<Int>()
playlistInt.add(1)
playlistInt.add(2)
playlistInt.add(3)

let intIterator = playlistInt.makeIterator()

while intIterator.hasNext() {
    print(intIterator.next())
}
