#pragma once

#include "ISorter.hpp"

namespace BetterSongList {
    class PrimitiveFunctionSorter : public ISorterPrimitive {
        public:
            using ValueGetterFunc = std::function<std::optional<float>(GlobalNamespace::BeatmapLevel*)>;
            PrimitiveFunctionSorter(const ValueGetterFunc& sortFunc);
            virtual bool get_isReady() const override;
            virtual std::future<void> Prepare() override;
            virtual std::optional<float> GetValueFor(GlobalNamespace::BeatmapLevel* level) const override;
        private:
            ValueGetterFunc sortValueGetter;
    };

    class PrimitiveFunctionSorterWithLegend : public PrimitiveFunctionSorter, public ISorterWithLegend {
        public:
            using LegendGetterFunc = std::function<std::string(GlobalNamespace::BeatmapLevel*)>;
            PrimitiveFunctionSorterWithLegend(const ValueGetterFunc& sortFunc, const LegendGetterFunc& legendFunc);
            virtual ISorterWithLegend::Legend BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const override;
        private:
            LegendGetterFunc legendValueGetter;
    };

    class ComparableFunctionSorter : public ISorterCustom {
        public:
            using CompareFunc = std::function<int(GlobalNamespace::BeatmapLevel*, GlobalNamespace::BeatmapLevel*)>;
            ComparableFunctionSorter(const CompareFunc& sortFunc);
            virtual bool get_isReady() const override;
            virtual std::future<void> Prepare() override;
            virtual void DoSort(ArrayW<GlobalNamespace::BeatmapLevel*>& levels, bool ascending) const override;
            int operator ()(GlobalNamespace::BeatmapLevel* lhs, GlobalNamespace::BeatmapLevel* rhs) const;
        private:
            CompareFunc sortValueGetter;
    };

    class ComparableFunctionSorterWithLegend : public ComparableFunctionSorter, public ISorterWithLegend {
        public:
            using LegendGetterFunc = std::function<std::string(GlobalNamespace::BeatmapLevel*)>;
            ComparableFunctionSorterWithLegend(const ComparableFunctionSorter::CompareFunc& sortFunc, const LegendGetterFunc& legendFunc);
            virtual ISorterWithLegend::Legend BuildLegend(ArrayW<GlobalNamespace::BeatmapLevel*> levels) const override;
        private:
            LegendGetterFunc legendValueGetter;
    };
}