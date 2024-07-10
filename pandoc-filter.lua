-- Fix gemini:// links
-- Requiered until this PR gets merged
-- https://github.com/jgm/pandoc/pull/9968
function Link(el)
  protopos = string.find(el.target, "/gemini://")
  if protopos then
    el.target = string.sub(el.target, protopos + 1);
  end
  return el
end

-- Change CodeBlock size on latex
function CodeBlock(block)
  if FORMAT == "beamer" or FORMAT == "latex" then
    return {
      pandoc.RawInline('latex', '\\footnotesize'),
      block,
      pandoc.RawInline('latex', '\\normalsize')
    }
  else
    return block
  end
end