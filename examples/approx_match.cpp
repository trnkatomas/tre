#include <iostream>
#include <tre/tre.h>

int main() {
    std::cerr << "Tre version: " << tre_version() << "\n";

    regex_t* pattern = new regex_t;
    auto compiled = tre_regcomp(pattern, "Don(ald)?( Ervin)? Knuth\0", REG_EXTENDED);

    std::cerr << "Compilation status: " << compiled << "\n";
    regamatch_t* match = new regamatch_t;
    auto number_of_groups = pattern->re_nsub;
    match->pmatch = new regmatch_t[number_of_groups + 1];
    match->nmatch = number_of_groups + 1;
    // typedef struct {
    //    int cost_ins;	       /* Default cost of an inserted character. */
    //    int cost_del;	       /* Default cost of a deleted character. */
    //    int cost_subst;          /* Default cost of a substituted character. */
    //    int max_cost;	       /* Maximum allowed cost of a match. */

    //    int max_ins;	       /* Maximum allowed number of inserts. */
    //    int max_del;	       /* Maximum allowed number of deletes. */
    //    int max_subst;           /* Maximum allowed number of substitutes. */
    //    int max_err;	       /* Maximum allowed number of errors total. */
    // } regaparams_t;*/
    regaparams_t parameters = regaparams_t{1, 1, 1, 1, 1, 1, 2, 10};
    tre_regaparams_default(&parameters);
    std::cerr << "default parameters were set\n";

    std::cerr << parameters.cost_del << " " << parameters.max_err << "\n";

    int approx = tre_have_approx(pattern);
    std::cerr << "is it approximate? " << approx << "\n";

    //tre_regaexec(const regex_t *preg, const char *string, regamatch_t *match, regaparams_t params, int eflags);
    std::cerr << "Pattern: re_sub " << pattern->re_nsub << " " << pattern->value << "\n";

    std::string haystack("In addition to fundamental contributions in several branches of \
theoretical computer science, Donnald Erwin Kuth is the creator of \
the TeX computer typesetting system, the related METAFONT font \
definition language and rendering system, and the Computer Modern \
family of typefaces.");
    int matched = tre_regaexec(pattern, haystack.c_str(), match, parameters, TRE_APPROX);

    std::cerr << "Did it match?: " << matched << "\n";

    if (matched == REG_OK) {
        std::cerr << "c: " << match->cost << ", d: " << match->num_del << ", i:" << match->num_ins << ", s: " << match->num_subst << "\n";
        for (size_t i = 0; i < match->nmatch; i++) {
            auto [ start_m, end_m ] = std::make_tuple(match->pmatch[i].rm_so, match->pmatch[i].rm_eo);
            std::cerr << "match " << i << ": (" << start_m << "," << end_m << ")\n";
            std::cerr << "\t" << haystack.substr(start_m, end_m - start_m + 1) << "\n";
        }
    }
}
