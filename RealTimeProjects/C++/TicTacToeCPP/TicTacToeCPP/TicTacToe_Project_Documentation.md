# Multiplayer Tic Tac Toe: Project Documentation

**Author:** Anuraag
**Language:** C++17 (server), HTML, CSS and JavaScript (browser page)
**Status:** Core game, server, and browser play are complete and tested. CMakeLists.txt for Windows is pending.

---

## 1. Overview

This project is a two-player Tic Tac Toe game played in two browser windows. A C++ server holds the game state in memory and is the single source of truth. The browser page is a thin client: it sends moves to the server and redraws the board from the server's answer about once a second. The page contains no game rules.

The project was built in small steps, and each step was compiled and tested before the next one was added.

## 2. Architecture

```
 Browser (index.html)                    C++ server (main.cpp)
 ----------------------                  ----------------------------
 Create room button  ---- POST /rooms --->  creates room, gives X token
 Join room button    ---- POST /rooms/N/join -> gives O token
 Click a cell        ---- POST /rooms/N/move?token=&cell= -> Game::play()
 Every second:
   draw()            ---- GET /rooms/N ---->  board, turn, status
   loadOpenRooms()   ---- GET /rooms ------>  list of rooms waiting for O
```

The server is built from these parts:

| Part | Responsibility |
|------|----------------|
| `Board` | Holds the nine cells and answers questions about them (winner, full, can play). |
| `Game` | Owns a `Board` and the current turn. Applies moves and reports status. |
| `Room` (in `main.cpp`) | One game plus the X and O tokens and whether O has joined. |
| `rooms` (in `main.cpp`) | All rooms, keyed by room number. |
| HTTP routes (in `main.cpp`) | Connect browser requests to the game through the `Room` data. |
| `index.html` | Browser page: buttons, board grid, polling, saved seat. |

## 3. Files

| File | Purpose |
|------|---------|
| `Board.hpp`, `Board.cpp` | Board storage and rules for a single grid. |
| `Game.hpp`, `Game.cpp` | Turn handling, move acceptance, game status. |
| `main.cpp` | Room storage and all HTTP routes. |
| `httplib.h` | cpp-httplib, a single-header HTTP server library (third party). |
| `json.hpp` | nlohmann/json, downloaded for later JSON work (third party, not used by the routes yet). |
| `index.html` | Browser client. |

## 4. Game Logic

### 4.1 Board

Cells are numbered 0 to 8, left to right, top to bottom:

```
 0 | 1 | 2
---+---+---
 3 | 4 | 5
---+---+---
 6 | 7 | 8
```

Each cell holds `' '` (empty), `'X'`, or `'O'`. The board provides:

- `set(cell, mark)`: places a mark.
- `canPlay(cell)`: true if the cell is 0 to 8 and empty.
- `cell(index)`: reads one cell, so other code can see the board without changing it.
- `winner()`: returns `'X'` or `'O'` if a line of three exists, otherwise `' '`.
- `isFull()`: true when all nine cells are filled.
- `print()`: prints the grid to the terminal.

### 4.2 Game

- A new game starts with X.
- `play(cell)` returns `true` if the move was accepted, and `false` otherwise. A move is refused if the game is already over, the cell is out of range, or the cell is taken. A refused move does not change the board or the turn.
- An accepted move switches the turn.
- `currentPlayer()` returns whose turn it is.
- `status()` returns `"X wins"`, `"O wins"`, `"Draw"`, or `"playing"`.
- `cellAt(index)` exposes one cell for the server's state route.

## 5. Server Routes

The server listens on `127.0.0.1`, port 8080. It is reachable only from the same computer.

| Method | Path | Purpose | Success reply | Error replies |
|--------|------|---------|---------------|---------------|
| GET | `/hello` | Check that the server is up | `hello` | none |
| GET | `/echo/:text` | Test that URL input is read | the text | none |
| POST | `/rooms` | Create a room; creator is X | `{"roomId":1,"token":"x1","symbol":"X"}` | none |
| POST | `/rooms/{id}/join` | Second player joins; gets O | `{"roomId":1,"token":"o1","symbol":"O"}` | 404 room not found; 409 room already has an O player |
| POST | `/rooms/{id}/move?token=T&cell=N` | Apply a move | `{"accepted":true,"player":"X"}` | 404 room not found; 403 invalid token; 409 not your turn |
| GET | `/rooms/{id}` | Read the board | `{"board":"X...O....","turn":"X","status":"playing"}` | 404 room not found |
| GET | `/rooms` | List rooms waiting for O | `{"open":[2,3]}` | none |

### 5.1 Move validation order

The move route checks in this order and stops at the first failure:

1. Does the room exist? (404)
2. Is the token one of this room's tokens? (403)
3. Is it this token's player's turn? (409)
4. Does the game accept the cell? (`accepted` true or false)

### 5.2 Room data

```cpp
struct Room {
    Game game;
    std::string xToken;   // "x" + room number, e.g. "x1"
    std::string oToken;   // "o" + room number, e.g. "o1"
    bool oJoined = false; // set to true by the first successful join
};
```

Room numbers are assigned in order: 1, 2, 3, and so on.

### 5.3 CORS

The server sends the header `Access-Control-Allow-Origin: *`. Browsers block a page opened from disk from calling a different address unless the server allows it. This header lets the page call the server.

## 6. Browser Page

The page is a single file, `index.html`, with CSS in the head and JavaScript at the end of the body.

### 6.1 Elements

- A heading and an info line (`info`).
- **Create room** button.
- Room number input (`joinId`) and **Join room** button.
- Open rooms area (`openRooms`), filled with one button per open room.
- The board (`board`): nine buttons in a 3x3 grid.

### 6.2 Saved seat

The page stores the player's seat in the browser's `localStorage` under the key `ttt-seat`:

```json
{"roomId": 2, "token": "x2", "symbol": "X"}
```

A seat is saved after a successful create or join. On reload the page reads the seat back, so a refresh keeps the player's place. Two windows in the same browser share this storage, so a second player needs a different browser or a private window.

### 6.3 Functions

| Function | What it does |
|----------|--------------|
| `saveSeat(data)` | Stores the seat in memory and in `localStorage`. |
| `createRoom()` | Calls POST `/rooms` and saves the seat. |
| `joinRoom()` | Calls POST `/rooms/{id}/join` and saves the seat. Returns silently if the server refuses. |
| `sendMove(cell)` | Calls POST `/rooms/{roomId}/move` with the saved token. |
| `draw()` | Every second: reads the room, updates the info line, and redraws the nine cells. |
| `loadOpenRooms()` | Every second: reads `/rooms` and shows one button per open room. Clicking a button joins that room. |

### 6.4 Behavior

- The info line shows `Room N, you are X. Turn: O` during play, and the result (such as `X wins`) when the game ends.
- Clicks are sent to the server for every cell. The server refuses moves that are out of turn or on taken cells. Because the page doesn't show refusals, a refused click looks like nothing happened.
- If the saved room no longer exists (for example after a server restart), `draw()` stops and the board stays empty. A new room must be created or joined.

## 7. Testing Record

Each test was run in Terminal (`curl`) or in the browser.

| # | Test | Result |
|---|------|--------|
| 1 | Game: X wins on the diagonal | Passed (`X wins`) |
| 2 | Game: move refused after game over | Passed (`accepted: 0`, board unchanged) |
| 3 | Server `/hello` and `/echo/:text` | Passed |
| 4 | Create room twice | Passed (rooms 1 and 2) |
| 5 | Join room 1, then join unknown room 99 | Passed (`o1`, then room not found) |
| 6 | Move with valid token, taken cell, bad token | Passed (accepted, refused, invalid token) |
| 7 | Out-of-turn move is refused | Passed (`not your turn`) |
| 8 | Room state after moves | Passed (`X...O....`) |
| 9 | Browser click sends a move to the server | Passed |
| 10 | Two browsers stay in sync | Passed |
| 11 | Second join to a room with O already joined | Passed (409) |
| 12 | Open rooms list after create and join | Passed (`[2]`) |
| 13 | Draw: full board with no line | Passed (`Draw`) |
| 14 | Full game in two browsers, X wins | Passed (both windows show `X wins`) |
| 15 | Third browser join to a full room | Passed (seats unchanged, join refused) |
| 16 | Create room from page, list from Terminal | Passed |
| 17 | Join from open rooms list in a second browser | Passed |

## 8. Problems Found and How They Were Fixed

| Problem | Cause | Fix |
|---------|-------|-----|
| Both players could move as X, so O got three in a row | The move route did not check whose turn it was | Added the turn check (409 not your turn) |
| Anyone could join a full or finished room | The join route had no check | Added `oJoined` and the 409 response |
| Page did not show the grid after a server restart | Saved room no longer existed | Documented; a new room must be created |
| Open rooms list did not appear in the page | `<div id="openRooms">` was placed inside the script block, which broke the script and stopped `createRoom` from loading | Moved the div into the body |
| Join in one browser changed the other browser's seat | Both windows shared one `localStorage` | Use a second browser or a private window for the second player |

## 9. Known Limitations

1. **Memory only.** All rooms are lost when the server restarts.
2. **Predictable tokens.** Tokens are `x1`, `o1`, and so on. Any client that knows a room number can guess its tokens. This is acceptable for a learning project but not for a real game.
3. **Silent failures in the page.** A refused join or move shows nothing. The server returns a message each time, but the page does not display it. An attempt to show the message was made and reverted.
4. **No room cleanup.** Finished rooms stay in memory until the server stops.
5. **Localhost only.** The server listens on `127.0.0.1`, so the second player must be on the same computer.
6. **Shared browser storage.** Two windows in one browser profile share a seat.
7. **Polling.** The page checks for changes once a second, so an opponent's move appears with up to a one-second delay.
8. **Unused library.** `json.hpp` is downloaded but not yet used. The routes build JSON strings by hand.
9. **One compiler warning.** `httplib.h` produces a warning about an uninitialized variable (`op`). It comes from the library, not from project code, and does not affect the program.

## 10. Build and Run

**Requirements:** Xcode on macOS, with the project containing `main.cpp`, `Game.hpp`, `Game.cpp`, `Board.hpp`, `Board.cpp`, `httplib.h`, and `json.hpp`.

**Run the server:**
1. Open the project in Xcode.
2. Press ⌘R. The output shows `server running on port 8080`.

**Run the game:**
1. Open `index.html` in a browser. Right-click it in Xcode and choose Show in Finder, or open it directly.
2. In the first browser, click **Create room**. It shows the room number and that you are X.
3. In a second browser or private window, click the room in the open list, or type the number and click **Join room**.
4. Take turns clicking cells. Both windows update within a second.

**Check the server from Terminal:**

```bash
curl http://127.0.0.1:8080/rooms
curl http://127.0.0.1:8080/rooms/1
```


## 12. Remaining Work

1. Show server error messages in the page (for refused joins and moves).
2. Add `CMakeLists.txt` so the project builds on Windows.
3. Replace predictable tokens with random ones.
4. Remove finished rooms from memory.
