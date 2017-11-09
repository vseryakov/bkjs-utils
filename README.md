# Utilities module for node and backendjs

# Usage
 - `splitArray(str)` - split a string into an array separated by commas, supports quoted strings
 - `countWords(word, text)` - return how many time word appears in the text, uses Knuth-Morris-Pratt algorithm
 - `countAllWords(list, text)` - return an object with counters for each word from the list, i.e. how many times each word appears in the text, uses Aho-Corasick algorithm
 - `countWordsInit()` - clears word counting cache
 - `snappyCompress(str)` - compress a string
 - `snappyUncompress(str)` - decompress a string
 - `zlibCompress(str)` - compress a string
 - `zlibUncompress(str)` - decompress a string
 - `unzip(zipfile, outdir)` - extract a zip archive into directory
 - `unzipFile(zipfile, file [, outfile])` - extract a file from zip archive, return contents if no outfile s specified
 - `run(command, callback)` - run shell command and return all output to the callback
 - `getUser([user])` - return an object with user info from the /etc/passwd file, user can be uid or name
 - `getGroup([group])` - return an object with specified group info for the current user of for the given group id or name
 - Geohash support
   - `geoDistance(lat1, lon1, lat2, lon2)` - return distance between 2 coordinates in km
   - `geoBoundingBox(lat, lon, distance)` - return bounding box geohash for given point around distance
   - `geoHashEncode(lat, lon, len)` - return geohash for given coordinate, len defines number of bytesin geohash
   - `geoHashDecode(hash)` - return coordinates for given geohash
   - `geoHashAdjacent(geohash, dir)` - return adjacent boxes for the given base in the given direction, dir is one of top,left,right,bottom
   - `geoHashGrid(geoshash, steps)` - return grid of all neighboring boxes around the center, steps defines how many boxes in each direction from the center. With steps = 1 the returned matrix is 3x3
   - `geoHashRow(geohash, steps)` - same as grid but returns only one row, i.e. left of right of the center
 - `initBusy(ms)` - setup busy monitor to track if incoming requests processing takes longer than given period in milliseconds
 - `isBusy()` - return true if currently request processing takes longer than configured interval
 - `getBusy()` - returns current lag for processing requests
 - `getTimeOfDay([type])` - return current time in microseconds, if type == 1 returns ad double seconds.microseconds, if type == 2 returns an object { tv_sec: T, tv_usec: U }

# Notes

For whatever reason npm exits with error `gyp: name 'openssl_fips' is not defined......` run it as

      npm i bkjs-utils --openssl_fips=X

# Author

Vlad Seryakov

