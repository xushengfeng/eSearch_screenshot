{
    "targets": [
        {
            "target_name": "eSearch_screenshot",
            "cflags": ["-Wall", "-std=c++11"],
            "conditions":[
                ["OS=='linux'", {
                    "sources": ["linux.cc"],
                    'link_settings': {
                        'libraries': [
                            '-lpng',
                            '-lz',
                            '-lX11',
                            '-lXtst'
                        ]
                    },
                }],
                ["OS=='mac'", {
                    "sources": ["macos.cc"]
                }],
                ["OS=='win'", {
                    "sources": ["windows.cc"]
                }]
            ],
            "xcode_settings": {
                "OTHER_CFLAGS": [
                    "-std=c++11"
                ]
            },
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "ExceptionHandling": "1 # /EHsc"
                }
            },
        }
    ]
}
