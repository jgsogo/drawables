#pragma once

#include "parser/parser.h"
#include "lego_part.h"
#include "lego_track.h"
#include "parser_tracks.h"

namespace example {

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    void registerParser(drawables::Parser<Origin, TDrawList, PixelsSymbol>& parser) {
        parser.registerNodeParser(LegoPart<Origin, TDrawList, PixelsSymbol>::type, parser::parseLegoPart<Origin, TDrawList, PixelsSymbol>);
        parser.registerNodeParser(LegoTrack<Origin, TDrawList, PixelsSymbol>::type, parser::parseLegoTrack<Origin, TDrawList, PixelsSymbol>);
    }

}
