{
	"targets": [{
		"target_name": "desktop",
		"include_dirs": ["<(module_root_dir)/includes"],
		"sources": [
			'<!@(node tools/list-sources.js)'
		],
		"libraries": [
			'<!@(pkg-config gtk+-3.0 --libs-only-l)'
		],
		"conditions": [
			["OS=='win'", {
				
			}],
			["OS=='linux'", {
				"cflags": [
					"-std=gnu11",
					'<!@(pkg-config gtk+-3.0 --cflags-only-I)'
				]
			}]
		]
	}]
}
