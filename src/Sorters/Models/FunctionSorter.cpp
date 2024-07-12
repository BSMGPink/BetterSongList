#include "Sorters/Models/FunctionSorter.hpp"

#include "Utils/SongListLegendBuilder.hpp"

namespace BetterSongList {
    /* PrimitiveFunctionSorter */
    PrimitiveFunctionSorter::PrimitiveFunctionSorter(
        const PrimitiveFunctionSorter::ValueGetterFunc& sortFunc
    ) : 
        ISorterPrimitive(), 
        sortValueGetter(sortFunc) {}

    bool PrimitiveFunctionSorter::get_isReady() const {
        return true;
    }

    std::future<void> PrimitiveFunctionSorter::Prepare() { return std::async(std::launch::deferred, []{}); }

    std::optional<float> PrimitiveFunctionSorter::GetValueFor(GlobalNamespace::BeatmapLevel* level) const {
        return sortValueGetter(level);
    }
    /* PrimitiveFunctionSorter */

    /* PrimitiveFunctionSorterWithLegend */
    PrimitiveFunctionSorterWithLegend::PrimitiveFunctionSorterWithLegend(
        const PrimitiveFunctionSorter::ValueGetterFunc& sortFunc, 
        const PrimitiveFunctionSorterWithLegend::LegendGetterFunc& legendFunc
    ) : 
        PrimitiveFunctionSorter(sortFunc), 
        ISorterWithLegend(), 
        legendValueGetter(legendFunc) {}
    
    ISorterWithLegend::Legend PrimitiveFunctionSorterWithLegend::BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const {
		return SongListLegendBuilder::BuildFor(levels, legendValueGetter);
    }

    /* PrimitiveFunctionSorterWithLegend */
    /* ComparableFunctionSorter */

    ComparableFunctionSorter::ComparableFunctionSorter(
        const CompareFunc& sortFunc
    ) : 
        ISorterCustom(), 
        sortValueGetter(sortFunc) {}

    bool ComparableFunctionSorter::get_isReady() const {
        return true;
    }

    std::future<void> ComparableFunctionSorter::Prepare() { return std::async(std::launch::deferred, []{}); }

    void ComparableFunctionSorter::DoSort(ArrayW<GlobalNamespace::BeatmapLevel*>& levels, bool ascending) const {
        if (ascending) {
            std::sort(levels.rbegin(), levels.rend(), *this);
        } else {
            std::sort(levels.begin(), levels.end(), *this);
        }
    }

    int ComparableFunctionSorter::operator ()(GlobalNamespace::BeatmapLevel* lhs, GlobalNamespace::BeatmapLevel* rhs) const {
        return sortValueGetter(lhs, rhs);
    }
    /* ComparableFunctionSorter */
    /* ComparableFunctionSorterWithLegend */
    
    ComparableFunctionSorterWithLegend::ComparableFunctionSorterWithLegend(
        const ComparableFunctionSorter::CompareFunc& sortFunc, 
        const ComparableFunctionSorterWithLegend::LegendGetterFunc& legendFunc
    ) :
        ComparableFunctionSorter(sortFunc),
        ISorterWithLegend(),
        legendValueGetter(legendFunc) {

    }

    ISorterWithLegend::Legend ComparableFunctionSorterWithLegend::BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const {
		return SongListLegendBuilder::BuildFor(levels, legendValueGetter);
    }
    /* ComparableFunctionSorterWithLegend */
}