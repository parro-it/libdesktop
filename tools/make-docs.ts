#!/bin/env ts-node

const jd = require("javadoc")
import { join } from "path";

async function run() {
    await jd.generate({
        include: [join(__dirname,"../includes/dsknapi.h")],
        exclude: ["**/node_modules/**"],
        format: "json",
        output: join(__dirname,"../docs/plugins/dsknapi.json"),
    });

    
}
 

run().catch(error => {
    console.error(console.error())
});