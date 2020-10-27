import test from 'tape-async'
const {sum,sum_exactly_two,sum_at_least_two} = require("../build/Release/desktop.node")

test('sum exported', (t:any) => {
    t.equal(typeof sum, "function")
    t.end()
})

test('return sum of a & b', (t:any) => {
    t.equal(sum(40,2), 42)
    t.end()
})

test('excess arguments ignored', (t:any) => {
    t.equal(sum(40,2,"ciao"), 42)
    t.end()
})

test('non supplied arguments are undefined.', (t:any) => {
    t.throws(()=>sum(40), /A number was expected/)
    t.end()
})

test('only number accepted.', (t:any) => {
    t.throws(()=>sum(40,""), /A number was expected/)
    t.end()
})


test('sum_exactly_two exported', (t:any) => {
    t.equal(typeof sum_exactly_two, "function")
    t.end()
})

test('sum_at_least_two exported', (t:any) => {
    t.equal(typeof sum_at_least_two, "function")
    t.end()
})

test('sum_exactly_two requires exactly two args', (t:any) => {
    t.equal(sum_exactly_two(40,2), 42)
    t.throws(()=>sum_exactly_two(40), /Wrong number of arguments. 2 required./)
    t.throws(()=>sum_exactly_two(40,1,2), /Wrong number of arguments. 2 required./)
    t.end()
})

test('sum_at_least_two requires at least two args', (t:any) => {
    t.equal(sum_at_least_two(40,2), 42)
    t.throws(()=>sum_at_least_two(40), /Too few arguments. At least 2 required./)
    t.equal(sum_at_least_two(40,1,2),41)
    t.end()
})