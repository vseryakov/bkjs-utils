{
    "target_defaults": {
      "defines": [
        "SNAPPY=1",
        "NDEBUG",
        "_REENTRANT",
        "_THREAD_SAFE",
        "_POSIX_PTHREAD_SEMANTICS",
        "UNSAFE_STAT_OK",
      ],
      "include_dirs": [
        ".",
        "bklib",
        "snappy",
        "<(node_root_dir)/deps/openssl/openssl/include",
        "/opt/local/include",
        "<!(node -e \"require('nan')\")"
      ]
    },
    "targets": [
    {
      "target_name": "binding",
      "libraries": [
        "-L/opt/local/lib",
      ],
      "sources": [
        "binding.cpp",
        "bklib/bklog.cpp",
        "bklib/bklib.cpp",
        "bklib/bkzip.cpp",
        "bklib/bkunzip.cpp",
        "bklib/bkregexp.cpp",
        "snappy/snappy.cc",
        "snappy/snappy-sinksource.cc",
        "snappy/snappy-stubs-internal.cc",
      ],
      "conditions": [
        [ 'OS=="mac"', {
          "defines": [
            "OS_MACOSX",
          ],
          "xcode_settings": {
            "OTHER_CFLAGS": [
              "-g -fPIC",
            ],
          },
        }],
        [ 'OS=="linux"', {
          "defines": [
            "OS_LINUX",
          ],
          "cflags_cc+": [
            "-g -fPIC -rdynamic",
          ],
        }],
      ]
    }]
}
