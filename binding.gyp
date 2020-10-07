{
	"targets": [{
		"target_name": "desktop",
		"include_dirs": ["<(module_root_dir)/includes"],
		"sources": [
			'<!@(node tools/list-sources.js)'
		],
		"conditions": [
			["OS=='win'", {
				"libraries": [
				]
			}],
			["OS=='linux'", {
				"cflags": [
					"-std=gnu11",
					'<!@(pkg-config gtk+-3.0 --cflags-only-I)'
				],
				"ldflags": [
					'<!@(pkg-config gtk+-3.0 --libs)'
				],
			}]
		]
	}]
}
