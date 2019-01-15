//
// Created by Vladyslav Yazykov on 2019-01-14.
//

#include "logic.h"
#include "util/code_organizers.h"
#include "util/math.h"

#define arg_is(argument) strcmp(argv[1], "--" #argument) == 0

/// Application entry point
int main(int argc, char **argv) {
    require(argc < 4, "Too many arguments specified. See --help for details.");

    //region no arg
    if (argc == 1) {
        OUT << "Enter two polygons to see if they intersect." << ENDL;
        OUT << "Polygons' convex hulls are formed, and then intersection is determined." << ENDL;
        OUT << "Please use the following syntax for points: (x1, y1) (x2, y2) ... " << ENDL;
        NEWL;

        try {

            OUT << "Enter first polygon\n";
            auto p1{get_polygon()};

            NEWL;
            OUT << "Enter second polygon\n";
            auto p2{get_polygon()};

            get_intersections({p1, p2});

        } catch (const std::invalid_argument &e) {
            OUT << e.what() << ENDL;
            return 1;
        }

        pthread_exit(nullptr);
    }
    //endregion

    //region --help
    if (arg_is(help)) {
        INFO("Displaying info for the program");

        OUT << "This program counts intersections between two bezier curves." << ENDL;
        OUT << "Supported arguments:" << ENDL;

        // Help
        NEWL;
        OUT << "--help" << ENDL;
        OUT << "Displays info about program. But you probably already know what --help does..." << ENDL;

        // Test
        NEWL;
        OUT << "--test" << ENDL;
        OUT << "This allows to test the algorithm by manually inputting two custom curves "
               "and counting their intersections." << ENDL;

        // Random
        NEWL;
        OUT << "--random" << ENDL;
        OUT << "This generates " << DEFAULT_POLYGONS_COUNT
            << " random polygons. And calculates the number of intersections."
               "Runs sequentially and in parallel, recording the time.\n" << ENDL;

        NEWL;
        return 0;
    }
    //endregion

    //region --random
    if (arg_is(random)) {

        int count{DEFAULT_POLYGONS_COUNT};

        if (argc >= 3) // Custom count
        {
            try {
                count = std::stoi(argv[2]);
            } catch (const std::invalid_argument &) {
                error("Specified curves count is not a valid integer. See --help for details.");
            }
        }

        get_intersections(random_polygons(static_cast<size_t>(count)));

        pthread_exit(nullptr);
    }
    //endregion

    error("Unknown arguments format. Please see --help for info.");
}