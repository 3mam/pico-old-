class _font {
  constructor(n, x, y) {
    this.x = x
    this.y = y
    this.n = n
  }
}
class font {
  constructor(g = graphic, fontName = 'data/font.png') {
    this.draw = g
    g.sprLoad(fontName)
    this.fontName = fontName
    this.font = new Map()

    this.font.set('A', new _font(0, 6, 0))
    this.font.set('B', new _font(1, 6, 0))
    this.font.set('C', new _font(2, 6, 0))
    this.font.set('D', new _font(3, 6, 0))
    this.font.set('E', new _font(4, 5, 0))
    this.font.set('F', new _font(5, 5, 0))
    this.font.set('G', new _font(6, 6, 0))
    this.font.set('H', new _font(7, 6, 0))
    this.font.set('I', new _font(8, 4, 0))
    this.font.set('J', new _font(9, 5, 0))
    this.font.set('K', new _font(10, 6, 0))
    this.font.set('L', new _font(11, 5, 0))
    this.font.set('M', new _font(12, 6, 0))
    this.font.set('N', new _font(13, 6, 0))
    this.font.set('O', new _font(14, 6, 0))
    this.font.set('P', new _font(15, 6, 0))
    this.font.set('R', new _font(16, 6, 0))
    this.font.set('S', new _font(17, 6, 0))
    this.font.set('T', new _font(18, 6, 0))
    this.font.set('U', new _font(19, 6, 0))
    this.font.set('W', new _font(20, 6, 0))
    this.font.set('Y', new _font(21, 5, 0))
    this.font.set('Z', new _font(22, 6, 0))
    this.font.set('a', new _font(23, 6, 0))
    this.font.set('b', new _font(24, 6, 0))
    this.font.set('c', new _font(25, 6, 0))
    this.font.set('d', new _font(26, 6, 0))
    this.font.set('e', new _font(27, 6, 0))
    this.font.set('f', new _font(28, 5, 0))
    this.font.set('g', new _font(29, 6, 0))
    this.font.set('h', new _font(30, 6, 0))
    this.font.set('i', new _font(31, 2, 0))
    this.font.set('j', new _font(32, 3, 0))
    this.font.set('k', new _font(33, 6, 0))
    this.font.set('l', new _font(34, 2, 0))
    this.font.set('m', new _font(35, 8, 0))
    this.font.set('n', new _font(36, 5, 0))
    this.font.set('o', new _font(37, 6, 0))
    this.font.set('p', new _font(38, 6, 0))
    this.font.set('r', new _font(39, 5, 0))
    this.font.set('s', new _font(40, 6, 0))
    this.font.set('y', new _font(44, 6, 0))
    this.font.set('u', new _font(42, 6, 0))
    this.font.set('w', new _font(43, 6, 0))
    this.font.set('t', new _font(41, 6, 0))
    this.font.set('z', new _font(45, 6, 0))
    this.font.set('0', new _font(65, 4, 0))
    this.font.set('1', new _font(66, 5, 0))
    this.font.set('2', new _font(67, 5, 0))
    this.font.set('3', new _font(68, 5, 0))
    this.font.set('4', new _font(69, 5, 0))
    this.font.set('5', new _font(70, 5, 0))
    this.font.set('6', new _font(71, 5, 0))
    this.font.set('7', new _font(72, 5, 0))
    this.font.set('8', new _font(73, 5, 0))
    this.font.set('9', new _font(74, 5, 0))
    this.font.set('.', new _font(75, 2, 0))
    this.font.set(',', new _font(76, 2, 0))
    // font['X'] = &_X;
    // font['x'] = &_x;
    // font['_'] = &__;
  }

  print(x, y, z, str) {
    let px = x
    let py = y
    let colorNum = 0

    for (let i = 0; i < str.length; i++) {
      const ch = this.font.get(str[i])
      switch (str[i]) {
        case '\n':
          py += 10
          px = x
          break
        case ' ':
          px += 5
          break
        case '^':
          colorNum = parseInt(str.substring(i + 1), 10)
          if (colorNum > 0 && colorNum < 10) {
            i += 1
          } else {
            i += 2
          }
          this.draw.pal(8, colorNum, 0)
          break
        default:
          this.draw.spr(this.fontName, ch.n, px, py + ch.y, 1, 1, 0, z)
          px += ch.x
      }
    }
    this.draw.palc()
  }
}