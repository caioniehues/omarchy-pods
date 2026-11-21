var LID_OPEN = 0
var LID_CLOSED = 1
var LID_UNKNOWN = 2
var CARD_COOLDOWN_MS = 15000

function newState() {
  return { lidState: LID_UNKNOWN, shownAt: null }
}

// The connected flag is not an input: it flips on wake, recovery and restart, which are not the case opening.
function shouldShow(state, lidState, now) {
  var opened = state.lidState === LID_CLOSED && lidState === LID_OPEN
  state.lidState = lidState
  if (!opened || (state.shownAt !== null && now - state.shownAt < CARD_COOLDOWN_MS)) return false
  state.shownAt = now
  return true
}
