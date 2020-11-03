import test from 'tape-async'
import {Event} from '../index'

test('Event object creation', async (t:any): Promise<void> => {
    t.equal(typeof Event, "function")
    const s = new Event("test",null)
    t.equal(typeof s, "object")
    t.true(Array.isArray((s as any).listeners));

    t.equal(typeof s.listen, "function")
    t.equal(typeof s.invoke, "function")

    t.equal((s as any).sender, null);
    t.equal((s as any).eventname, "test");
})

test('Add listener', async (t:any): Promise<void> => {
    const ev = new Event("test",null)
    ev.listen(() => {});
    t.equal(1, (ev as any).listeners.length);
})

test('Invoke event', async (t:any): Promise<void> => {
    const ev = new Event("test",null)
    let called = 0;
    ev.listen(() => {
        called++;
    });
    ev.listen(() => {
        called++;
    });
    ev.invoke(null)
    t.equal(2, called);
})
