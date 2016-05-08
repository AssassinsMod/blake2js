{
	"targets": [
		{
			"target_name": "hook",
			"sources": [
				"src/bindings.cc",
				"src/class/blake2b.cc",

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
