//
// Created by Vladyslav Yazykov on 2019-01-14.
//

#include "logic.h"
#include "util/code_organizers.h"
#include "util/math.h"

#define arg_is(argument) strcmp(argv[current_arg], "--" #argument) == 0
#define require_arg(condition) require(condition, "Too many arguments given.")


/// Application entry point
int main(int argc, char **argv) {

    require(argc > 1, "Please provide program with arguments. See ./Bezier --help for details.");

    int current_arg{1};

    //region --help
    if (arg_is(help)) {
        require_arg(argc == 2);

        Log("Displaying info for Bezier");

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
        OUT << "--random <count>" << ENDL;
        OUT << "This generates <count> amount of curves and calculates the total number of intersections.\n"
               "<count> parameter is optional, if not provided, <count> is assumed 50." << ENDL;

        NEWL;

        return 0;
    }
    //endregion

    //region --random
    if (arg_is(random)) {
        require_arg(argc <= 3);

        ++current_arg;

        int count{DEFAULT_CURVES_COUNT};
        if (argc >= 3) // Custom count
        {
            try {
                count = std::stoi(argv[2]);
            } catch (const std::invalid_argument &) {
                error("Specified curves count is not a valid integer. See --help for details.");
            }
        }

        get_intersections(random_curves(static_cast<size_t>(count)));

        pthread_exit(nullptr);
    }
    //endregion

    //region --test
    if (arg_is(test)) {
        require_arg(argc == 2);
        Log("Comparing custom curves.");

        OUT << "Enter two curves to see if they intersect." << ENDL;
        OUT << "Please use the following syntax for points: (x1, y1) (x2, y2) ... " << ENDL;
        NEWL;
        OUT << "Enter first curve\n";
        auto curve1{get_curve()};

        NEWL;
        OUT << "Enter second curve\n";
        auto curve2{get_curve()};

        get_intersections({curve1, curve2});

        pthread_exit(nullptr);
//        return 0;
    }
    //endregion

    error("Unknown arguments format. Please see --help for info.");
}