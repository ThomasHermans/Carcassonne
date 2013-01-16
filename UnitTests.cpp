#include <test_o_matic.hpp>
#include <iostream>

namespace tom
{
using namespace test_o_matic;

int
mainTests()
{
    const bool verbose = true;
    tom::simple_logger lgr(std::cout, verbose);
    tom::runner rnr;

    for (const tom::test *t = tom::first_test(); t; t = t->next)
        tom::run_test(*t, lgr, rnr);

    return lgr.summary(std::cout);
}
} // end of namespace tom
