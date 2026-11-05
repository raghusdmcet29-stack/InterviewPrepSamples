//
//  Scanner.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//



import Foundation

func fileExists(at path: String) -> Bool {
    return FileManager.default.fileExists(atPath: path)
}
