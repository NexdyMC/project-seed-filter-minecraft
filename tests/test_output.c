#include "output.h"
#include "seedfilter.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void) {
    SFStructureHit hits[2];
    hits[0] = (SFStructureHit){ SF_VILLAGE, -160, 80, 160, 80, 240, 208, 1 };
    hits[1] = (SFStructureHit){ SF_TREASURE, 640, -320, 640, 320, 960, 32, 0 };

    SFSeedResult res;
    res.seed = 21;
    res.hits = hits;
    res.hitCount = 2;
    res.passed = 1;

    char *json = seed_result_to_json(&res);
    assert(json != NULL);
    printf("%s\n", json);
    assert(strstr(json, "\"seed\":21") != NULL);
    assert(strstr(json, "\"village\"") != NULL);
    assert(strstr(json, "\"passed\":true") != NULL);
    sf_free_json(json);

    SFSearchOutput out;
    out.results = &res;
    out.resultCount = 1;
    out.attemptsMade = 12345;
    char *json2 = search_output_to_json(&out);
    printf("%s\n", json2);
    assert(strstr(json2, "\"attempts_made\":12345") != NULL);
    sf_free_json(json2);

    printf("SEMUA TEST OUTPUT LULUS\n");
    return 0;
}
