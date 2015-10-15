//
//  Author: Vlad Seryakov vseryakov@gmail.com
//  October 2014

#include "bklib.h"

static uint32_t _crc32[256] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
    0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
    0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
    0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
    0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
    0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
    0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
    0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
    0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
    0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
    0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
    0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
    0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
    0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
    0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
    0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
    0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
    0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
    0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
    0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
    0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
    0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

// Characters that can be part of a token.  Any character whose value is greater than 0x80 (any UTF character) can be part of a token.
static const char _alphabet[] = {
     /* x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 2x */
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, /* 3x */
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 4x */
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, /* 5x */
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 6x */
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, /* 7x */
};
#define isDelim(ch) (((ch) & 0x80) == 0 && ((ch) < 0x20 || !_alphabet[(ch) - 0x20]))

static int _log(Log_Notice);

bool bkLogCheck(int level)
{
    return _log >= level;
}

int bkLogGet(void)
{
    return _log;
}

int bkLogSet(int level)
{
    return _log = level;
}

int bkLogSet(const char *level)
{
    return bkLogSet(bkLogFromString(level));
}

const char *bkLogToString(int level)
{
    return level == Log_Error ? "ERROR" : level == Log_Warn ? "WARN" : level == Log_Notice ? "NOTICE" :
           level == Log_Info ? "INFO" : level == Log_Debug ? "DEBUG" : level == Log_Dev ? "DEV" :
           level == Log_Test ? "TEST" : "NONE";
}

int bkLogFromString(const char *str)
{
    if (!str) return 0;

    if (isdigit(str[0])) {
        int i = atoi(str);
        return i >= Log_None && i <= Log_Test ? i : -1;
    }

    if (!strcasecmp(str, "ERROR")) return Log_Error;
    if (!strcasecmp(str, "WARN")) return Log_Warn;
    if (!strcasecmp(str, "NOTICE")) return Log_Notice;
    if (!strcasecmp(str, "INFO")) return Log_Info;
    if (!strcasecmp(str, "DEBUG")) return Log_Debug;
    if (!strcasecmp(str, "DEV")) return Log_Dev;
    if (!strcasecmp(str, "TEST")) return Log_Test;
    return -1;
}

void bkLogPrint(int level, const char *prefix, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    bkLogVPrint(level, prefix, fmt, ap);
    va_end(ap);
}

void bkLogVPrint(int level, const char *prefix, const char *fmt, va_list ap)
{
    if (!bkLogCheck(level)) return;
    struct tm ltm;
    struct timeval tv;
    char tbuf[64];
    FILE *fp = level >= Log_Warn ? stderr : stdout;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &ltm);
    strftime(tbuf, 64, "%Y-%m-%d %H:%M:%S", &ltm);
    fprintf(fp, "[%s.%ld][%d.%p][%s] %s: ", tbuf, (long int)tv.tv_usec/1000, getpid(), (void*)pthread_self(), prefix ? prefix : "N/A", bkLogToString(level));
    vfprintf(fp, fmt, ap);
    fprintf(fp, "\n");
}

void bkLibInit()
{
}

enum CW_MODE { CW_SUM, CW_SUMV, CW_MIN, CW_MINV, CW_MAX, CW_MAXV, CW_AVG, CW_AVGV, CW_MUL, CW_MULV, CW_ANY, CW_ANYV };
const static char *_cwModes[] = { "SUM", "SUMV", "MIN", "MINV", "MAX", "MAXV", "AVG", "AVGV", "MUL", "MULV", "ANY", "ANYV", NULL };

bkAhoCorasick::bkAhoCorasick(string id): name(id), count(0), value(0), mode(0), state(0), matches(NULL), failures(NULL), gotos(NULL)
{
    memset(alphabet, 0, sizeof(alphabet));
    memcpy(&alphabet[32], _alphabet, sizeof(_alphabet));
}

bkAhoCorasick::~bkAhoCorasick()
{
    reset();
}

void bkAhoCorasick::reset()
{
    if (matches) free(matches);
    if (failures) free(failures);
    if (gotos) free(gotos);
    matches = failures = gotos = NULL;
    list.clear();
    counters.clear();
    count = value = mode = 0;
}

void bkAhoCorasick::setAlphabet(const char *symbols, int size, bool delimiter)
{
    if (!symbols) return;
    for (int i = 0; i < size; i++) {
        alphabet[(int)symbols[i]] = !delimiter;
    }
}

void bkAhoCorasick::prepare()
{
    uint wsize = list.size(), msize = 1;
    for (uint i = 0; i < wsize; i++) msize += list[i].word.size();

    matches = (int*)calloc(1, msize * sizeof(int));
    failures = (int*)malloc(msize * sizeof(int));
    gotos = (int*)malloc(msize * 256 * sizeof(int));

    memset(matches, 0, msize * sizeof(int));
    memset(failures, -1, msize * sizeof(int));
    memset(gotos, -1, msize * 256 * sizeof(int));

    int state = 1;
    for (uint i = 0; i < wsize; ++i) {
        const unsigned char *kword = (const unsigned char*)list[i].word.c_str();
        uint r = 0, ksize = list[i].word.size();
        for (uint j = 0; j < ksize; ++j) {
            int c = tolower(kword[j]);
            // Allocate a new node
            if (gotos[r*256+c] == -1) gotos[r*256+c] = state++;
            r = gotos[r*256+c];
        }
        matches[r] |= (1 << i); // There's a match of words[i] at node state;
    }
    // State 0 should have an outgoing edge for all characters.
    for (int c = 0; c < 256; ++c) {
        if (gotos[0*256+c] == -1) gotos[0*256+c] = 0;
    }

    // Now, let's build the failure function
    queued = queue<int>();
    for (int c = 0; c < 256; ++c) {
        // All nodes s of depth 1 have failures[s] = 0
        if (gotos[0*256+c] != -1 and gotos[0*256+c] != 0) {
            failures[gotos[0*256+c]] = 0;
            queued.push(gotos[0*256+c]);
        }
    }
    while (queued.size()) {
        int r = queued.front();
        queued.pop();
        for (int c = 0; c < 256; ++c) {
            if (gotos[r*256+c] != -1) {
                int failure = failures[r];
                while (gotos[failure*256+c] == -1) failure = failures[failure];
                failure = gotos[failure*256+c];
                failures[gotos[r*256+c]] = failure;
                matches[gotos[r*256+c]] |= matches[failure]; // Merge out values
                queued.push(gotos[r*256+c]);
            }
        }
    }
}

int bkAhoCorasick::search(const char *text, int tsize)
{
    count = value = 0;
    counters.clear();
    counters.resize(list.size(), 0);

    if (!text || !tsize || !list.size() || !*text) return count;
    if (!matches) prepare();

    state = 0;
    uint wsize = list.size();
    for (int i = 0; i < tsize; ++i) {
        int r = state;
        int c = tolower((unsigned char)text[i]);
        while (gotos[r*256+c] == -1) r = failures[r];
        state = gotos[r*256+c];
        if (matches[state] == 0) continue;
        for (uint j = 0; j < wsize; ++j) {
            if (matches[state] & (1 << j) && list[j].word.size()) {
                // Must be a word separated by delimiters
                int pos = i - list[j].word.size() + 1;
                if ((pos == 0 || !alphabet[(int)text[pos-1]]) &&
                    (i + 1 == tsize || !alphabet[(int)text[i + 1]]) &&
                    !strncasecmp(&text[pos], list[j].word.c_str(), list[j].word.size())) {
                    count++;
                    counters[j]++;
                }
            }
        }
    }
    // Combine according to the function
    if (!count) return 0;
    if (mode == CW_MIN || mode == CW_MINV) value = INT_MAX;
    if (mode == CW_MAX || mode == CW_MAX) value = INT_MIN;
    for (uint i = 0; i < counters.size(); i++) {
        if (!counters[i]) continue;
        int v = list[i].value * counters[i];
        switch (mode) {
        case CW_ANY:
            if (!value) value = v, i = counters.size();
            break;

        case CW_ANYV:
            if (!value) value = list[i].value, i = counters.size();
            break;

        case CW_MIN:
            if (v < value) value = v;
            break;
        case CW_MINV:
            if (list[i].value < value) value = list[i].value;
            break;
        case CW_MAX:
            if (v > value) value = v;
            break;
        case CW_MAXV:
            if (list[i].value > value) value = list[i].value;
            break;
        case CW_MUL:
            value *= v;
            break;
        case CW_MULV:
            value *= list[i].value;
            break;
        case CW_AVGV:
        case CW_SUMV:
            value += list[i].value;
            break;
        case CW_AVG:
        case CW_SUM:
        default:
            value += v;
        }
    }
    if (mode == CW_ANY && !value && list[0].value) value = list[0].value*count;
    if (mode == CW_ANYV && !value && list[0].value) value = list[0].value;
    if (count && (mode == CW_AVG || mode == CW_AVGV)) value /= count;
    return count;
}

bool bkAhoCorasick::addJson(const char *text, int tsize, string *errmsg)
{
    bool rc = false;
    bkJsonValue *json = NULL, *words;

    reset();
    if (text && tsize) {
        json = words = bkJsonParse(text, tsize, errmsg);
        if (json && json->type == JSON_OBJECT) {
            words = json->first;
            if (words) setMode(words->name);
        }
        if (words && words->type == JSON_ARRAY) {
            for (bkJsonValue *j = words->first; j; j = j->next) {
                if (j->type == JSON_STRING) {
                    add(j->value);
                } else
                if (j->type == JSON_INT) {
                    if (!list.empty()) list.back().value = atoi(j->value.c_str());
                }
            }
            rc = true;
        }
    }
    bkJsonFree(json);
    return rc;
}

void bkAhoCorasick::setMode(string name)
{
    if (!name.size()) return;
    for (mode = 0; _cwModes[mode]; mode++) {
        if (name == _cwModes[mode]) break;
    }
}

string bkAhoCorasick::modeName()
{
    return mode >= 0 && (uint)mode < sizeof(_cwModes)/sizeof(_cwModes[0]) - 1 ? _cwModes[mode] : "SUM";
}

int bkCountWords(const string &word, const string &text, bool all)
{
    return bkCountWords(word.c_str(), word.size(), text.c_str(), text.size(), all);
}

bool bkFindWords(const char *word, int wlen, const char *text, int tlen)
{
    return bkCountWords(word, wlen, text, tlen, false);
}

int bkCountWords(const char *word, int wlen, const char *text, int tlen, bool all)
{
    if (!word || !text || !wlen || !tlen || wlen > tlen) return 0;

    int kmpNext[tlen + 1];
    int i = 0, j = kmpNext[0] = -1, count = 0;

    // Preprocessing
    while (i < wlen) {
        while (j > -1 && word[i] != word[j]) j = kmpNext[j];
        i++;
        j++;
        if (word[i] == word[j])
            kmpNext[i] = kmpNext[j];
        else
            kmpNext[i] = j;
    }

    // Searching
    i = j = 0;
    while (j < tlen) {
        while (i > -1 && word[i] != text[j]) i = kmpNext[i];
        i++;
        j++;
        if (i >= wlen) {
            // Must be a word separated by delimiters
            if ((j - i == 0 || isDelim(text[j-i-1])) && (j == tlen || isDelim(text[j]))) {
                count++;
                // Return first match
                if (!all) return count;
            }
            i = kmpNext[i];
        }
    }
    return count;
}

uint32_t bkCrc32(const void *data, int size)
{
    uint32_t rc = 0xFFFFFFFF;
    uint8_t* ptr = (uint8_t*)data;

    for (int i = 0; i < size; i++) {
        rc = (rc >> 8) ^ _crc32[(ptr[i]) ^ (rc & 0x000000FF )];
    }
    return ~rc;
}

uint32_t bkHash(const uint8_t *buf, int size)
{
#undef get16bits
#if defined(__GNUC__) && defined(__i386__)
#define get16bits(d) (*((const uint16_t *) (d)))
#else
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)+(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

    if (!buf || size <= 0) return 0;
    uint32_t hash = 0, tmp;
    int rem = size & 3;
    size >>= 2;

    for (;size > 0; size--) {
        hash  += get16bits(buf);
        tmp = (get16bits(buf + 2) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        buf += 2*sizeof(uint16_t);
        hash += hash >> 11;
    }
    switch (rem) {
    case 3:
        hash += get16bits(buf);
        hash ^= hash << 16;
        hash ^= buf[sizeof (uint16_t)] << 18;
        hash += hash >> 11;
        break;
    case 2:
        hash += get16bits(buf);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1:
        hash += *buf;
        hash ^= hash << 10;
        hash += hash >> 1;
        break;
    }
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

uint32_t bkHash2(const uint8_t *buf, int size, uint32_t seed)
{
    if (!buf || size <= 0) return 0;
    const int nblocks = size / 4;
    uint32_t h1 = seed;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    const uint32_t * blocks = (const uint32_t *)(buf + nblocks*4);
    for (int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;
        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> (32 - 13));
        h1 = h1*5+0xe6546b64;
    }
    const uint8_t *tail = (const uint8_t*)(buf + nblocks*4);
    uint32_t k1 = 0;
    switch(size & 3) {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
            k1 *= c1;
            k1 = (k1 << 15) | (k1 >> (32 - 15));
            k1 *= c2;
            h1 ^= k1;
    }
    h1 ^= size;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
}

long long bkClock()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)((long long)(tv.tv_sec)*1000 + (tv.tv_usec/1000));
}

string bkFmtTime(string fmt, int64_t sec)
{
    char buf[256];
    time_t now = sec;
#ifdef _UNIX
    struct tm tmbuf;
    strftime(buf, sizeof(buf), fmt.c_str(), localtime_r((time_t*)&now, &tmbuf));
#else
    strftime(buf, sizeof(buf), fmt.c_str(), localtime((time_t*)&now));
#endif
    return buf;
}

string bkFmtTime3339(int64_t msec)
{
    char buf[128];
    time_t sec = msec / 1000;
    msec = msec % 1000;

    struct tm tmbuf;
    struct tm *tm = localtime_r((time_t*)&sec, &tmbuf);
    snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d.%03d%c%02d:%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec,
             (int)msec,
             tm->tm_gmtoff < 0 ? '-' : '+',
             (int)(tm->tm_gmtoff / 3600), (int)((tm->tm_gmtoff % 3600) / 60));
    return buf;
}

string bkFmtStr(string fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    string str = bkFmtStrV(fmt, ap);
    va_end(ap);
    return str;
}

string bkFmtStrV(string fmt, va_list ap)
{
    string str;
    char *buf = NULL;
    int n = vasprintf(&buf, fmt.c_str(), ap);
    if (n > 0) {
        str = buf;
        free(buf);
    }
    return str;
}

string bkStrFmt(string &str, string fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    bkStrFmtV(str, fmt, ap);
    va_end(ap);
    return str;
}

string bkStrFmtV(string &str, string fmt, va_list ap)
{
    char *buf = NULL;
    int n = vasprintf(&buf, fmt.c_str(), ap);
    if (n > 0) {
        str = buf;
        free(buf);
    }
    va_end(ap);
    return str;
}

void bkSetFileTime(string file, int64_t t)
{
    struct utimbuf tbuf;
    tbuf.actime = tbuf.modtime = t;
    if (utime(file.c_str(), &tbuf)) {
        LogError("file=%s, mtime=%lld, rc=%s", file.c_str(), t, strerror(errno));
    }
}

bool bkMakePath(string path)
{
    string dir;
    vector<string> list = bkStrSplit(path, "/");
    for (uint i = 0; i < list.size() - 1; i++) {
        dir += list[i] + '/';
        if (mkdir(dir.c_str(), 0755)) {
            if (errno != EEXIST) {
                LogError("%s: %s", dir.c_str(), strerror(errno));
                return 0;
            }
        }
    }
    return 1;
}

bool bkWriteFile(const string file, const string data, int perms)
{
    if (file.empty()) {
        return false;
    }

    int fd = ::open(file.c_str(), O_CREAT|O_WRONLY, 0644);
    if (fd > 0) {
        uint rc = ::write(fd, data.data(), data.size());
        ::close(fd);
        if (rc != data.size()) return false;

        if (perms) ::chmod(file.c_str(), (mode_t)perms);
        return true;
    }
    return false;
}


vector<string> bkShuffleList(const vector<string> list)
{
    vector<string> rc;

    rc = list;
    for (uint i = 0; i < rc.size(); i++) {
        uint j = (int) ((rc.size() - 1) * (rand() / (RAND_MAX + 1.0)));
        if (i == j) {
            continue;
        }
        string str = rc[j];
        rc[j] = rc[i];
        rc[i] = str;
    }
    return rc;
}

bool bkStrNumeric(string str)
{
    bool rc = 1;
    for (uint i = 0; rc && i < str.size(); i++) if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-') rc = 0;
    return rc;
}

// Returns new string
string bkStrReplace(const string value, const string search, const string replace)
{
    string::size_type next = 0, prev = 0;
    string rc;

    while ((next = value.find(search, next)) != string::npos) {
        rc += value.substr(prev, next - prev);
        rc += replace;
        prev = next + search.length();
        next += search.length();
    }
    rc += value.substr(prev);
    return rc;
}

string bkToString(vector<string> *list, string delim)
{
    return bkToString(*list, delim);
}

string bkToString(const vector<string> &list, string delim)
{
    string rc;
    for (unsigned n = 0; n < list.size(); ++n) rc += list[n] + (n < list.size() - 1 ? delim : "");
    return rc;
}

string bkStrToLower(const string word)
{
    string s(word);
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string bkStrTrim(const string str, const string delim)
{
    string::size_type i = str.find_first_not_of(delim, 0);
    if (i == string::npos) return "";
    string::size_type j = str.find_last_not_of(delim);
    if (j == string::npos) return str.substr(i);
    return str.substr(i, j - i + 1);
}

string bkStrRtrim(const string str, const string delim)
{
    string::size_type j = str.find_last_not_of(delim);
    if (j == string::npos) return str;
    return str.substr(0, j + 1);
}

bool bkStrContains(const vector<string> &list, const string key)
{
    vector<string>::const_iterator it = find(list.begin(), list.end(), key);
    return it != list.end();
}

bool bkStrEqual(const string &a, const string &b)
{
    unsigned int sz = a.size();
    if (b.size() != sz) return false;
    for (unsigned int i = 0; i < sz; ++i) {
        if (tolower(a[i]) != tolower(b[i])) return false;
    }
    return true;
}

vector<string> bkStrSplit(const string str, const string delim, const string quotes)
{
    vector<string> rc;
    string::size_type i = 0, j = 0, q = string::npos;
    const string::size_type len = str.length();

    while (i < len) {
        i = str.find_first_not_of(delim, i);
        if (i == string::npos) break;
        // Opening quote
        if (i && quotes.find(str[i]) != string::npos) {
            q = ++i;
            while (q < len) {
                q = str.find_first_of(quotes, q);
                // Ignore escaped quotes
                if (q == string::npos || str[q - 1] != '\\') break;
                q++;
            }
        }
        // End of the word
        j = str.find_first_of(delim, i);
        if (j == string::npos) {
            rc.push_back(str.substr(i, q != string::npos ? q - i : str.size() - i));
            break;
        } else {
            rc.push_back(str.substr(i, q != string::npos ? q - i : j - i));
        }
        i = q != string::npos ? q + 1 : j + 1;
        q = string::npos;
    }
    return rc;
}

static const char *hatoui(const char *first, const char *last, unsigned int *out)
{
    unsigned int result = 0;
    for (; first != last; ++first) {
        int digit;
        if (isdigit(*first)) {
            digit = *first - '0';
        } else
        if (*first >= 'a' && *first <= 'f') {
            digit = *first - 'a' + 10;
        } else
        if (*first >= 'A' && *first <= 'F') {
            digit = *first - 'A' + 10;
        } else {
            break;
        }
        result = 16 * result + digit;
    }
    *out = result;
    return first;
}

bkJsonValue *bkJsonParse(const char *source, int size, string *errmsg)
{
    bkJsonValue *root = 0, *top = 0;
    const char *it = source;
    unsigned int codepoint;
    bkJsonValue *object;
    string name, val;
    int i = 0, line = 0;

    while (*it && (size < 0 || size > i)) {
        switch (*it) {
        case '{':
        case '[':
            // create new value
            object = new bkJsonValue((*it == '{') ? JSON_OBJECT : JSON_ARRAY, name);
            name.clear();

            // skip open character
            ++it;
            ++i;

            // set top and root
            if (top) {
                bkJsonAppend(top, object);
            } else
            if (!root) {
                root = object;
            } else {
                if (errmsg) bkStrFmt(*errmsg, "json: Only one root allowed: %d: %d: %s", it - source, size, it);
                bkJsonFree(root);
                return 0;
            }
            top = object;
            break;

        case '}':
        case ']':
            if (!top || top->type != ((*it == '}') ? JSON_OBJECT : JSON_ARRAY)) {
                if (errmsg) bkStrFmt(*errmsg, "json: Mismatch closing brace/bracket: %d: %d: %s", it - source, size, it);
                bkJsonFree(root);
                return 0;
            }

            // skip close character
            ++it;
            ++i;

            // set top
            top = top->parent;
            break;

        case ':':
            if (!top || top->type != JSON_OBJECT) {
                if (errmsg) bkStrFmt(*errmsg, "json: Unexpected character: %d: :%s", it - source, it);
                bkJsonFree(root);
                return 0;
            }
            ++it;
            ++i;
            break;

        case ',':
            if (!top) {
                if (errmsg) bkStrFmt(*errmsg, "json: Unexpected character: %d: ,%s", it - source, it);
                bkJsonFree(root);
                return 0;
            }
            ++it;
            ++i;
            break;

        case '"':
            if (!top) {
                if (errmsg) bkStrFmt(*errmsg, "json: Unexpected character: %d: %d: %s", it - source, size, it);
                bkJsonFree(root);
                return 0;
            }

            // skip '"' character
            ++it;
            ++i;
            val.clear();
            while (*it) {
                if ((unsigned char) *it < '\x20') {
                    if (errmsg) bkStrFmt(*errmsg, "json: Control characters not allowed in strings: %d: %d: %s", it - source, size, it);
                    bkJsonFree(root);
                    return 0;
                }
                if (*it == '\\') {
                    switch (it[1]) {
                    case '"':
                        val += '"';
                        break;
                    case '\\':
                        val += '\\';
                        break;
                    case '/':
                        val += '/';
                        break;
                    case 'b':
                        val += '\b';
                        break;
                    case 'f':
                        val += '\f';
                        break;
                    case 'n':
                        val += '\n';
                        ++line;
                        break;
                    case 'r':
                        val += '\r';
                        break;
                    case 't':
                        val += '\t';
                        break;
                    case 'u':
                        if (hatoui(it + 2, it + 6, &codepoint) != it + 6) {
                            if (errmsg) bkStrFmt(*errmsg, "json: Bad unicode codepoint: %d: %d: %s", it - source, size, it);
                            bkJsonFree(root);
                            return 0;
                        }
                        if (codepoint <= 0x7F) {
                            val += (char) codepoint;
                        } else
                        if (codepoint <= 0x7FF) {
                            val += (char) (0xC0 | (codepoint >> 6));
                            val += (char) (0x80 | (codepoint & 0x3F));
                        } else
                        if (codepoint <= 0xFFFF) {
                            val += (char) (0xE0 | (codepoint >> 12));
                            val += (char) (0x80 | ((codepoint >> 6) & 0x3F));
                            val += (char) (0x80 | (codepoint & 0x3F));
                        }
                        it += 4;
                        i += 4;
                        break;

                    default:
                        if (errmsg) bkStrFmt(*errmsg, "json: Unrecognized escape sequence: %d: %d: %s", it - source, size, it);
                        bkJsonFree(root);
                        return 0;
                    }
                    it += 2;
                    i += 2;
                } else
                if (*it == '"') {
                    ++it;
                    ++i;
                    break;
                } else {
                    val += *it++;
                    ++i;
                }
            }

            if (!name.size() && top->type == JSON_OBJECT) {
                // field name in object
                name = val;
            } else {
                // new string value
                object = new bkJsonValue(JSON_STRING, name, val);
                name.clear();
                bkJsonAppend(top, object);
            }
            break;

        case 'n':
        case 't':
        case 'f':
            if (!top) {
                if (errmsg) bkStrFmt(*errmsg, "json: Unexpected character: %d: %d: %s", it - source, size, it);
                bkJsonFree(root);
                return 0;
            }

            // null
            if (it[0] == 'n' && it[1] == 'u' && it[2] == 'l' && it[3] == 'l') {
                object = new bkJsonValue(JSON_NULL, name);
                it += 4;
                i += 4;
            } else
            // true
            if (it[0] == 't' && it[1] == 'r' && it[2] == 'u' && it[3] == 'e') {
                object = new bkJsonValue(JSON_BOOL, name, "true");
                it += 4;
                i += 4;
            } else
            // false
            if (it[0] == 'f' && it[1] == 'a' && it[2] == 'l' && it[3] == 's' && it[4] == 'e') {
                object = new bkJsonValue(JSON_BOOL, name, "false");
                it += 5;
                i += 5;
            } else {
                if (errmsg) bkStrFmt(*errmsg, "json: Unknown identifier: %d: %d: %s", it - source, size, it);
                bkJsonFree(root);
                return 0;
            }

            name.clear();
            bkJsonAppend(top, object);
            break;

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (!top) {
                if (errmsg) bkStrFmt(*errmsg, "json: Unexpected character: %d: %d: %s", it - source, size, it);
                bkJsonFree(root);
                return 0;
            }
            // new number value
            object = new bkJsonValue(JSON_INT, name);
            while (*it != '\x20' && *it != '\x9' && *it != '\xD' && *it != '\xA' && *it != ',' && *it != ']' && *it != '}') {
                if (*it == '.' || *it == 'e' || *it == 'E') {
                    object->type = JSON_FLOAT;
                }
                object->value += *it++;
                ++i;
            }
            name.clear();
            bkJsonAppend(top, object);
            break;

        default:
            if (errmsg) bkStrFmt(*errmsg, "json: Unexpected character: %X, %d: %d: %s", *it, it - source, size, it);
            bkJsonFree(root);
            return 0;
        }

        // skip white space
        while (*it == '\x20' || *it == '\x9' || *it == '\xD' || *it == '\xA') {
            ++it;
            ++i;
        }
    }

    if (top) {
        if (errmsg) bkStrFmt(*errmsg, "json: Not all objects/arrays have been properly closed: %d: %d: %s", it - source, size, it);
        bkJsonFree(root);
        return 0;
    }
    return root;
}

void jsonFree(bkJsonValue *root)
{
    if (!root) return;
    switch(root->type) {
    case JSON_OBJECT:
    case JSON_ARRAY:
        for (bkJsonValue *it = root->first; it; it = it->next) jsonFree(it);

    default:
        delete root;
    }
}

static string bkJsonEscape(string value)
{
    string rc;

    for (char *c = (char *)value.c_str(); *c; c++) {
        switch (*c) {
        case '\b':
            rc += "\\b";
            break;
        case '\f':
            rc += "\\f";
            break;
        case '\n':
            rc += "\\n";
            break;
        case '\r':
            rc += "\\r";
            break;
        case '\t':
            rc += "\\t";
            break;
        case '"':
            rc += "\\\"";
            break;
        case '\\':
            rc += "\\\\";
            break;
        default:
            if ((unsigned char) *c < ' ')
                rc += bkFmtStr("\\u%04x", (int) *c);
            else
                rc.append(c, 1);
            break;
        }
    }
    return rc;
}

string bkJsonStringify(bkJsonValue *value, string rc)
{
    if (!value) return rc;

    if (value->name.size()) {
        if (!(value->parent && value->parent->type == JSON_ARRAY && bkStrNumeric(value->name))) {
            rc += "\"" + value->name + "\":";
        }
    }
    switch(value->type) {
    case JSON_NULL:
            rc += "null";
            break;
    case JSON_OBJECT:
    case JSON_ARRAY:
            rc += value->type == JSON_OBJECT ? "{" : "[";
            for (bkJsonValue *it = value->first; it; it = it->next) {
                rc += bkJsonStringify(it);
                if (it->next) rc += ",";
            }
            rc += value->type == JSON_OBJECT ? "}" : "]";
            break;
    case JSON_STRING:
            rc += "\"" + bkJsonEscape(value->value) + "\"";
            break;
    case JSON_INT:
    case JSON_FLOAT:
    case JSON_BOOL:
            rc += value->value;
            break;
    }
    return rc;
}

bool bkJsonAppend(bkJsonValue *root, bkJsonValue *val)
{
    if (!root || !val) return false;
    val->parent = root;
    if (root->last) {
        root->last = root->last->next = val;
    } else {
        root->first = root->last = val;
    }
    // Assign an index
    if (root->type == JSON_ARRAY && !val->name.size()) {
        val->name = bkFmtStr("%d", bkJsonLength(root) - 1);
    }
    return true;
}

int bkJsonLength(bkJsonValue *root)
{
    int n = 0;
    for (bkJsonValue *it = root ? root->first : NULL; it; it = it->next, n++);
    return n;
}

bool bkJsonDel(bkJsonValue *root, string name)
{
    if (!root) return false;
    if (root->type != JSON_OBJECT && root->type != JSON_ARRAY) return false;

    for (bkJsonValue *it = root->first, *prev = 0; it; prev = it, it = it->next) {
        if (it->name == name) {
            if (it == root->first) {
                root->first = it->next;
            } else {
                if (it == root->last) root->last = prev;
                if (prev) prev->next = it->next;
            }
            bkJsonFree(it);
            return true;
        }
    }
    return false;
}

bool bkJsonSet(bkJsonValue *root, bkJsonValue *val)
{
    if (!root || !val) return false;
    if (root->type != JSON_OBJECT && root->type != JSON_ARRAY) return false;

    bkJsonValue *old = bkJsonGet(root, val->name);
    if (!old) {
        bkJsonAppend(root, val);
    } else {
        old->value = val->value;
        jsonFree(val);
    }
    return true;
}

bool bkJsonSet(bkJsonValue *root, bkJsonType type, string name, string val)
{
    bkJsonValue *v = new bkJsonValue(type, name, val);
    if (!bkJsonSet(root, v)) {
        bkJsonFree(v);
        return false;
    }
    return true;
}

bkJsonValue *bkJsonGet(bkJsonValue *root, string name)
{
    switch(root ? root->type : 0) {
    case JSON_OBJECT:
    case JSON_ARRAY:
        for (bkJsonValue *it = root->first; it; it = it->next) {
            if (it->name == name) return it;
        }
    default:
        break;
    }
    return NULL;
}

string bkJsonGetStr(bkJsonValue *root, string name)
{
    bkJsonValue *v = bkJsonGet(root, name);
    return v ? v->value : string();
}

int64_t bkJsonGetInt(bkJsonValue *root, string name)
{
    bkJsonValue *v = bkJsonGet(root, name);
    return v ? atoll(v->value.c_str()) : 0;
}

double bkJsonGetNum(bkJsonValue *root, string name)
{
    bkJsonValue *v = bkJsonGet(root, name);
    return v ? atof(v->value.c_str()) : 0;
}

#define IDENT(n) for (int i = 0; i < n; ++i) printf("    ")

void bkJsonPrint(bkJsonValue *value, int ident)
{
    IDENT(ident);
    if (value->name.size()) printf("\"%s\" = ", value->name.c_str());
    switch(value->type) {
    case JSON_NULL:
            printf("null\n");
            break;
    case JSON_OBJECT:
    case JSON_ARRAY:
            printf(value->type == JSON_OBJECT ? "{\n" : "[\n");
            for (bkJsonValue *it = value->first; it; it = it->next) bkJsonPrint(it, ident + 1);
            IDENT(ident);
            printf(value->type == JSON_OBJECT ? "}\n" : "]\n");
            break;
    case JSON_STRING:
            printf("\"%s\"\n", value->value.c_str());
            break;
    case JSON_INT:
    case JSON_FLOAT:
    case JSON_BOOL:
            printf("%s\n", value->value.c_str());
            break;
    }
}

#define PI 3.14159265358979323846264338327950288
#define DEG2RAD(degrees) (degrees * PI / 180)
#define RAD2DEG(radians) (radians * 180 / PI)

double bkDistance(double lat1, double long1, double lat2, double long2)
{
	double lat1rad = DEG2RAD(lat1);
	double lat2rad = DEG2RAD(lat2);
	return acos(sin(lat1rad) * sin(lat2rad) + cos(lat1rad) * cos(lat2rad) * cos(DEG2RAD(long2) - DEG2RAD(long1))) * 6378.1;
}

int bkBearing(double lat1, double long1, double lat2, double long2)
{
	static double d2r = PI / 180;
	double dlong = (long2 - long1) * d2r;
	//double dlat = (lat2 - lat1) * d2r;
	double y = sin(dlong) * cos(lat2 * d2r);
	double x = cos(lat1 * d2r) * sin(lat2 * d2r) - sin(lat1 * d2r) * cos(lat2 * d2r) * cos(dlong);
	double b = atan2(y, x);
	return (int)((b * 180 / PI) + 360) % 360;
}

vector<double> bkBoundingBox(double lat, double lon, double distance)
{
	double rad_dist = distance / 6378.1;

	double MIN_LAT = DEG2RAD(-90);
	double MAX_LAT = DEG2RAD(90);
	double MIN_LON = DEG2RAD(-180);
	double MAX_LON = DEG2RAD(180);

    double min_lat = DEG2RAD(lat) - rad_dist;
    double max_lat = DEG2RAD(lat) + rad_dist;
    double min_lon, max_lon;

    if (min_lat > MIN_LAT && max_lat < MAX_LAT) {
        double delta_lon = asin(sin(rad_dist) / cos(lat));
        min_lon = DEG2RAD(lon) - delta_lon;
        if (min_lon < MIN_LON) min_lon += 2 * PI;
        max_lon = DEG2RAD(lon) + delta_lon;
        if (max_lon > MAX_LON) max_lon -= 2 * PI;
    } else {
        min_lat = MAX(min_lat, MIN_LAT);
        max_lat = MIN(max_lat, MAX_LAT);
        min_lon = MIN_LON;
        max_lon = MAX_LON;
    }
    vector<double> rc;
    rc.push_back(RAD2DEG(min_lat));
    rc.push_back(RAD2DEG(min_lon));
    rc.push_back(RAD2DEG(max_lat));
    rc.push_back(RAD2DEG(max_lon));
    return rc;
}

static int BITS[] = {16, 8, 4, 2, 1};
static string BASE32 = "0123456789bcdefghjkmnpqrstuvwxyz";

vector<double> bkGeoHashDecode(string hash)
{
    int is_even = 1;
    double lat[3] = {-90,90,0}, lon[3] = {-180,180,0}, lat_err = 90.0, lon_err = 180.0;
    vector<double> rc;

    for (uint i = 0; i< hash.size(); i++) {
        char c = hash[i];
        string::size_type cd = BASE32.find(c);
        for (int j = 0; j < 5; j++) {
            int mask = BITS[j];
            if (is_even) {
                lon_err /= 2;
                if (cd & mask) lon[0] = (lon[0] + lon[1])/2; else lon[1] = (lon[0] + lon[1])/2;
            } else {
                lat_err /= 2;
                if (cd & mask) lat[0] = (lat[0] + lat[1])/2; else lat[1] = (lat[0] + lat[1])/2;
            }
            is_even = !is_even;
        }
    }
    rc.push_back((lat[0] + lat[1])/2);
    rc.push_back((lon[0] + lon[1])/2);
    for (int i = 0; i < 2; i++) rc.push_back(lat[i]);
    for (int i = 0; i < 2; i++) rc.push_back(lon[i]);
    return rc;
}

string bkGeoHashEncode(double latitude, double longitude, uint precision)
{
	int is_even=1,bit=0,ch=0;
	double lat[2] = {-90,90}, lon[2] = {-180,180};
	string hash;
	if (precision == 0) precision = 8;
	if (precision > 12) precision = 12;

	while (hash.size() < precision) {
		if (is_even) {
			double mid = (lon[0] + lon[1]) / 2;
			if (longitude > mid) {
				ch |= BITS[bit];
				lon[0] = mid;
			} else {
				lon[1] = mid;
			}
		} else {
			double mid = (lat[0] + lat[1]) / 2;
			if (latitude > mid) {
				ch |= BITS[bit];
				lat[0] = mid;
			} else {
				lat[1] = mid;
			}
		  }

		is_even = !is_even;
		if (bit < 4) {
			bit++;
		} else {
			hash += BASE32[ch];
			bit = 0;
			ch = 0;
		}
	}
	return hash;
}

string bkGeoHashAdjacent(string hash, string dir)
{
	static map<string,string> NEIGHBORS;
	static map<string,string> BORDERS;
	static bool init = 0;
	if (!init) {
		NEIGHBORS["right.even"] = "bc01fg45238967deuvhjyznpkmstqrwx";
		NEIGHBORS["left.even"] = "238967debc01fg45kmstqrwxuvhjyznp";
		NEIGHBORS["top.even"] = "p0r21436x8zb9dcf5h7kjnmqesgutwvy";
		NEIGHBORS["bottom.even"] = "14365h7k9dcfesgujnmqp0r2twvyx8zb";
		NEIGHBORS["bottom.odd"] = NEIGHBORS["left.even"];
		NEIGHBORS["top.odd"] = NEIGHBORS["right.even"];
		NEIGHBORS["left.odd"] = NEIGHBORS["bottom.even"];
		NEIGHBORS["right.odd"] = NEIGHBORS["top.even"];

		BORDERS["right.even"] = "bcfguvyz";
		BORDERS["left.even"] = "0145hjnp";
		BORDERS["top.even"] = "prxz";
		BORDERS["bottom.even"] = "028b";
		BORDERS["bottom.odd"] = BORDERS["left.even"];
		BORDERS["top.odd"] = BORDERS["right.even"];
		BORDERS["left.odd"] = BORDERS["bottom.even"];
		BORDERS["right.odd"] = BORDERS["top.even"];
		init = 1;
	}
	if (!hash.size()) return string();
	char lastChr = hash[hash.size() - 1];
	string type = dir + (hash.size() % 2 ? ".odd" : ".even");
	string base = hash.substr(0, hash.size()-1);
	if (BORDERS[type].find(lastChr) != string::npos) base = bkGeoHashAdjacent(base, dir);
	string::size_type cd = NEIGHBORS[type].find(lastChr);
	if (cd != string::npos) base += BASE32[cd];
	return base;
}

vector<string> bkGeoHashRow(string center, int steps)
{
    vector<string> rc;
    rc.push_back(center);

    string left = center, right = center;
    for (int i = 0; i < steps; i++) {
        string next = bkGeoHashAdjacent(left, "left");
        rc.push_back(next);
        left = next;
        next = bkGeoHashAdjacent(right, "right");
        rc.push_back(next);
        right = next;
    }
    return rc;
}

vector< vector<string> > bkGeoHashGrid(string center, int steps)
{
    vector< vector<string> > rc;

    vector<string> col = bkGeoHashRow(center, steps);
    rc.push_back(col);
    string top = center, bottom = center;

    for (int i = 0; i < steps; i++) {
        string next = bkGeoHashAdjacent(top, "top");
        col = bkGeoHashRow(next, steps);
        rc.push_back(col);
        top = next;
        next = bkGeoHashAdjacent(bottom, "bottom");
        col = bkGeoHashRow(next, steps);
        rc.push_back(col);
        bottom = next;
    }
    return rc;
}
