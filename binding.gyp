{
	"targets": [{
		"target_name": "libdesktop",
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
				"cflags": ["-std=gnu11"],
				'include_dirs': [
					'<!@(pkg-config gtk+-3.0 --cflags-only-I | sed s/-I//g)'
				],
				'libraries': [
					'<!@(pkg-config gtk+-3.0 --cflags-only-I | sed s/-l//g)'
				],
			}]
		]
	}]
}
