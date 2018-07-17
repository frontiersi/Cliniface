-- 
--  This is file `fontspec.lua',
--  generated with the docstrip utility.
-- 
--  The original source files were:
-- 
--  fontspec.dtx  (with options: `lua')
--  fontspec-lua.dtx  (with options: `lua')
--  ------------------------------------------------
--  The FONTSPEC package for XeLaTeX/LuaLaTeX
--  (C)  2004--2017  Will Robertson and Khaled Hosny
--  License information appended.
--  ------------------------------------------------
fontspec          = fontspec or {}
local fontspec    = fontspec
fontspec.module   = {
    name          = "fontspec",
    version       = "2.6a",
    date          = "2017/03/31",
    description   = "Font selection for XeLaTeX and LuaLaTeX",
    author        = "Khaled Hosny, Philipp Gesang, Will Robertson",
    copyright     = "Khaled Hosny, Philipp Gesang, Will Robertson",
    license       = "LPPL"
}
local err, warn, info, log = luatexbase.provides_module(fontspec.module)
fontspec.log     = log  or (function (s) luatexbase.module_info("fontspec", s)    end)
fontspec.warning = warn or (function (s) luatexbase.module_warning("fontspec", s) end)
fontspec.error   = err  or (function (s) luatexbase.module_error("fontspec", s)   end)
local latex
if luatexbase.registernumber then
    latex = luatexbase.registernumber("catcodetable@latex")
else
    latex = luatexbase.catcodetables.CatcodeTableLaTeX
end
local function tempswatrue()  tex.sprint(latex,[[\FontspecSetCheckBoolTrue ]]) end
local function tempswafalse() tex.sprint(latex,[[\FontspecSetCheckBoolFalse]]) end
function fontspec.check_ot_script(fnt, script)
    if luaotfload.aux.provides_script(font.id(fnt), script) then
        tempswatrue()
    else
        tempswafalse()
    end
end
function fontspec.check_ot_lang(fnt, lang, script)
    if luaotfload.aux.provides_language(font.id(fnt), script, lang) then
        tempswatrue()
    else
        tempswafalse()
    end
end
function fontspec.check_ot_feat(fnt, feat, lang, script)
    for _, f in ipairs { "+trep", "+tlig", "+anum" } do
        if feat == f then
            tempswatrue()
            return
        end
    end
    if luaotfload.aux.provides_feature(font.id(fnt), script, lang, feat) then
        tempswatrue()
    else
        tempswafalse()
    end
end
function fontspec.mathfontdimen(fnt, str)
    local mathdimens = luaotfload.aux.get_math_dimension(fnt, str)
    if mathdimens then
        tex.sprint(mathdimens)
        tex.sprint("sp")
    else
        tex.sprint("0pt")
    end
end
--  ------------------------------------------------
--  Copyright 2004--2017 Will Robertson <will.robertson@latex-project.org>
--  Copyright 2009--2013   Khaled Hosny <khaledhosny@eglug.org>
--  
--  Distributable under the LaTeX Project Public License, version 1.3c or higher.
--  The latest version of this license is at: http://www.latex-project.org/lppl.txt
--  
--  This work is "maintained" by Will Robertson.
--  It consists of the files: fontspec*.dtx, fontspec.cfg, fontspec*.tex.
--  And the derived files: fontspec*.sty,fontspec.lua, fontspec.pdf.
--  ------------------------------------------------
-- 
--  End of file `fontspec.lua'.
