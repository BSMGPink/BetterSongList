#include "Patches/HookBeatmapDataLoader.hpp"

#include "UI/FilterUI.hpp"

namespace BetterSongList::Hooks {
    bool HookBeatmapDataLoader::GetMarkStatus(std::vector<SimpleObstacle> obstacles) {
        if(obstacles.size() == 0)
            return false;

        float wallExistence[2] = {0, 0};

        for(auto& o : obstacles) {
            // Ignore 1 wide walls on left
            if(o.line == 3 || (o.line == 0 && o.width == 1))
                continue;

            // Filter out fake walls, they dont drain energy
            if(o.duration < 0 || o.width <= 0)
                continue;

            // Detect >2 wide walls anywhere, or 2 wide wall in middle
            if(o.width > 2 || (o.width == 2 && o.line == 1)) {
                if(o.layer == 2 || o.layer != 0 && (o.height - o.layer >= 2))
                    return true;
            }

            // Is the wall on the left or right half?
            auto isLeftHalf = o.line <= 1;

            // Check if the other half has an active wall, which would mean there is one on both halfs
            // I know this technically does not check if one of the halves is half-height, but whatever
            if(wallExistence[isLeftHalf ? 1 : 0] >= o.beat)
                return true;

            // Extend wall lengths by 120ms so that staggered crouchwalls that dont overlap are caught
            wallExistence[isLeftHalf ? 0 : 1] = std::max(wallExistence[isLeftHalf ? 0 : 1], o.beat + o.duration + 0.12f);
        }
        return false;
    }
}