# Utilities module for node and backendjs

# Usage
 - `splitArray(str)` - split a string into an array separated by commas, supports double quotes
 - `logging([level])` - set or return logging level, this is internal C++ logging facility
 - `loggingChannel(channelname)` - redirect logging into stdout or stderr, this is internal C++ logging
 - `countWords(word, text)` - return how many time word appers in the text, uses Knuth-Morris-Pratt algorithm
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
   - `geoHashAdjacent()`
   - `geoHashGrid()`
   - `geoHashRow()`

# Author 

Vlad Seryakov

