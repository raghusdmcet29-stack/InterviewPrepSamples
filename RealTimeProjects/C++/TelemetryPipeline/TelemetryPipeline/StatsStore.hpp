//
//  StatsStore.hpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef StatsStore_h
#define StatsStore_h

#pragma once
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include "Reading.hpp"
#include "DeviceStats.hpp"

class StatsStore {
public:
    void record(const Reading& reading) {
        std::unique_lock lock(m_);   // writer: exclusive
        stats_[reading.deviceID].add(reading.value);
    }

    std::optional<DeviceStats> snapshot(const std::string& deviceID) const {
        std::shared_lock lock(m_);   // reader: shared
        auto it = stats_.find(deviceID);
        if (it == stats_.end()) return std::nullopt;
        return it->second;           // copy, so the lock can be released safely
    }

private:
    mutable std::shared_mutex m_;    // mutable so const snapshot() can lock it
    std::unordered_map<std::string, DeviceStats> stats_;
};


#endif /* StatsStore_h */
