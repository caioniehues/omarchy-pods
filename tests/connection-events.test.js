const source = Deno.readTextFileSync(new URL('../ConnectionEvents.js', import.meta.url))
const { newState, shouldShow, LID_OPEN, LID_CLOSED, LID_UNKNOWN, CARD_COOLDOWN_MS } =
  new Function(source + '; return { newState, shouldShow, LID_OPEN, LID_CLOSED, LID_UNKNOWN, CARD_COOLDOWN_MS }')()
function check(name, value, expected) { if (value !== expected) throw new Error(`${name}: got ${value}, expected ${expected}`) }

// The connected flag is not an input: it flips on wake, recovery and restart.
check('shouldShow takes state, lid state and time only', shouldShow.length, 3)

let state = newState()
check('first observation, open, never shows', shouldShow(state, LID_OPEN, 0), false)

state = newState()
check('first observation, closed, never shows', shouldShow(state, LID_CLOSED, 0), false)
check('closed to open shows', shouldShow(state, LID_OPEN, 100), true)
check('open to open does not show', shouldShow(state, LID_OPEN, 200), false)

state = newState()
check('unknown first observation is quiet', shouldShow(state, LID_UNKNOWN, 0), false)
check('unknown to open does not show', shouldShow(state, LID_OPEN, 100), false)

state = newState()
shouldShow(state, LID_CLOSED, 0)
check('closed to unknown is quiet', shouldShow(state, LID_UNKNOWN, 100), false)
check('unknown to open after closed does not show', shouldShow(state, LID_OPEN, 200), false)

state = newState()
shouldShow(state, LID_CLOSED, 0)
check('first opening shows', shouldShow(state, LID_OPEN, 1000), true)
shouldShow(state, LID_CLOSED, 2000)
check('second opening within the cooldown does not show', shouldShow(state, LID_OPEN, 1000 + CARD_COOLDOWN_MS - 1), false)
shouldShow(state, LID_CLOSED, 1000 + CARD_COOLDOWN_MS)
check('an opening after the cooldown shows', shouldShow(state, LID_OPEN, 1000 + 2 * CARD_COOLDOWN_MS), true)

state = newState()
for (let t = 0; t < 5; t++) check('status updates with no lid change never show', shouldShow(state, LID_UNKNOWN, t * 1000), false)
console.log('connection-events.test.js: all checks passed')
