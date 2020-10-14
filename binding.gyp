{
	"targets": [{
		"target_name": "desktop",
		"include_dirs": ["<(module_root_dir)/includes","yoga"],
		"sources": [
			'<!@(node tools/list-sources.js)'
		],
		
		"conditions": [
			["OS=='win'", {
				
			}],
			["OS=='mac'", {
				"libraries": ["-framework AppKit"]
			}],
			["OS=='linux'", {
				"libraries": [
					'<!@(pkg-config gtk+-3.0 --libs-only-l)'
				],
				
				"cflags": [
					
					'<!@(pkg-config gtk+-3.0 --cflags-only-I)'
				]
			}]

		]
	}]
}
