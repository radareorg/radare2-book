const fs = require('fs');
const path = require('path');
const snippets = process.argv.slice(2);
const { spawnSync } = require('child_process');

for (let snip of snippets) {
  const dir = path.dirname(snip);
  const text = fs.readFileSync(snip);
  console.error('FILE', snip);
  parseSnippet(snip, dir, text.toString('utf8'))
}

function system(cmd, args) {
  const result = spawnSync(cmd, args, { stdio: ['ignore', 'pipe', 2]});
  return result.stdout;
}

function mustRebuild(input, output) {
  try {
    const s = fs.statSync(output);
    const a = new Date(s.mtime);
    const i = fs.statSync(input);
    const b = new Date(i.mtime);
    if (b < a) {
      return true;
    }
    return false;
  } catch (e) {
    return true;
  }
}

function buildSnippetImage(input, output, mode, args) {
  if (!mustRebuild(input, output)) {
    console.error('PNG ---', output);
    return true;
  }
  if (mode === 'string') {
    console.error('PNG STR', output);
    const png = system("node", ['r2png/index.js', '"', ...args]);
    fs.writeFileSync(output, png);
  } else {
    // we need to parse the first arg to get the filename. lets use - for now
    console.error('PNG R2R', output);
    const png = system("node", ['r2png/index.js', ...args]);
    fs.writeFileSync(output, png);
  }
}

function parseSnippet(input, dir, text) {
  var filename = '';
  var readMode = '';
  var args = [];
  var comment = {};
  for (let _line of text.split('\n')) {
    const line = _line.trim();
    if (readMode) {
      if (line === readMode) {
        const mode = (readMode === '"')? 'string': 'commands';
        if (mode === 'commands') {
          args = [comment.open, ...args];
        }
        // console.error("RUN", filename, mode, args);
        buildSnippetImage(input, path.join(dir, filename), mode, args);
        readMode = '';
        args = [];
        continue;
      }
      args.push(line);
      continue;
    }
    if (line.startsWith('//')) {
      const words = line.substring(2).trim().split(' ');
      comment[words[0]] = words[1];
    }
    if (line.endsWith('"')) {
      if (line.endsWith("(\"")) {
        filename = line.substring(0, line.length - 2).trim();
        readMode = '\")';
      } else {
        filename = line.substring(0, line.length - 1).trim();
        readMode = '\"';
      }
    }
  }
}
