# ChunkSync Documentation

ChunkSync copies a file from a client to a server in 4 KB chunks over TCP. Each chunk is identified by its SHA-256 fingerprint. The server stores each chunk once, so a chunk it already has is never sent again.

Status: Swift version complete at commit f928f72 (InterviewPrepSamples, RealTimeProjects folder).

## 1. Projects

| Project | Role | Main files |
|---|---|---|
| ChunkSyncServer | Listens on port 9100, stores chunks, rebuilds files | main.swift (startServer call), Server.swift, ChunkStore.swift, FileAssembler.swift, LineBuffer.swift, Protocol.swift |
| ChunkSyncClient | Reads a file, sends it to the server | main.swift, Protocol.swift |

`Protocol.swift` must be identical in both projects. It defines the constants, the commands, the fingerprint function, and the line parser.

## 2. Constants

| Name | Value | Meaning |
|---|---|---|
| serverPort | 9100 | TCP port the server listens on |
| chunkSize | 4096 | Bytes per piece |
| respOK | "OK" | Success reply |
| respErr | "ERR" | Failure reply |
| respHaveIt | "HAVE_IT" | Server already has the chunk |
| respNeed | "NEED" | Server does not have the chunk |

## 3. Protocol

Every message is one text line ending in `\n`, except CHUNK, which is followed by raw bytes.

| Command | Sent by | Format | Server reply |
|---|---|---|---|
| BEGIN | Client | `BEGIN <name>` | OK or ERR |
| HAVE | Client | `HAVE <fp>` | HAVE_IT or NEED |
| CHUNK | Client | `CHUNK <fp> <length>\n` then `<length>` bytes | OK or ERR |
| APPEND | Client | `APPEND <name> <fp>` | OK or ERR |
| COMMIT | Client | `COMMIT <name>` | OK or ERR |

Rules:
- `<fp>` is 64 lowercase hex characters (SHA-256).
- `<length>` is from 1 to 4096.
- `<name>` has no `/`, does not start with `.`, and is at most 255 characters.
- Any line that does not match these rules gets `ERR`.

## 4. Sync flow (client side)

The client handles one piece at a time.

1. Connect to 127.0.0.1:9100.
2. Send `BEGIN <file name>`. Wait for OK.
3. Open the file and read the next 4096-byte piece.
4. Compute the piece's fingerprint.
5. Send `HAVE <fp>`. Read the reply.
6. If the reply is NEED, send `CHUNK <fp> <length>` followed by the piece's bytes. Wait for OK.
7. Send `APPEND <name> <fp>`. Wait for OK.
8. Repeat from step 3 until the file ends.
9. Send `COMMIT <name>`. Wait for OK.
10. Print the counts (`Chunks: <total>, sent: <sent>`) and exit with code 0.

Any unexpected reply stops the sync with an error and exit code 1.

## 5. Sync flow (server side)

For each connection, the server:

1. Receives bytes and adds them to a `LineBuffer`.
2. Takes the next full line (up to `\n`) and parses it with `parseCommand`.
3. Runs the matching action:
   - HAVE: checks whether `chunks/<fp>` exists. Replies HAVE_IT or NEED.
   - CHUNK: stores the header, then reads exactly `<length>` bytes. Hashes the bytes. If the hash equals `<fp>`, saves them to `chunks/<fp>` and replies OK. Otherwise replies ERR and saves nothing.
   - BEGIN: creates an empty `parts/<name>.part` file. Replies OK.
   - APPEND: reads `chunks/<fp>` and writes its bytes to the end of `parts/<name>.part`. Replies OK, or ERR if the chunk is missing.
   - COMMIT: closes `parts/<name>.part` and moves it to `files/<name>`. Replies OK.

Any command that fails inside the server gets ERR and the connection stays open.

## 6. Server storage

All server data is under `~/ChunkSyncServer/`.

| Folder | Contents | Used by |
|---|---|---|
| chunks/ | One file per chunk. The file name is the fingerprint and the contents are the chunk's bytes. | HAVE, CHUNK, APPEND |
| parts/ | One `<name>.part` file per upload in progress | BEGIN, APPEND, COMMIT |
| files/ | Completed files | COMMIT |

Memory: the server keeps no list of chunks. Each HAVE is one disk lookup. Each chunk is held in memory only while it is being received or copied.

Chunks are written atomically (to a temporary file, then renamed), so a crash cannot leave a half-written chunk.

## 7. Client memory and reads

- The file is never loaded whole. Each piece is read with `FileHandle.read(upToCount: 4096)`.
- Only the current piece is held in memory.
- The client reads the file once from the start, one piece at a time. It keeps no list of fingerprints.

## 8. Example

File `test.txt` of 8192 bytes, server empty:

- Piece 0 (4096 bytes): HAVE → NEED; CHUNK → OK; APPEND → OK
- Piece 1 (4096 bytes): HAVE → NEED; CHUNK → OK; APPEND → OK
- COMMIT → OK

Result: `files/test.txt` matches the original (checked with `cmp`).

Run again with the server unchanged:

- Piece 0: HAVE → HAVE_IT; APPEND → OK
- Piece 1: HAVE → HAVE_IT; APPEND → OK
- COMMIT → OK

Result: `Chunks: 2, sent: 0`. No data crossed the network.

## 9. Logs

The server prints each step, in order:

- `[recv] N bytes arrived`: bytes received from the client
- `[buffer] pendingChunk: ...`: whether the server is waiting for CHUNK bytes
- `Line: ...`: a complete text line
- `[cmd] parsed: ...`: the command after parsing
- `[have]`, `[chunk-header]`, `[chunk] got ...`, `[chunk] verifying ...`, `[append]`, `[begin]`, `[commit]`: the action taken
- `Error: ...`: a failure

The client prints each piece and each step: `[piece]`, `[client have]`, `[client answer]`, `[client chunk]`, `[client append]`, `[client commit]`.

## 10. Tests completed

| Test | Result |
|---|---|
| 16-byte file, first sync | NEED, one chunk, cmp matched |
| 16-byte file, after server restart | HAVE_IT, sent 0 |
| 8192-byte file (two full chunks) | Sent 2, cmp matched |
| 20000-byte file (four full chunks and one 3616-byte chunk) | Sent 5, cmp matched |
| Restart with chunks on disk | Server remembers chunks, sent 0 |
| CHUNK with wrong bytes | Server replies ERR, saves nothing |
| APPEND of unknown chunk | Server replies ERR |
| Client stops before COMMIT | Unfinished file stays in parts/, not in files/ |
| Deleted chunks/ folder during run | Recreated, sync succeeds |

## 11. Known limits (not implemented)

- No limit on line length. A client that sends a very long line with no newline makes the server's buffer grow.
- If `parts/` or `files/` is deleted while the server runs, BEGIN or COMMIT fails. Restarting the server fixes it.
- The client path is hardcoded to `~/test.txt`, and the remote name is `test.txt`.
- No retry after a failed chunk. The sync stops and must be run again.
- The client reads the whole file once on every sync, even when nothing changed.

## 12. How to run

1. Build and start ChunkSyncServer. It prints `Waiting for clients on port 9100...`.
2. Build and run ChunkSyncClient. It syncs `~/test.txt`.
3. Compare the result: `cmp ~/test.txt ~/ChunkSyncServer/files/test.txt`. No output means identical.

To reset the server, stop it, run `rm -rf ~/ChunkSyncServer`, and start it again.

## 13. Version control

Both projects are in the InterviewPrepSamples repository, in the RealTimeProjects folder, on branch main. The last commit for this project is f928f72.
