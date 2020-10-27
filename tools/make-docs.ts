#!/bin/env ts-node

const jd = require("javadoc")
import { join } from "path";

async function run() {
    const jsonFile = join(__dirname,"../docs/plugins/dsknapi.json")
    await jd.generate({
        include: [join(__dirname,"../includes/dsknapi.h")],
        exclude: ["**/node_modules/**"],
        format: "json",
        output: jsonFile,
    });
    
    const jsonDocs = require(jsonFile);
    for (const block of jsonDocs.success[0]) {
        for (const [key,val] of Object.entries(block)) {
            console.log(key,(val as any)[0])
        }

    }
    //console.log(JSON.stringify( jsonDocs,null,4))
}
 

run().catch(error => {
    console.error(console.error())
});