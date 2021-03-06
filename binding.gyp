{
	"targets": [
		{
			"target_name": "addon",
			"sources": [
				"src/addon.cc",
				"src/blake2.cc",

				"lib/BLAKE2/sse/blake2b.c",
				"lib/BLAKE2/sse/blake2bp.c",
				"lib/BLAKE2/sse/blake2s.c",
				"lib/BLAKE2/sse/blake2sp.c"
			],
			"include_dirs" : [
				"lib/BLAKE2/sse"
			],
			"cflags_c": [ "-std=c99" ]
		}
	]
}
