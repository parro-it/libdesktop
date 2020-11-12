import test from 'tape-async'
const {sum,sum_exactly_two,sum_at_least_two,Decimal} = require("../build/Release/desktop.node")


test('Class definition', (t:any) => {
    t.equal(typeof Decimal, "function")
    t.equal(typeof new Decimal(4,42), "object")
    t.end()
})

test('property definition', (t:any) => {
    const d = new Decimal(4,42)
    //t.equal(4, d.integral);
    //d.integral = 13;
    //t.equal(13, d.integral);
    t.throws(()=>{d.integral = "13";}, /A number was expected/)
    t.end()
})

test('method definition', (t:any) => {
    const d = new Decimal(4,42)
    t.equal("4.42", String(d));
    t.end()
})

test('static method definition', (t:any) => {
    t.equal("function", typeof Decimal.create);
    t.equal(undefined, Decimal.create());
    t.end()
})

test('static property definition', (t:any) => {
    t.equal(undefined, Decimal.answer);
    Decimal.answer=42
    t.equal(42, Decimal._integral);
    Decimal._integral=41;
    t.equal(41, Decimal.answer);
    t.end()
})


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

test('napi_throw_error called manually', (t:any) => {
    try {
        Decimal.throwError1()
    } catch (err:any) {
        t.equal(err.code,"CIAO")
        t.equal(err.message,"SALVE")
        t.end()
        return
    }
    t.equal(true,false)
})


test('DSK_FAILURE called ', (t:any) => {
    try {
        Decimal.throwError2()
    } catch (err:any) {
        t.equal(err.code,undefined)
        t.equal(err.message,"gege")
        t.end()
        return
    }
    t.equal(true,false)
})

test('throw pending exception', (t:any) => {
    try {
        new Decimal(1,1).throwError3()
    } catch (err:any) {
        t.equal(err.code,undefined)
        t.equal(err.message,"Error: gege")
        t.end()
        return
    }
    t.equal(true,false)
})