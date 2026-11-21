#pragma once

#include <QtGlobal>
#include <iterator>
#include <optional>

struct LowBatteryAlert {
    quint8 stage;  // the stage crossed: 20, 10 or 5
    quint8 level;  // the reading that crossed it
};

// Staged low-battery alerts for one source (a pod or the case).
class LowBatteryWatcher {
public:
    static constexpr quint8 stages[] = {20, 10, 5};
    static constexpr quint8 rearmLevel = 25;

    // A reading that crosses several stages yields one alert, for the deepest.
    std::optional<LowBatteryAlert> evaluate(quint8 level, bool available, bool charging) {
        if (!available) return std::nullopt;
        if (charging || level >= rearmLevel) {
            m_stagesDone = 0;
            return std::nullopt;
        }
        int crossed = 0;
        while (crossed < int(std::size(stages)) && level <= stages[crossed]) ++crossed;
        if (crossed <= m_stagesDone) return std::nullopt;
        m_stagesDone = crossed;
        return LowBatteryAlert{stages[crossed - 1], level};
    }

private:
    int m_stagesDone = 0;
};
