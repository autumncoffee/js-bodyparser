{
  "targets": [
    {
      "target_name": "ac_bodyparser",
      "sources": [
        "<!@(find src -type f -name '*.cpp')",
        "<!@(find node_modules/ac-common/*.cpp -type f)",
        "<!@(find node_modules/ac-common/utils/*.cpp -type f)",
        "<!@(find node_modules/ac-library/http/utils/*.cpp -type f)",
        "<!@(find node_modules/ac-library/http/abstract_message.cpp -type f)",
        "<!@(find node_modules/ac-library/httplike/parser/parser.cpp -type f)"
      ],
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags": [ "-std=c++17" ],
      "cflags_cc": [ "-std=c++17" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "node_modules"
      ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "conditions": [
        ["OS==\"mac\"", {
          "cflags+": [ "-stdlib=libc++" ],
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS" : [ "-std=c++17", "-stdlib=libc++" ],
            "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LANGUAGE_STANDARD":"c++17",
            "CLANG_CXX_LIBRARY": "libc++"
          }
        }]
      ]
    }
  ]
}
