//
//  Author: Vlad Seryakov vseryakov@gmail.com
//  April 2013
//

#include "bkjs.h"
#include "bklib.h"
#include "snappy.h"
#include "bkzip.h"
#include "bkunzip.h"

static const unsigned int POLL_PERIOD_MS = 500;
static unsigned int HIGH_WATER_MARK_MS = 70;
static const unsigned int AVG_DECAY_FACTOR = 3;
static uv_timer_t _busyTimer;
static uint32_t _currentLag;
static uint64_t _lastMark;

// Based on the https://github.com/lloyd/node-toobusy
static void busy_timer(uv_timer_t* handle)
{
    uint64_t now = uv_hrtime();

    if (_lastMark > 0) {
        // keep track of (dampened) average lag.
        uint32_t lag = (uint32_t) ((now - _lastMark) / 1000000);
        lag = (lag < POLL_PERIOD_MS) ? 0 : lag - POLL_PERIOD_MS;
        _currentLag = (lag + (_currentLag * (AVG_DECAY_FACTOR-1))) / AVG_DECAY_FACTOR;
    }
    _lastMark = now;
}

static NAN_METHOD(initBusy)
{
    NAN_OPTIONAL_ARGUMENT_INT(0, ms, 0);

    if (ms > 10) HIGH_WATER_MARK_MS = ms;

    if ((long long)_busyTimer.data != 1970) {
        uv_timer_init(uv_default_loop(), &_busyTimer);
        uv_timer_start(&_busyTimer, (uv_timer_cb)busy_timer, POLL_PERIOD_MS, POLL_PERIOD_MS);
        _busyTimer.data = (void*)1970;
    }
    NAN_RETURN(HIGH_WATER_MARK_MS);
}

static NAN_METHOD(isBusy)
{
    if (_currentLag > HIGH_WATER_MARK_MS) {
        // probabilistically block requests proportional to how far behind we are.
        double pctToBlock = ((_currentLag - HIGH_WATER_MARK_MS) / (double) HIGH_WATER_MARK_MS) * 100.0;
        double r = (rand() / (double) RAND_MAX) * 100.0;
        if (r < pctToBlock) NAN_RETURN(Nan::True());
    }
    NAN_RETURN(Nan::False());
}

static NAN_METHOD(getBusy)
{
    NAN_RETURN(_currentLag);
}

static NAN_METHOD(getUser)
{
   struct passwd *pw = NULL;

   if (info.Length() > 0) {
       Nan::Utf8String name(Nan::To<v8::String>(info[0]).ToLocalChecked());
       pw = getpwnam(*name);
       if (!pw && bkStrNumeric(*name)) pw = getpwuid(Nan::To<int32_t>(info[0]).FromJust());
   } else {
       pw = getpwnam(getlogin());
   }
   Local<Object> obj = Nan::New<Object>();
   if (pw) {
       Nan::Set(obj, Nan::New("uid").ToLocalChecked(), Nan::New(pw->pw_uid));
       Nan::Set(obj, Nan::New("gid").ToLocalChecked(), Nan::New(pw->pw_gid));
       Nan::Set(obj, Nan::New("name").ToLocalChecked(), Nan::New(pw->pw_name).ToLocalChecked());
       Nan::Set(obj, Nan::New("dir").ToLocalChecked(), Nan::New(pw->pw_dir).ToLocalChecked());
   }
   NAN_RETURN(obj);
}

static NAN_METHOD(getGroup)
{
   struct group *g = NULL;

   if (info.Length() > 0) {
       Nan::Utf8String name(Nan::To<v8::String>(info[0]).ToLocalChecked());
       g = getgrnam(*name);
       if (!g && bkStrNumeric(*name)) g = getgrgid(Nan::To<int32_t>(info[0]).FromJust());
   } else {
       struct passwd *pw = getpwnam(getlogin());
       g = getgrgid(pw ? pw->pw_gid : 0);
   }
   Local<Object> obj = Nan::New<Object>();
   if (g) {
       Nan::Set(obj, Nan::New("gid").ToLocalChecked(), Nan::New(g->gr_gid));
       Nan::Set(obj, Nan::New("name").ToLocalChecked(), Nan::New(g->gr_name).ToLocalChecked());
   }
   NAN_RETURN(obj);
}

static vector<bkAhoCorasick*> _wc;

static NAN_METHOD(countWordsInit)
{
    for (uint i = 0; i < _wc.size(); i++) delete _wc[i];
    _wc.clear();
}

static NAN_METHOD(countWords)
{
    NAN_REQUIRE_ARGUMENT_AS_STRING(0, word);
    NAN_REQUIRE_ARGUMENT_AS_STRING(1, text);

    NAN_RETURN(Nan::New(bkCountWords(*word, *text)));
}

static NAN_METHOD(countAllWords)
{
    NAN_REQUIRE_ARGUMENT_ARRAY(0, list);
    NAN_REQUIRE_ARGUMENT_AS_STRING(1, text);

    bkAhoCorasick w, *cw = &w;

    // Find cached class
    if (info.Length() > 2 && !info[2]->IsNull()) {
        cw = NULL;
        Nan::Utf8String hash(info[2]);
        for (uint i = 0; i < _wc.size() && !cw; i++) {
            if (_wc[i]->name == *hash) cw = _wc[i];
        }
        if (!cw) {
            cw = new bkAhoCorasick(*hash);
            _wc.push_back(cw);
        }
    }

    // Additional delimiters
    if (info.Length() > 3 && !info[3]->IsNull()) {
        Nan::Utf8String str(info[3]);
        cw->setAlphabet(*str, str.length(), true);
    }
    // Additional non-delimiters
    if (info.Length() > 4 && !info[4]->IsNull()) {
        Nan::Utf8String str(info[4]);
        cw->setAlphabet(*str, str.length(), false);
    }

    // Op mode
    if (info.Length() > 5 && !info[5]->IsNull()) {
        Nan::Utf8String str(info[5]);
        cw->setMode(*str);
    }

    if (!cw->list.size()) {
        for (uint i = 0; i < list->Length(); i++) {
            Local<Value> val = Nan::Get(list, i).ToLocalChecked();
            if (val->IsString()) {
                Nan::Utf8String str(val);
                cw->add(*str);
            } else
            if (val->IsInt32()) {
                if (!cw->list.empty()) cw->list.back().value = Nan::To<int32_t>(val).FromJust();
            }
        }
    }

    cw->search(*text);
    list = Nan::New<Array>();
    Local<Array> counters = Nan::New<Array>();
    Local<Array> values = Nan::New<Array>();
    for (uint i = 0, j = 0; i < cw->counters.size(); i++) {
        if (cw->counters[i]) {
            string w = cw->list[i].word;
            if (cw->list[i].value) w += bkFmtStr("/%d", cw->list[i].value);
            Nan::Set(list, Nan::New(j), Nan::New(w.c_str()).ToLocalChecked());
            Nan::Set(counters, Nan::New(j), Nan::New(cw->counters[i]));
            Nan::Set(values, Nan::New(j++), Nan::New(cw->list[i].value));
        }
    }
    Local<Object> obj = Nan::New<Object>();
    Nan::Set(obj, Nan::New("count").ToLocalChecked(), Nan::New(cw->count));
    Nan::Set(obj, Nan::New("value").ToLocalChecked(), Nan::New(cw->value));
    Nan::Set(obj, Nan::New("mode").ToLocalChecked(), Nan::New(cw->modeName().c_str()).ToLocalChecked());
    Nan::Set(obj, Nan::New("matches").ToLocalChecked(), list);
    Nan::Set(obj, Nan::New("counters").ToLocalChecked(), counters);
    Nan::Set(obj, Nan::New("values").ToLocalChecked(), values);

    NAN_RETURN(obj);
}

static NAN_METHOD(geoHashEncode)
{
   NAN_REQUIRE_ARGUMENT_NUMBER(0, lat);
   NAN_REQUIRE_ARGUMENT_NUMBER(1, lon);
   NAN_OPTIONAL_ARGUMENT_INT(2, len, 0);

   string hash = bkGeoHashEncode(lat, lon, len);
   NAN_RETURN(Nan::New(hash).ToLocalChecked());
}

static NAN_METHOD(geoHashDecode)
{
   NAN_REQUIRE_ARGUMENT_AS_STRING(0, hash);

   vector<double> rc = bkGeoHashDecode(*hash);
   Local<Array> result = Nan::New<Array>(rc.size());
   for (uint i = 0; i < rc.size(); i++) {
       Nan::Set(result, Nan::New(i), Nan::New(rc[i]));
   }
   NAN_RETURN(result);
}

static NAN_METHOD(geoHashAdjacent)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, base);
   NAN_REQUIRE_ARGUMENT_STRING(1, dir);

   string hash = bkGeoHashAdjacent(*base, *dir);
   NAN_RETURN(Nan::New(hash).ToLocalChecked());
}

static bool isNumber(Local<Value> arg)
{
    Nan::HandleScope scope;
    Nan::Utf8String str(arg);
    const char *p = *str;
    while (*p && (*p == ' ' || *p == '-' || *p == '+')) p++;
    if (!isdigit(*p)) return false;
    return true;
}

static NAN_METHOD(geoDistance)
{
   if (info.Length() < 4) return;
   double lat1 = Nan::To<double>(info[0]).FromJust();
   double lon1 = Nan::To<double>(info[1]).FromJust();
   double lat2 = Nan::To<double>(info[2]).FromJust();
   double lon2 = Nan::To<double>(info[3]).FromJust();
   if (::isnan(lat1) || ::isnan(lon1) || ::isnan(lat2) || ::isnan(lon2)) return;
   if (lat1 == 0 && !isNumber(Nan::To<String>(info[0]).ToLocalChecked())) return;
   if (lon1 == 0 && !isNumber(Nan::To<String>(info[1]).ToLocalChecked())) return;
   if (lat2 == 0 && !isNumber(Nan::To<String>(info[2]).ToLocalChecked())) return;
   if (lon2 == 0 && !isNumber(Nan::To<String>(info[3]).ToLocalChecked())) return;

   info.GetReturnValue().Set(Nan::New(bkDistance(lat1, lon1, lat2, lon2)));
}

static NAN_METHOD(geoBoundingBox)
{
   NAN_REQUIRE_ARGUMENT_NUMBER(0, lat1);
   NAN_REQUIRE_ARGUMENT_NUMBER(1, lon1);
   NAN_REQUIRE_ARGUMENT_NUMBER(2, distance);

   vector<double> rc = bkBoundingBox(lat1, lon1, distance);
   Local<Array> result = Nan::New<Array>(rc.size());
   for (uint i = 0; i < rc.size(); i++) {
       Nan::Set(result, Nan::New(i), Nan::New(rc[i]));
   }
   NAN_RETURN(result);
}

static NAN_METHOD(geoHashGrid)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, base);
   NAN_OPTIONAL_ARGUMENT_NUMBER(1, steps, 0);
   if (steps <= 0) steps = 1;

   vector< vector<string> > rc = bkGeoHashGrid(*base, steps);
   Local<Array> result = Nan::New<Array>();
   for (uint j = 0, n = 0; j < rc[0].size(); j++) {
       for (uint i = 0; i < rc.size(); i++) {
           Nan::Set(result, Nan::New(n++), Nan::New(rc[i][j]).ToLocalChecked());
       }
   }
   NAN_RETURN(result);
}

static NAN_METHOD(geoHashRow)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, base);
   NAN_OPTIONAL_ARGUMENT_NUMBER(1, steps, 0);
   if (steps <= 0) steps = 1;

   vector<string> rc = bkGeoHashRow(*base, steps);
   Local<Array> result = Nan::New<Array>(rc.size());
   for (uint i = 0; i < rc.size(); i++) {
       Nan::Set(result, Nan::New(i), Nan::New(rc[i]).ToLocalChecked());
   }
   NAN_RETURN(result);
}

static NAN_METHOD(snappyCompress)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, str);

   string out;
   snappy::Compress(*str, str.length(), &out);
   NAN_RETURN(Nan::New(out.c_str(), out.size()).ToLocalChecked());
}

static NAN_METHOD(snappyUncompress)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, str);

   string out;
   snappy::Uncompress(*str, str.length(), &out);
   NAN_RETURN(Nan::New(out.c_str(), out.size()).ToLocalChecked());
}

static NAN_METHOD(zlibCompress)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, str);
   NAN_OPTIONAL_ARGUMENT_INT(1, level, 0);

   string out;
   z_stream strm;
   bkDeflateInit(&strm, level ? level : Z_BEST_SPEED);
   bkDeflate(&strm, *str, str.length(), &out);
   bkDeflateEnd(&strm, &out);
   NAN_RETURN(Nan::New(out.c_str(), out.size()).ToLocalChecked());
}

static NAN_METHOD(zlibUncompress)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, str);

   string out;
   z_stream strm;
   bkInflateInit(&strm);
   bkInflate(&strm, *str, str.length(), &out);
   bkInflateEnd(&strm);
   NAN_RETURN(Nan::New(out.c_str(), out.size()).ToLocalChecked());
}

static NAN_METHOD(unzipFile)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, zip);
   NAN_REQUIRE_ARGUMENT_STRING(1, file);
   NAN_OPTIONAL_ARGUMENT_STRING(2, outfile);

   if (info.Length() == 3) {
       int rc = bkUnzip::unzip(*zip, *file, *outfile);
       NAN_RETURN(Nan::New(rc));
   }

   string out = bkUnzip::toString(*zip, *file);
   NAN_RETURN(Nan::New(out.c_str(), out.size()).ToLocalChecked());
}

static NAN_METHOD(unzip)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, zip);
   NAN_REQUIRE_ARGUMENT_STRING(1, dir);

   int rc = bkUnzip::unzip(*zip, *dir);
   NAN_RETURN(Nan::New(rc));
}

static NAN_METHOD(zipFiles)
{
   NAN_REQUIRE_ARGUMENT_STRING(0, zip);

   vector<pair<string,int> > list = bkUnzip::get_files(*zip);
   Local<Array> result = Nan::New<Array>(list.size());
   for (uint i = 0; i < list.size(); i++) {
        Local<Object> obj = Nan::New<Object>();
        Nan::Set(obj, Nan::New("file").ToLocalChecked(), Nan::New(list[i].first.c_str()).ToLocalChecked());
        Nan::Set(obj, Nan::New("size").ToLocalChecked(), Nan::New(list[i].second));
        Nan::Set(result, Nan::New(i), obj);
   }
   NAN_RETURN(result);
}

static NAN_METHOD(strSplit)
{
   NAN_OPTIONAL_ARGUMENT_STRING(0, str);
   NAN_OPTIONAL_ARGUMENT_STRING(1, delim);
   NAN_OPTIONAL_ARGUMENT_STRING(2, quotes);
   NAN_OPTIONAL_ARGUMENT_BOOL(3, empty, false);

   vector<string> list = bkStrSplit(*str, *delim, *quotes, empty);
   NAN_RETURN(toArray(list, 0));
}

static NAN_METHOD(run)
{
   NAN_OPTIONAL_ARGUMENT_STRING(0, cmd);
   string out;

   FILE *fp = popen(*cmd, "r");
   if (fp) {
       size_t len;
       char buf[4096];
       while (!feof(fp)) {
           len = fread(buf, 1, sizeof(buf), fp);
           if (len) out += string(buf, len);
       }
       pclose(fp);
   }
   NAN_RETURN(Nan::New(out.c_str(), out.size()).ToLocalChecked());
}

static NAN_METHOD(getTimeOfDay)
{
    NAN_OPTIONAL_ARGUMENT_INT(0, type, 0);
    timeval t;
    int r = gettimeofday(&t, NULL);
    if (r < 0) t.tv_sec = t.tv_usec = 0;
    if (type == 1) {
        info.GetReturnValue().Set(Nan::New<v8::Number>(t.tv_sec + (t.tv_usec * 0.000001)));
    } else
    if (type == 2) {
        Local<Object> obj = Nan::New<Object>();
        Nan::Set(obj, Nan::New("tv_sec").ToLocalChecked(), Nan::New<v8::Number>(t.tv_sec));
        Nan::Set(obj, Nan::New("tv_usec").ToLocalChecked(), Nan::New<v8::Number>(t.tv_usec));
        NAN_RETURN(obj);
    } else {
        info.GetReturnValue().Set(Nan::New<v8::Number>((t.tv_sec * 1000000.0) + t.tv_usec));
    }
}


static NAN_MODULE_INIT(UtilsInit)
{
    bkLibInit();

    NAN_EXPORT(target, run);

    NAN_EXPORT(target, strSplit);

    NAN_EXPORT(target, getUser);
    NAN_EXPORT(target, getGroup);

    NAN_EXPORT(target, countWordsInit);
    NAN_EXPORT(target, countWords);
    NAN_EXPORT(target, countAllWords);

    NAN_EXPORT(target, snappyCompress);
    NAN_EXPORT(target, snappyUncompress);

    NAN_EXPORT(target, zlibCompress);
    NAN_EXPORT(target, zlibUncompress);

    NAN_EXPORT(target, geoDistance);
    NAN_EXPORT(target, geoBoundingBox);
    NAN_EXPORT(target, geoHashEncode);
    NAN_EXPORT(target, geoHashDecode);
    NAN_EXPORT(target, geoHashAdjacent);
    NAN_EXPORT(target, geoHashGrid);
    NAN_EXPORT(target, geoHashRow);

    NAN_EXPORT(target, zipFiles);
    NAN_EXPORT(target, unzipFile);
    NAN_EXPORT(target, unzip);

    NAN_EXPORT(target, initBusy);
    NAN_EXPORT(target, isBusy);
    NAN_EXPORT(target, getBusy);

    NAN_EXPORT(target, getTimeOfDay);
}

NODE_MODULE(binding, UtilsInit);
