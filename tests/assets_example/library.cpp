#include "library.h"

#include "parser_tracks.h"
#include "lego_part.h"
#include "lego_track.h"

namespace example {

    void registerParser(drawables::Parser &parser) {
        parser.registerNodeParser(LegoPart::type, parser::parseLegoPart);
        parser.registerNodeParser(LegoTrack::type, parser::parseLegoTrack);
    }

}
