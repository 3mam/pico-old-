
function initGL(gl) {
  const verTex = `
   attribute vec2 a_position;
   attribute vec2 a_texCoord;
   varying vec2 v_texCoord;
   void main() {
    gl_Position = vec4(a_position, 0, 1);
    v_texCoord = a_texCoord;
  }
`

  const fragTex = `
  precision mediump float;
  uniform sampler2D u_image;
  varying vec2 v_texCoord;
  void main() {
    gl_FragColor = vec4(texture2D(u_image, v_texCoord).xyz, 1);
  }
`

  function shader(str, typ) {
    let shader = gl.createShader(typ)
    gl.shaderSource(shader, str)
    gl.compileShader(shader)
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      return -1
    }
    return shader
  }

  function createShader(ver, frag) {
    let verShader = shader(ver, gl.VERTEX_SHADER)
    let fragShader = shader(frag, gl.FRAGMENT_SHADER)
    let id = gl.createProgram()
    gl.attachShader(id, verShader)
    gl.attachShader(id, fragShader)
    gl.linkProgram(id)

    return id
  }

  let program = createShader(verTex, fragTex)
  let positionLocation = gl.getAttribLocation(program, "a_position")
  let texCoordLocation = gl.getAttribLocation(program, "a_texCoord")
  gl.useProgram(program)

  let buffer = gl.createBuffer()
  gl.bindBuffer(gl.ARRAY_BUFFER, buffer)
  gl.bufferData(
    gl.ARRAY_BUFFER,
    new Float32Array([-1.0, 1.0,
      1.0, 1.0, -1.0, -1.0, -1.0, -1.0,
      1.0, 1.0,
      1.0, -1.0
    ]),
    gl.STATIC_DRAW)
  gl.enableVertexAttribArray(positionLocation)
  gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0)

  let texCoordBuffer = gl.createBuffer()
  gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer)
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
    0.0, 0.0,
    1.0, 0.0,
    0.0, 1.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0
  ]), gl.STATIC_DRAW)
  gl.enableVertexAttribArray(texCoordLocation)
  gl.vertexAttribPointer(texCoordLocation, 2, gl.FLOAT, false, 0, 0)

  let texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture)
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE)
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE)
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
}

class graphic {
  constructor(width, height) {
    this.canvas = document.getElementById('canvas')
    this.gl = canvas.getContext('webgl')
    const palette = new Uint8Array([
      255, 0, 64, 0,
      19, 19, 19, 1,
      27, 27, 27, 2,
      39, 39, 39, 3,
      61, 61, 61, 4,
      93, 93, 93, 5,
      133, 133, 133, 6,
      180, 180, 180, 7,
      255, 255, 255, 8,
      199, 207, 221, 9,
      146, 161, 185, 10,
      101, 115, 146, 11,
      66, 76, 110, 12,
      42, 47, 78, 13,
      26, 25, 50, 14,
      14, 7, 27, 15,
      28, 18, 28, 16,
      57, 31, 33, 17,
      93, 44, 40, 18,
      138, 72, 54, 19,
      191, 111, 74, 20,
      230, 156, 105, 21,
      246, 202, 159, 22,
      249, 230, 207, 23,
      237, 171, 80, 24,
      224, 116, 56, 25,
      198, 69, 36, 26,
      142, 37, 29, 27,
      255, 80, 0, 28,
      237, 118, 20, 29,
      255, 162, 20, 30,
      255, 200, 37, 31,
      255, 235, 87, 32,
      211, 252, 126, 33,
      153, 230, 95, 34,
      90, 197, 79, 35,
      51, 152, 75, 36,
      30, 111, 80, 37,
      19, 76, 76, 38,
      12, 46, 68, 39,
      0, 57, 109, 40,
      0, 105, 170, 41,
      0, 152, 220, 42,
      0, 205, 249, 43,
      12, 241, 255, 44,
      148, 253, 255, 45,
      253, 210, 237, 46,
      243, 137, 245, 47,
      219, 63, 253, 48,
      122, 9, 250, 49,
      48, 3, 217, 50,
      12, 2, 147, 51,
      3, 25, 63, 52,
      59, 20, 67, 53,
      98, 36, 97, 54,
      147, 56, 143, 55,
      202, 82, 201, 56,
      200, 80, 134, 57,
      246, 129, 135, 58,
      245, 85, 93, 59,
      234, 50, 60, 60,
      196, 36, 48, 61,
      137, 30, 43, 62,
      87, 28, 39, 63,
    ])

    this.paletteOriginal = new ArrayBuffer(4 * 64)
    this.paletteOriginal.ui8 = new Uint8Array(this.paletteOriginal)
    this.paletteOriginal.ui32 = new Uint32Array(this.paletteOriginal)
    this.paletteOriginal.ui8.set(palette)

    this.palette = new ArrayBuffer(4 * 64)
    this.palette.ui8 = new Uint8Array(this.palette)
    this.palette.ui32 = new Uint32Array(this.palette)
    this.palette.ui8.set(palette)

    this.sprites = new Map()

    this.screen = {
      x: 0,
      y: 0,
      width: width,
      height: height,

      clip: {
        x: -1,
        y: -1,
        width: width,
        height: height,
      },
      palette: new Int8Array(this.palette.ui32.length),
    }
    this.screen.palette.fill(-1)
    this.view = {
      x: 0,
      y: 0,
      width: 0,
      height: 0,
    }

    this.window = {
      width: 0,
      height: 0,
    }

    this.alpha = new Uint8Array(this.palette.ui32.length)
    this.alpha.fill(255)
    this.alpha[0] = 0
    this.zBuffer = new Int8Array(width * height)
    this.canvas = new ArrayBuffer(width * height * 4)
    this.canvas.ui8 = new Uint8Array(this.canvas)
    this.canvas.ui32 = new Uint32Array(this.canvas)

    initGL(this.gl)
  }

  draw() {
    /*
    let c = 0
    for (let i = this.canvas.ui32.length; i--;) {
      c = this.canvas.ui32[i] >> 24
      if (this.screen.palette[c] != -1) {
        this.canvas.ui32[i] = this.paletteOriginal.ui32[this.screen.palette[c]]
      }
    }
    */

    //this.gl.clear(this.gl.GL_COLOR_BUFFER_BIT)
    this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.RGBA, this.screen.width, this.screen.height, 0, this.gl.RGBA, this.gl.UNSIGNED_BYTE, this.canvas.ui8)
    this.gl.drawArrays(this.gl.TRIANGLES, 0, 6)
  }

  cls(col = 0) {
    for (let n = this.canvas.ui32.length; n--;) {
      this.canvas.ui32[n] = this.palette.ui32[col]
      this.zBuffer[n] = 0
    }
  }

  pal(c0, c1, t) {
    if (t) {
      this.screen.palette[c0] = c1
    }
    else {
      this.palette.ui32[c0] = this.paletteOriginal.ui32[c1]
    }
  }

  palc() {
    this.screen.palette.fill(-1)
    this.alpha.fill(255)
    this.alpha[0] = 0
    this.palette.ui32.set(this.paletteOriginal.ui32)
  }

  palt(c, t) {
    if (t) {
      this.alpha[c] = 1
    }
    else {
      this.alpha[c] = 0
    }
  }

  clip(x, y, w, h) {
    this.screen.clip.x = x - 1
    this.screen.clip.y = y - 1
    this.screen.clip.width = w
    this.screen.clip.height = h
  }

  pget(x, y) {
    if (x < this.screen.width && x > -1 && y < this.screen.height && y > -1) {
      const xy = ((y * this.screen.width) + x)
      return this.canvas[xy] >> 24
    }
    return -1
  }

  pset(x, y, c, a, z) {
    x -= this.screen.x
    y -= this.screen.y
    if (this.alpha[c] == 255 && x < this.screen.width && x > -1 &&
      y < this.screen.height && y > -1 && x < this.screen.clip.width &&
      x > this.screen.clip.x && y < this.screen.clip.height &&
      y > this.screen.clip.y) {

      const xy = ((y * this.screen.width) + x)
      if (this.zBuffer[xy] > z) {
        return
      }
      this.zBuffer[xy] = z;
      if (a < 255) {
        let rb = this.palette.ui32[c] & 0xff00ff
        let g = this.palette.ui32[c] & 0x00ff00
        rb += ((this.canvas.ui32[xy] & 0xff00ff) - rb) * (255 - a) >> 8
        g += ((this.canvas.ui32[xy] & 0x00ff00) - g) * (255 - a) >> 8
        this.canvas.ui32[xy] = (rb & 0xff00ff) | (g & 0xff00)
      } else {
        this.canvas.ui32[xy] = this.palette.ui32[c]
      }
    }
  }

  circFill(x0, y0, r, col, a, z) {
    x0 = Math.floor(x0)
    y0 = Math.floor(y0)
    let x = -r
    let y = 0
    let err = 2 - 2 * r // r * 2 - 2
    let yRepit = 0

    for (; x < 0;) {
      r = err
      if (r > x) {
        x++
        err += x * 2 + 1
      }
      if (r <= y) {
        y++
        err += y * 2 + 1
      }

      if (yRepit == y) {
        continue
      }
      yRepit = y

      for (let i = x; i < 0; i++) {
        this.pset(x0 + x - i - 1, y0 - y, col, a, z)
        this.pset(x0 + x - i - 1, y0 + y - 1, col, a, z)
        this.pset(x0 - x + i, y0 - y, col, a, z)
        this.pset(x0 - x + i, y0 + y - 1, col, a, z)
      }
    }
  }

  sprLoad(name) {
    const img = new Image
    img.src = name
    img.onload = () => {
      const canvas = document.createElement('canvas')
      canvas.width = img.width
      canvas.height = img.height
      const ctx = canvas.getContext("2d")
      ctx.drawImage(img, 0, 0)
      const pixels = ctx.getImageData(0, 0, img.width, img.height)
      this.sprites.set(name, new sprite(pixels, this.paletteOriginal.ui8))
    }
  }
  
  spr(name, n, x, y, w, h, flip, z) {
    const s = this.sprites.get(name)
    if (s === void 0) {
      return
    }

    x -= this.screen.x
    y -= this.screen.y
    let c = 0
    let ww = 0
    let hh = 0
    let x0 = 0
    let y0 = 0

    if (w == 0 && h == 0 && n == 0) {
      w = s.w / 10
      h = s.h / 10
    }
  
    ww = s.cell[n].x + (w * 10)
    hh = s.cell[n].y + (h * 10)
  
    for (let yy = s.cell[n].y; yy < hh; yy++) {
      y0 = (yy - hh) + y + (10 * h)

      if (y0 < this.screen.height && y0 > -1 && y0 < this.screen.clip.height &&
        y0 > this.screen.clip.y) {
          
        for (let xx = s.cell[n].x; xx < ww; xx++) {
          x0 = (xx - ww) + x + (10 * w)
          if (x0 < this.screen.width && x0 > -1 && x0 < this.screen.clip.width &&
            x0 > this.screen.clip.x) {
            switch (flip) {
            case 3:
              c = s.canvas[((-yy + hh) * s.w) + (-xx + ww)]
              break
            case 2:
              c = s.canvas[((-yy + hh) * s.w) + xx];
              break
            case 1:
              c = s.canvas[(yy * s.w) + (-xx + ww)]
              break
            case 0:
              c = s.canvas[((yy * s.w) + xx)]
            }
            if (this.zBuffer[y0 * this.screen.width + x0] > z) {
              continue
            }
            if (this.alpha[c] == 255) {
              this.zBuffer[y0 * this.screen.width + x0] = z
              this.canvas.ui32[y0 * this.screen.width + x0] = this.palette.ui32[c]
            }
          }
        }
      }
    }
  }
}

class sprite {
  constructor(src = ImageData, palette = Uint8Array) {
    this.canvas = new Uint8Array(src.width * src.height)
    this.w = src.width
    this.h = src.height
    for (let i = this.canvas.length; i--;) {
      for (let j = palette.length; j -= 4;) {
        if (src.data[i * 4] == palette[j] &&
          src.data[i * 4 + 1] == palette[j + 1] &&
          src.data[i * 4 + 2] == palette[j + 2]) {
          this.canvas[i] = j / 4
        }
      }
    }
    let n = (this.w / 10) * (this.h / 10)
    this.cell = new Array(n)
    n--
    for (let y = this.h / 10; y--;) {
      for (let x = this.w / 10; x--;) {
        this.cell[n] = {
          x: (x * 10),
          y: (y * 10),
        }
        n--
      }
    }
  }
}