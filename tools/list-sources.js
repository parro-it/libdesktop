const {readdirSync, existsSync} = require('fs');
const {platform} = require('os');
const {join} = require('path');

function listDir(dir) {
	if (existsSync(dir)) {
		readdirSync(dir)
			.filter(f => f.endsWith('.c') || f.endsWith('.m'))
			.forEach(f => console.log(join(dir,f)));
		
		readdirSync(dir,{withFileTypes:true})
			.filter(f => f.isDirectory())
			.forEach(f => listDir(join(dir,f.name)));
	}
}

function listCPPDir(dir) {
	if (existsSync(dir)) {
		readdirSync(dir)
			.filter(f => f.endsWith('.cc') || f.endsWith('.cpp'))
			.forEach(f => console.log(join(dir,f)));
		readdirSync(dir,{withFileTypes:true})
			.filter(f => f.isDirectory())
			.forEach(f => listDir(join(dir,f.name)));
	}
}

listCPPDir('yoga/yoga');
listDir('src/platforms/common');
listDir(`src/platforms/${platform()}`);
if (process.env.LIBDESKTOP_TARGET === 'test') {
	listDir('tests/internals');
} /*else {
	console.log('tools/test_main_stub.c');
}*/
