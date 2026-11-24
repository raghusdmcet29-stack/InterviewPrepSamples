//
//  main.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
//

import Foundation

// The store lives in Server.swift now, so nothing is needed here for it.



// Start the server the same way you did before the ChunkStore test.
startServer()   // <-- replace with your real start call

// Keeps the command-line program running so the listener can accept connections.
dispatchMain()
