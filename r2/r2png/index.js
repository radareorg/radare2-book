const r2promise = require('r2pipe-promise');
const fs = require('fs');
const file = process.argv[2];
const cmds = process.argv.slice(3)
const text2png = require('text2png');

if (process.argv.length < 4) {
  console.error('Usage: r2png [file] [cmds ...] > output.png');
  process.exit(1);
}

function log(x) {
  if (x) {
    console.error(x);
  }
}

main().then(log).catch(log);

function replaceAll (txt, replace, with_this) {
  replace = replace.replace('|', '\\|');
  return txt.replace(new RegExp(replace, 'g'), with_this);
}

function renderText (txt, cb) {
  var w = 1;
  var h = 0;
  var col = 0;
  var text = '';
  var directives = [];
  var mode, modes = [];
  var tlen = txt.length;
  var bg = '0';

  for (var i = 0; txt[i] === ';' && i < tlen;) {
    switch (txt[i + 2]) {
      case '-':
      case '=':
      case '>':
      case '<':
        mode = txt[i + 2];
        break;
      default:
        continue;
    }
    for (var j = i; txt[j] != '\n' && j < tlen; j++);
    if (i === j) {
      //	console.log ('break ', i);
      break;
    }
    var d = txt.substring(i + 3, j);
    // cconsole.log ('directive',d,j);
    var txti = txt[i + 1];
    directives[txti] = d;
    modes[txti] = mode;
    i = j + 1;
    if (mode === '-') {
      bg = d.substring(1); // txt[i+1];
    }
  }
  txt = txt.substring(i);
  // console.log ('i', i, txt);
  for (var i = 0; i < txt.length; i++) {
    if (txt[i] === '\n') {
      h++;
      if (col > w) w = col - 1;
      col = 1;
    } else col++;
  }
  var text = txt;
  for (var a in directives) {
    var k = directives[a][0];
    var v = directives[a].substring(1);
    // console.log (a);
    if (modes[k] === '=' || modes[k] === '<') {
      text = replaceAll(text, a, '\x1b[' + v + 'm' + k + '\x1b[' + bg + 'm');
    } else {
      text = replaceAll(text, a, '\x1b[' + v + 'm' + k + '\x1b[' + bg + 'm');
      // text = replaceAll (text, a, '\x1b['+v+'m ');
    }
  }
  text += '\x1b[0m';
  if (h > 0) cb(file, text, w, h);
  else console.error('invalid sizes (h=0)');
}

function render(res) {
  renderText(res, function (f, t, w, h) {
    f = 'output';
    // console.log ('w',w,'h',h);
    // console.log (t);
    var e = require('./escapes').escapes(t, function (x) {
      var stream = x.canvas.createSyncPNGStream();
      var fh = fs.openSync(f + '.png', 'w');
      stream.on('data', function (x) {
        fs.writeSync(fh, x, 0, x.length);
      });
      stream.on('end', function (x) {
        fs.closeSync(fh);
      });
    }, {
      'width': w * 8,
      'height': h * 16
    });
  });
}

function render2(res) {
const png = text2png(res, { font: '48px Courier New', bgColor: 'white' });
process.stdout.write(png);
// fs.writeFileSync('output.png', png);
}

async function main() {
  if (file === '"') {
    render2 (cmds.join('\n'));
  } else {
    const r2 = await r2promise.open(file, ['-escr.color=0']);
    let res = '';
    for (let cmd of cmds) {
      const seek = (await r2.cmd('?vx $$')).trim();
      const out = await r2.cmd(cmd);
      res += '['+seek+']> ' + cmd + '\n' + out;
    }
    // render (res);
    render2 (res);
    await r2.quit();
  }
}

/*
r2pipe.open(file, ['-escr.color=0'], (err, r2) => {
  r2.cmd(cmds, (err, res) => {
    res = '[0x00000000]> ' + cmds + '\n' + res;
  });
});
*/
