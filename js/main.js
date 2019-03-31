class time {
  constructor() {
    this.delta = 0.016
  }

  start() {
    this.then = performance.now()
  }
  end() {
    this.now = performance.now()
    this.delta = (this.now - this.then) / 1000
    //this.delta *= 1000
  }
}

main()

var yMove = 100.0
var xMove = 100.0
var delta = 0.0
var press = {
  w: false,
  s: false,
  a: false,
  d: false,
}

function test({foo, ble}) {
  console.log(foo, ble)
}

function main() {
  const draw = new graphic(320, 180)
  const text = new font(draw)
  //view.setUint8(1,255,false)
  const foo = {
    foo: 100,
    ble: "ave", 
  }
  test(foo)
  draw.sprLoad('data/player.png')

  let a = 0
  let last = 0
  draw.cls(0)

  const f = (now) => {
    delta = (now - last) / 1000
    last = now
    draw.cls(1)

    if (press.w) {
      yMove -= delta*100
    }
    if (press.s) {
      yMove += delta*100
    }
    if (press.a) {
      xMove -= delta*100
    }
    if (press.d) {
      xMove += delta*100
    }

    draw.spr('data/player.png', 0, Math.floor(xMove), Math.floor(yMove), 0, 0, 0, 1)

    text.print(10, 10, 0, delta.toString())
    //console.log(delta)
    a += delta * 60
    if (a > 260) {
      a = 0
    }
    //draw.circFill(a, 50, 10, 8, 255, 0)
    draw.draw()
    requestAnimationFrame(f)
  }
  requestAnimationFrame(f)
}

window.addEventListener("keydown", function (event) {
  if (event.defaultPrevented) {
    return // Do nothing if the event was already processed
  }

  switch (event.key) {
    case "w":
    press.w = true
      break
    case "s":
    press.s = true
      break
    case "a":
    press.a = true
      break
    case "d":
    press.d = true
      break
    default:
      return // Quit when this doesn't handle the key event.
  }

  // Cancel the default action to avoid it being handled twice
  event.preventDefault()
}, true)

window.addEventListener("keyup", function (event) {
  if (event.defaultPrevented) {
    return // Do nothing if the event was already processed
  }

  switch (event.key) {
    case "w":
    press.w = false
      break
    case "s":
    press.s = false
      break
    case "a":
    press.a = false
      break
    case "d":
    press.d = false
    default:
      return // Quit when this doesn't handle the key event.
  }

  // Cancel the default action to avoid it being handled twice
  event.preventDefault()
}, true)