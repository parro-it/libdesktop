const {readdirSync, existsSync} = require('fs');
const {platform} = require('os')

function listDir(dir) {
	if (existsSync(dir)) {
		readdirSync(dir)
			.filter(f => f.endsWith('.c') || f.endsWith('.m'))
			.forEach(f => console.log(`${dir}/${f}`));
	}
}

function listCPPDir(dir) {
	if (existsSync(dir)) {
		readdirSync(dir)
			.filter(f => f.endsWith('.cc') || f.endsWith('.cpp'))
			.forEach(f => console.log(`${dir}/${f}`));
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
