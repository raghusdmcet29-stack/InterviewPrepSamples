# BackupStore: How the Algorithm Works

## The one-sentence idea

Cut a file into 4 KB chunks, save each **unique** chunk once in a data file (`store.dat`), and save the index that says how to rebuild the original file from those chunks.

## Files in the project

| File | Purpose |
|---|---|
| `ChunkStore.hpp / .cpp` | Shared types (`ChunkLocation`, `Store`, `Order`) and `readChunk` |
| `Backup.hpp / .cpp` | Backup controller: reads the input file and writes the store and indexes |
| `Restore.hpp / .cpp` | Restore controller: loads the indexes and rebuilds the file |
| `main.cpp` | Asks for the backup file path and the restore folder, then calls the two controllers |

Files written by a backup run, in the program's working folder:

| File | What it holds |
|---|---|
| `store.dat` | The unique chunk bytes, one after another |
| `store.cat` | The catalog: one line per unique chunk (`fingerprint offset length`) |
| `store.idx` | The reading list: one line per chunk of the original file (`fingerprint position`) |

## The library picture

1. **`store.dat` is the shelf.** It holds the chunk bytes. Each unique chunk is stored exactly once.
2. **`store.cat` is the catalog.** For each fingerprint (a short ID calculated from a chunk's bytes), it records where that chunk sits on the shelf: an offset (start position) and a length.
3. **`store.idx` is the reading list.** It records, in the same order as the original file, which catalog entry each chunk uses.

## Key terms

- **Chunk:** a piece of the input file, 4096 bytes each. The last chunk may be shorter.
- **Fingerprint:** a number calculated from a chunk's bytes using `std::hash`. Identical bytes always give the same fingerprint. Different chunks can occasionally share one, so the bytes are also compared.
- **Shelf:** all stored entries that share one fingerprint. Usually a shelf holds one entry.
- **Offset:** where a chunk starts inside `store.dat`, counted from byte 0.
- **Length:** how many bytes that chunk takes.

## Backup: step by step

1. **Open `store.dat`** fresh (old contents are erased) and set `storeEnd = 0`, the position where the next new chunk will be written.
2. **Read the next chunk** from the input file (up to 4096 bytes).
3. **Compute its fingerprint.**
4. **Look up the shelf** for that fingerprint in the catalog.
5. **Compare bytes.** For each entry on the shelf, read its bytes from `store.dat` and compare them with the new chunk.
   - **Match found:** duplicate. Nothing is written. Record the matching position.
   - **No match:** new. Write its bytes at `storeEnd`, add `{offset, length}` to the shelf, and move `storeEnd` forward by the chunk's length.
6. **Record** `(fingerprint, position on shelf)` in the reading list.
7. **Repeat** until the input file ends.
8. **Save the indexes:** write `store.idx` (the reading list) and `store.cat` (the catalog).

## Restore: step by step

1. **Load `store.cat`** into the catalog. Each line becomes an entry on its shelf, in the saved order.
2. **Load `store.idx`** into the reading list.
3. **Open `store.dat`** for reading.
4. **Create the output file** in the folder you chose, named `<original name>_restored.txt`.
5. **For each reading-list entry in order:** look up its address in the catalog, read those bytes from `store.dat`, and write them to the output file.

Restore needs only `store.dat`, `store.cat`, and `store.idx`. The original input file is not needed.

## Worked example

Suppose the file has three chunks: `AAA`, `BBB`, `AAA`. (Real chunks are 4096 bytes. The logic is the same.)

**Backup**

**Chunk 1: `AAA`**
- Fingerprint `h1`. The shelf is empty, so it's new.
- Write `AAA` at offset 0. `storeEnd` becomes 3.
- Catalog: `h1 → {offset 0, length 3}`
- Reading list: `(h1, 0)`
- Output: `new`

**Chunk 2: `BBB`**
- Fingerprint `h2`. The shelf is empty, so it's new.
- Write `BBB` at offset 3. `storeEnd` becomes 6.
- Catalog: `h2 → {offset 3, length 3}`
- Reading list: `(h2, 0)`
- Output: `new`

**Chunk 3: `AAA`**
- Fingerprint `h1`. The shelf has `{offset 0, length 3}`.
- Read 3 bytes from offset 0: `AAA`. It matches, so it's a duplicate. Nothing is written.
- Reading list: `(h1, 0)`, pointing to the same stored copy.
- Output: `duplicate`

**Saved files**

| File | Contents |
|---|---|
| `store.dat` | `AAABBB` (6 bytes instead of 9) |
| `store.cat` | `h1 0 3` and `h2 3 3`, one line per unique chunk |
| `store.idx` | `h1 0`, `h2 0`, `h1 0`, one line per chunk of the original |

**Restore**

Reading list `(h1,0), (h2,0), (h1,0)`:
- `(h1,0)` reads `AAA` from offset 0.
- `(h2,0)` reads `BBB` from offset 3.
- `(h1,0)` reads `AAA` from offset 0 again.

Output: `AAABBBAAA`, identical to the original.

## Why this design

- **Less disk space when there are repeats:** repeated chunks are stored once.
- **Low memory use:** RAM holds only fingerprints, offsets, lengths, and the reading list. Chunk bytes stay on disk.
- **Safe duplicate detection:** the fingerprint only narrows the search. The bytes are compared, so two different chunks with the same fingerprint are never confused.
- **Restore without the original:** the saved indexes are enough to rebuild the file later.

## Main rule

**Write each unique chunk once. Repeated chunks only add a pointer.**

## Limitations (current version)

- **Only one backup at a time.** `store.dat`, `store.cat`, and `store.idx` are overwritten on each backup run.
- **Repeats are found only within one file.** Chunks are not shared with other backups.
- **Fixed 4 KB chunks.** If a few bytes are inserted early in a file, every later chunk shifts and may no longer match.
- **No metadata.** File name, folder, dates, and permissions are not saved. Restore takes the name from the backup file path.
- **No compression or encryption.**
- **Byte comparison reads from disk**, which is slower than comparing in memory.
- **`std::hash` is not cryptographic.** It is fine for learning, not for real backups.

## Output messages

- `chunk N: new, X bytes`: the chunk was written to `store.dat`.
- `chunk N: duplicate`: the chunk already existed, so nothing was written.
- `total chunks: T, unique stored: U`: T chunks in the file, U unique chunks stored.
- `backup done. index saved to: <path>`: `store.idx` and `store.cat` were written.
- `restored to <path>`: the file was rebuilt into the chosen folder.
