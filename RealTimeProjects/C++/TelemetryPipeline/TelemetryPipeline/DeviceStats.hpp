//
//  DeviceStats.hpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef DeviceStats_h
#define DeviceStats_h

#pragma once
#include <algorithm>
#include <limits>

struct DeviceStats {
    int count = 0;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
    double sum = 0;

    void add(double value) {
        count++;
        min = std::min(min, value);
        max = std::max(max, value);
        sum += value;
    }

    double average() const {
        return count == 0 ? 0 : sum / count;
    }
};


#endif /* DeviceStats_h */
