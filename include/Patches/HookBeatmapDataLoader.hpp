#pragma once

#include <vector>

namespace BetterSongList::Hooks {
    struct SimpleObstacle {
        int line;
        int layer;
        int width;
        int height;
        float duration;
        float beat;
    };
    class HookBeatmapDataLoader {
        public:
            /// @brief prio int max value
            static bool GetMarkStatus(std::vector<SimpleObstacle> obstacles);
    };
}