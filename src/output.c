#include "output.h"
#include "seedfilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

const char *sf_structure_type_name(SFStructureType t) {
    switch (t) {
        case SF_TREASURE:       return "treasure";
        case SF_VILLAGE:        return "village";
        case SF_SHIPWRECK:      return "shipwreck";
        case SF_RUINED_PORTAL:  return "ruined_portal";
        case SF_MANSION:        return "mansion";
        case SF_BASTION:        return "bastion";
        case SF_FORTRESS:       return "fortress";
        case SF_END_CITY:       return "end_city";
        case SF_DESERT_PYRAMID: return "desert";
        case SF_JUNGLE_TEMPLE:  return "jungle";
        case SF_MINESHAFT:      return "mineshaft";
        case SF_IGLOO:          return "igloo";
        case SF_MONUMENT:       return "monument";
        default:                return "unknown";
    }
}

const char *sf_biome_type_name(SFBiomeType b) {
    switch (b) {
        case SF_BIOME_OCEAN:      return "ocean";
        case SF_BIOME_DEEP_OCEAN: return "deep_ocean";
        case SF_BIOME_BEACH:      return "beach";
        case SF_BIOME_PLAINS:     return "plains";
        case SF_BIOME_DESERT:     return "desert";
        default:                  return "unknown";
    }
}

/* ---- Growable string buffer internal --------------------------------- */
typedef struct {
    char *data;
    size_t len;
    size_t cap;
} SBuf;

static int sbuf_init(SBuf *b, size_t initialCap) {
    b->data = (char *)malloc(initialCap);
    if (!b->data) return 0;
    b->data[0] = '\0';
    b->len = 0;
    b->cap = initialCap;
    return 1;
}

static int sbuf_ensure(SBuf *b, size_t extra) {
    if (b->len + extra + 1 <= b->cap) return 1;
    size_t newCap = b->cap * 2;
    while (newCap < b->len + extra + 1) newCap *= 2;
    char *nd = (char *)realloc(b->data, newCap);
    if (!nd) return 0;
    b->data = nd;
    b->cap = newCap;
    return 1;
}

static int sbuf_appendf(SBuf *b, const char *fmt, ...) {
    char tmp[512];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    if (n < 0) return 0;

    if ((size_t)n < sizeof(tmp)) {
        if (!sbuf_ensure(b, (size_t)n)) return 0;
        memcpy(b->data + b->len, tmp, (size_t)n + 1);
        b->len += (size_t)n;
        return 1;
    }
    /* tmp terlalu kecil (jarang terjadi untuk data kita, tapi tetap
     * ditangani supaya tidak ada truncation diam-diam). */
    if (!sbuf_ensure(b, (size_t)n)) return 0;
    va_start(ap, fmt);
    vsnprintf(b->data + b->len, (size_t)n + 1, fmt, ap);
    va_end(ap);
    b->len += (size_t)n;
    return 1;
}

static void append_hit_json(SBuf *b, const SFStructureHit *h, int isFirst) {
    sbuf_appendf(b,
        "%s{\"type\":\"%s\",\"x\":%d,\"z\":%d,\"dx\":%d,\"dz\":%d,"
        "\"dist_manhattan\":%d,\"radius_threshold\":%d,\"passed\":%s}",
        isFirst ? "" : ",",
        sf_structure_type_name(h->type), h->x, h->z, h->dx, h->dz,
        h->distManhattan, h->radiusThreshold, h->passed ? "true" : "false");
}

static void append_result_json(SBuf *b, const SFSeedResult *r) {
    sbuf_appendf(b, "{\"seed\":%lld,\"passed\":%s,\"hits\":[",
                 (long long)r->seed, r->passed ? "true" : "false");
    for (int i = 0; i < r->hitCount; i++) {
        append_hit_json(b, &r->hits[i], i == 0);
    }
    sbuf_appendf(b, "]}");
}

char *seed_result_to_json(const SFSeedResult *result) {
    if (!result) return NULL;
    SBuf b;
    if (!sbuf_init(&b, 256 + (size_t)result->hitCount * 128)) return NULL;
    append_result_json(&b, result);
    return b.data; /* caller wajib panggil sf_free_json() */
}

char *search_output_to_json(const SFSearchOutput *output) {
    if (!output) return NULL;
    SBuf b;
    if (!sbuf_init(&b, 512 + (size_t)output->resultCount * 256)) return NULL;

    sbuf_appendf(&b, "{\"attempts_made\":%lld,\"result_count\":%d,\"results\":[",
                 (long long)output->attemptsMade, output->resultCount);
    for (int i = 0; i < output->resultCount; i++) {
        if (i > 0) sbuf_appendf(&b, ",");
        append_result_json(&b, &output->results[i]);
    }
    sbuf_appendf(&b, "]}");
    return b.data;
}

void sf_free_json(char *json) {
    free(json);
}
