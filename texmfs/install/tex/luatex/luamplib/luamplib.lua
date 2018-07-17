-- 
--  This is file `luamplib.lua',
--  generated with the docstrip utility.
-- 
--  The original source files were:
-- 
--  luamplib.dtx  (with options: `lua')
--  
--  See source file 'luamplib.dtx' for licencing and contact information.
--  

luamplib          = luamplib or { }


local luamplib    = luamplib
luamplib.showlog  = luamplib.showlog or false
luamplib.lastlog  = ""

luatexbase.provides_module {
  name          = "luamplib",
  version       = "2.11.3",
  date          = "2016/03/31",
  description   = "Lua package to typeset Metapost with LuaTeX's MPLib.",
}


local format, abs = string.format, math.abs

local err  = function(...) return luatexbase.module_error  ("luamplib", format(...)) end
local warn = function(...) return luatexbase.module_warning("luamplib", format(...)) end
local info = function(...) return luatexbase.module_info   ("luamplib", format(...)) end

local stringgsub    = string.gsub
local stringfind    = string.find
local stringmatch   = string.match
local stringgmatch  = string.gmatch
local stringexplode = string.explode
local tableconcat   = table.concat
local texsprint     = tex.sprint
local textprint     = tex.tprint

local texget      = tex.get
local texgettoks  = tex.gettoks
local texgetbox   = tex.getbox

local mplib = require ('mplib')
local kpse  = require ('kpse')
local lfs   = require ('lfs')

local lfsattributes = lfs.attributes
local lfsisdir      = lfs.isdir
local lfsmkdir      = lfs.mkdir
local lfstouch      = lfs.touch
local ioopen        = io.open

local file = file or { }
local replacesuffix = file.replacesuffix or function(filename, suffix)
  return (stringgsub(filename,"%.[%a%d]+$","")) .. "." .. suffix
end
local stripsuffix = file.stripsuffix or function(filename)
  return (stringgsub(filename,"%.[%a%d]+$",""))
end

local is_writable = file.is_writable or function(name)
  if lfsisdir(name) then
    name = name .. "/_luam_plib_temp_file_"
    local fh = ioopen(name,"w")
    if fh then
      fh:close(); os.remove(name)
      return true
    end
  end
end
local mk_full_path = lfs.mkdirs or function(path)
  local full = ""
  for sub in stringgmatch(path,"(/*[^\\/]+)") do
    full = full .. sub
    lfsmkdir(full)
  end
end

local luamplibtime = kpse.find_file("luamplib.lua")
luamplibtime = luamplibtime and lfsattributes(luamplibtime,"modification")

local currenttime = os.time()

local outputdir
if lfstouch then
  local texmfvar = kpse.expand_var('$TEXMFVAR')
  if texmfvar and texmfvar ~= "" and texmfvar ~= '$TEXMFVAR' then
    for _,dir in next,stringexplode(texmfvar,os.type == "windows" and ";" or ":") do
      if not lfsisdir(dir) then
        mk_full_path(dir)
      end
      if is_writable(dir) then
        local cached = format("%s/luamplib_cache",dir)
        lfsmkdir(cached)
        outputdir = cached
        break
      end
    end
  end
end
if not outputdir then
  outputdir = "."
  for _,v in ipairs(arg) do
    local t = stringmatch(v,"%-output%-directory=(.+)")
    if t then
      outputdir = t
      break
    end
  end
end

function luamplib.getcachedir(dir)
  dir = dir:gsub("##","#")
  dir = dir:gsub("^~",
    os.type == "windows" and os.getenv("UserProfile") or os.getenv("HOME"))
  if lfstouch and dir then
    if lfsisdir(dir) then
      if is_writable(dir) then
        luamplib.cachedir = dir
      else
        warn("Directory '"..dir.."' is not writable!")
      end
    else
      warn("Directory '"..dir.."' does not exist!")
    end
  end
end

local noneedtoreplace = {
  ["boxes.mp"] = true,
  --  ["format.mp"] = true,
  ["graph.mp"] = true,
  ["marith.mp"] = true,
  ["mfplain.mp"] = true,
  ["mpost.mp"] = true,
  ["plain.mp"] = true,
  ["rboxes.mp"] = true,
  ["sarith.mp"] = true,
  ["string.mp"] = true,
  ["TEX.mp"] = true,
  ["metafun.mp"] = true,
  ["metafun.mpiv"] = true,
  ["mp-abck.mpiv"] = true,
  ["mp-apos.mpiv"] = true,
  ["mp-asnc.mpiv"] = true,
  ["mp-bare.mpiv"] = true,
  ["mp-base.mpiv"] = true,
  ["mp-butt.mpiv"] = true,
  ["mp-char.mpiv"] = true,
  ["mp-chem.mpiv"] = true,
  ["mp-core.mpiv"] = true,
  ["mp-crop.mpiv"] = true,
  ["mp-figs.mpiv"] = true,
  ["mp-form.mpiv"] = true,
  ["mp-func.mpiv"] = true,
  ["mp-grap.mpiv"] = true,
  ["mp-grid.mpiv"] = true,
  ["mp-grph.mpiv"] = true,
  ["mp-idea.mpiv"] = true,
  ["mp-luas.mpiv"] = true,
  ["mp-mlib.mpiv"] = true,
  ["mp-page.mpiv"] = true,
  ["mp-shap.mpiv"] = true,
  ["mp-step.mpiv"] = true,
  ["mp-text.mpiv"] = true,
  ["mp-tool.mpiv"] = true,
}
luamplib.noneedtoreplace = noneedtoreplace

local function replaceformatmp(file,newfile,ofmodify)
  local fh = ioopen(file,"r")
  if not fh then return file end
  local data = fh:read("*all"); fh:close()
  fh = ioopen(newfile,"w")
  if not fh then return file end
  fh:write(
    "let normalinfont = infont;\n",
    "primarydef str infont name = rawtextext(str) enddef;\n",
    data,
    "vardef Fmant_(expr x) = rawtextext(decimal abs x) enddef;\n",
    "vardef Fexp_(expr x) = rawtextext(\"$^{\"&decimal x&\"}$\") enddef;\n",
    "let infont = normalinfont;\n"
  ); fh:close()
  lfstouch(newfile,currenttime,ofmodify)
  return newfile
end

local esctex  = "!!!T!!!E!!!X!!!"
local esclbr  = "!!!!!LEFTBRCE!!!!!"
local escrbr  = "!!!!!RGHTBRCE!!!!!"
local escpcnt = "!!!!!PERCENT!!!!!"
local eschash = "!!!!!HASH!!!!!"
local begname = "%f[A-Z_a-z]"
local endname = "%f[^A-Z_a-z]"

local btex_etex        = begname.."btex"..endname.."%s*(.-)%s*"..begname.."etex"..endname
local verbatimtex_etex = begname.."verbatimtex"..endname.."%s*(.-)%s*"..begname.."etex"..endname

local function protecttexcontents(str)
  return str:gsub("\\%%", "\\"..escpcnt)
            :gsub("%%.-\n", "")
            :gsub("%%.-$",  "")
            :gsub('"', '"&ditto&"')
            :gsub("\n%s*", " ")
            :gsub(escpcnt, "%%")
end

local function replaceinputmpfile (name,file)
  local ofmodify = lfsattributes(file,"modification")
  if not ofmodify then return file end
  local cachedir = luamplib.cachedir or outputdir
  local newfile = name:gsub("%W","_")
  newfile = cachedir .."/luamplib_input_"..newfile
  if newfile and luamplibtime then
    local nf = lfsattributes(newfile)
    if nf and nf.mode == "file" and ofmodify == nf.modification and luamplibtime < nf.access then
      return nf.size == 0 and file or newfile
    end
  end
  if name == "format.mp" then return replaceformatmp(file,newfile,ofmodify) end

  local fh = ioopen(file,"r")
  if not fh then return file end
  local data = fh:read("*all"); fh:close()

  local count,cnt = 0,0

  data = data:gsub("\"[^\n]-\"", function(str)
    return str:gsub("([bem])tex"..endname,"%1"..esctex)
  end)

  data, cnt = data:gsub(btex_etex, function(str)
    return format("rawtextext(\"%s\")",protecttexcontents(str))
  end)
  count = count + cnt
  data, cnt = data:gsub(verbatimtex_etex, "")
  count = count + cnt

  data = data:gsub("\"[^\n]-\"", function(str) -- restore string btex .. etex
    return str:gsub("([bem])"..esctex, "%1tex")
  end)

  if count == 0 then
    noneedtoreplace[name] = true
    fh = ioopen(newfile,"w");
    if fh then
      fh:close()
      lfstouch(newfile,currenttime,ofmodify)
    end
    return file
  end
  fh = ioopen(newfile,"w")
  if not fh then return file end
  fh:write(data); fh:close()
  lfstouch(newfile,currenttime,ofmodify)
  return newfile
end

local randomseed = nil

local mpkpse = kpse.new("luatex", "mpost")

local special_ftype = {
  pfb = "type1 fonts",
  enc = "enc files",
}

local function finder(name, mode, ftype)
  if mode == "w" then
    return name
  else
    ftype = special_ftype[ftype] or ftype
    local file = mpkpse:find_file(name,ftype)
    if file then
      if not lfstouch or ftype ~= "mp" or noneedtoreplace[name] then
        return file
      end
      return replaceinputmpfile(name,file)
    end
    return mpkpse:find_file(name,stringmatch(name,"[a-zA-Z]+$"))
  end
end
luamplib.finder = finder


function luamplib.resetlastlog()
  luamplib.lastlog = ""
end

local mplibone = tonumber(mplib.version()) <= 1.50

if mplibone then

  luamplib.make = luamplib.make or function(name,mem_name,dump)
    local t = os.clock()
    local mpx = mplib.new {
      ini_version = true,
      find_file = luamplib.finder,
      job_name = stripsuffix(name)
    }
    mpx:execute(format("input %s ;",name))
    if dump then
      mpx:execute("dump ;")
      info("format %s made and dumped for %s in %0.3f seconds",mem_name,name,os.clock()-t)
    else
      info("%s read in %0.3f seconds",name,os.clock()-t)
    end
    return mpx
  end

  function luamplib.load(name)
    local mem_name = replacesuffix(name,"mem")
    local mpx = mplib.new {
      ini_version = false,
      mem_name = mem_name,
      find_file = luamplib.finder
    }
    if not mpx and type(luamplib.make) == "function" then
      -- when i have time i'll locate the format and dump
      mpx = luamplib.make(name,mem_name)
    end
    if mpx then
      info("using format %s",mem_name,false)
      return mpx, nil
    else
      return nil, { status = 99, error = "out of memory or invalid format" }
    end
  end

else

  local preamble = [[
    boolean mplib ; mplib := true ;
    let dump = endinput ;
    let normalfontsize = fontsize;
    input %s ;
  ]]

  luamplib.make = luamplib.make or function()
  end

  function luamplib.load(name,verbatim)
    local mpx = mplib.new {
      ini_version = true,
      find_file = luamplib.finder,
      math_mode = luamplib.numbersystem,
      random_seed = randomseed,
    }
    local preamble = preamble .. (verbatim and "" or luamplib.mplibcodepreamble)
    if luamplib.textextlabel then
      preamble = preamble .. (verbatim and "" or luamplib.textextlabelpreamble)
    end
    local result
    if not mpx then
      result = { status = 99, error = "out of memory"}
    else
      result = mpx:execute(format(preamble, replacesuffix(name,"mp")))
    end
    luamplib.reporterror(result)
    return mpx, result
  end

end

local currentformat = "plain"

local function setformat (name) --- used in .sty
  currentformat = name
end
luamplib.setformat = setformat

luamplib.reporterror = function (result)
  if not result then
    err("no result object returned")
  else
    local t, e, l = result.term, result.error, result.log
    local log = stringgsub(t or l or "no-term","^%s+","\n")
    luamplib.lastlog = luamplib.lastlog .. "\n " .. (l or t or "no-log")
    if result.status > 0 then
      warn("%s",log)
      if result.status > 1 then
        err("%s",e or "see above messages")
      end
    end
    return log
  end
end

local function process_indeed (mpx, data, indeed)
  local converted, result = false, {}
  if mpx and data then
    result = mpx:execute(data)
    local log = luamplib.reporterror(result)
    if indeed and log then
      if luamplib.showlog then
        info("%s",luamplib.lastlog)
        luamplib.resetlastlog()
      elseif result.fig then
        if stringfind(log,"\n>>") then info("%s",log) end
        converted = luamplib.convert(result)
      else
        info("%s",log)
        warn("No figure output. Maybe no beginfig/endfig")
      end
    end
  else
    err("Mem file unloadable. Maybe generated with a different version of mplib?")
  end
  return converted, result
end

luamplib.codeinherit = false
local mplibinstances = {}
local process = function (data,indeed,verbatim)
  local standalone, firstpass = not luamplib.codeinherit, not indeed
  local currfmt = currentformat .. (luamplib.numbersystem or "scaled")
  currfmt = firstpass and currfmt or (currfmt.."2")
  local mpx = mplibinstances[currfmt]
  if standalone or not mpx then
    randomseed = firstpass and math.random(65535) or randomseed
    mpx = luamplib.load(currentformat,verbatim)
    mplibinstances[currfmt] = mpx
  end
  return process_indeed(mpx, data, indeed)
end
luamplib.process = process

local function getobjects(result,figure,f)
  return figure:objects()
end

local function convert(result, flusher)
  luamplib.flush(result, flusher)
  return true -- done
end
luamplib.convert = convert

local function pdf_startfigure(n,llx,lly,urx,ury)
  texsprint(format("\\mplibstarttoPDF{%f}{%f}{%f}{%f}",llx,lly,urx,ury))
end

local function pdf_stopfigure()
  texsprint("\\mplibstoptoPDF")
end

local function pdf_literalcode(fmt,...) -- table
  textprint({"\\mplibtoPDF{"},{-2,format(fmt,...)},{"}"})
end
luamplib.pdf_literalcode = pdf_literalcode

local function pdf_textfigure(font,size,text,width,height,depth)
  -- if text == "" then text = "\0" end -- char(0) has gone
  text = text:gsub(".",function(c)
    return format("\\hbox{\\char%i}",string.byte(c)) -- kerning happens in metapost
  end)
  texsprint(format("\\mplibtextext{%s}{%f}{%s}{%s}{%f}",font,size,text,0,-( 7200/ 7227)/65536*depth))
end
luamplib.pdf_textfigure = pdf_textfigure

local bend_tolerance = 131/65536

local rx, sx, sy, ry, tx, ty, divider = 1, 0, 0, 1, 0, 0, 1

local function pen_characteristics(object)
  local t = mplib.pen_info(object)
  rx, ry, sx, sy, tx, ty = t.rx, t.ry, t.sx, t.sy, t.tx, t.ty
  divider = sx*sy - rx*ry
  return not (sx==1 and rx==0 and ry==0 and sy==1 and tx==0 and ty==0), t.width
end

local function concat(px, py) -- no tx, ty here
  return (sy*px-ry*py)/divider,(sx*py-rx*px)/divider
end

local function curved(ith,pth)
  local d = pth.left_x - ith.right_x
  if abs(ith.right_x - ith.x_coord - d) <= bend_tolerance and abs(pth.x_coord - pth.left_x - d) <= bend_tolerance then
    d = pth.left_y - ith.right_y
    if abs(ith.right_y - ith.y_coord - d) <= bend_tolerance and abs(pth.y_coord - pth.left_y - d) <= bend_tolerance then
      return false
    end
  end
  return true
end

local function flushnormalpath(path,open)
  local pth, ith
  for i=1,#path do
    pth = path[i]
    if not ith then
      pdf_literalcode("%f %f m",pth.x_coord,pth.y_coord)
    elseif curved(ith,pth) then
      pdf_literalcode("%f %f %f %f %f %f c",ith.right_x,ith.right_y,pth.left_x,pth.left_y,pth.x_coord,pth.y_coord)
    else
      pdf_literalcode("%f %f l",pth.x_coord,pth.y_coord)
    end
    ith = pth
  end
  if not open then
    local one = path[1]
    if curved(pth,one) then
      pdf_literalcode("%f %f %f %f %f %f c",pth.right_x,pth.right_y,one.left_x,one.left_y,one.x_coord,one.y_coord )
    else
      pdf_literalcode("%f %f l",one.x_coord,one.y_coord)
    end
  elseif #path == 1 then
    -- special case .. draw point
    local one = path[1]
    pdf_literalcode("%f %f l",one.x_coord,one.y_coord)
  end
  return t
end

local function flushconcatpath(path,open)
  pdf_literalcode("%f %f %f %f %f %f cm", sx, rx, ry, sy, tx ,ty)
  local pth, ith
  for i=1,#path do
    pth = path[i]
    if not ith then
      pdf_literalcode("%f %f m",concat(pth.x_coord,pth.y_coord))
    elseif curved(ith,pth) then
      local a, b = concat(ith.right_x,ith.right_y)
      local c, d = concat(pth.left_x,pth.left_y)
      pdf_literalcode("%f %f %f %f %f %f c",a,b,c,d,concat(pth.x_coord, pth.y_coord))
    else
      pdf_literalcode("%f %f l",concat(pth.x_coord, pth.y_coord))
    end
    ith = pth
  end
  if not open then
    local one = path[1]
    if curved(pth,one) then
      local a, b = concat(pth.right_x,pth.right_y)
      local c, d = concat(one.left_x,one.left_y)
      pdf_literalcode("%f %f %f %f %f %f c",a,b,c,d,concat(one.x_coord, one.y_coord))
    else
      pdf_literalcode("%f %f l",concat(one.x_coord,one.y_coord))
    end
  elseif #path == 1 then
    -- special case .. draw point
    local one = path[1]
    pdf_literalcode("%f %f l",concat(one.x_coord,one.y_coord))
  end
  return t
end

local further_split_keys = {
  ["MPlibTEXboxID"] = true,
  ["sh_color_a"]    = true,
  ["sh_color_b"]    = true,
}

local function script2table(s)
  local t = {}
  for _,i in ipairs(stringexplode(s,"\13+")) do
    local k,v = stringmatch(i,"(.-)=(.*)") -- v may contain = or empty.
    if k and v and k ~= "" then
      if further_split_keys[k] then
        t[k] = stringexplode(v,":")
      else
        t[k] = v
      end
    end
  end
  return t
end

local mplibcodepreamble = [[
vardef rawtextext (expr t) =
  if unknown TEXBOX_:
    image( special "MPlibmkTEXbox="&t;
      addto currentpicture doublepath unitsquare; )
  else:
    TEXBOX_ := TEXBOX_ + 1;
    if known TEXBOX_wd_[TEXBOX_]:
      image ( addto currentpicture doublepath unitsquare
        xscaled TEXBOX_wd_[TEXBOX_]
        yscaled (TEXBOX_ht_[TEXBOX_] + TEXBOX_dp_[TEXBOX_])
        shifted (0, -TEXBOX_dp_[TEXBOX_])
        withprescript "MPlibTEXboxID=" &
          decimal TEXBOX_ & ":" &
          decimal TEXBOX_wd_[TEXBOX_] & ":" &
          decimal(TEXBOX_ht_[TEXBOX_]+TEXBOX_dp_[TEXBOX_]); )
    else:
      image( special "MPlibTEXError=1"; )
    fi
  fi
enddef;
if known context_mlib:
  defaultfont := "cmtt10";
  let infont = normalinfont;
  let fontsize = normalfontsize;
  vardef thelabel@#(expr p,z) =
    if string p :
      thelabel@#(p infont defaultfont scaled defaultscale,z)
    else :
      p shifted (z + labeloffset*mfun_laboff@# -
        (mfun_labxf@#*lrcorner p + mfun_labyf@#*ulcorner p +
        (1-mfun_labxf@#-mfun_labyf@#)*llcorner p))
    fi
  enddef;
  def graphictext primary filename =
    if (readfrom filename = EOF):
      errmessage "Please prepare '"&filename&"' in advance with"&
      " 'pstoedit -ssp -dt -f mpost yourfile.ps "&filename&"'";
    fi
    closefrom filename;
    def data_mpy_file = filename enddef;
    mfun_do_graphic_text (filename)
  enddef;
  if unknown TEXBOX_: def mfun_do_graphic_text text t = enddef; fi
else:
  vardef textext@# (text t) = rawtextext (t) enddef;
fi
def externalfigure primary filename =
  draw rawtextext("\includegraphics{"& filename &"}")
enddef;
def TEX = textext enddef;
def specialVerbatimTeX (text t) = special "MPlibVerbTeX="&t; enddef;
def normalVerbatimTeX  (text t) = special "PostMPlibVerbTeX="&t; enddef;
let VerbatimTeX = specialVerbatimTeX;
extra_beginfig := extra_beginfig & " let VerbatimTeX = normalVerbatimTeX;" ;
extra_endfig   := extra_endfig   & " let VerbatimTeX = specialVerbatimTeX;" ;
]]
luamplib.mplibcodepreamble = mplibcodepreamble

local textextlabelpreamble = [[
primarydef s infont f = rawtextext(s) enddef;
def fontsize expr f =
  begingroup
  save size,pic; numeric size; picture pic;
  pic := rawtextext("\hskip\pdffontsize\font");
  size := xpart urcorner pic - xpart llcorner pic;
  if size = 0: 10pt else: size fi
  endgroup
enddef;
]]
luamplib.textextlabelpreamble = textextlabelpreamble

local TeX_code_t = {}

local function domakeTEXboxes (data)
  local num = 255 -- output box
  if data and data.fig then
    local figures = data.fig
    for f=1, #figures do
      TeX_code_t[f] = nil
      local figure = figures[f]
      local objects = getobjects(data,figure,f)
      if objects then
        for o=1,#objects do
          local object    = objects[o]
          local prescript = object.prescript
          prescript = prescript and script2table(prescript)
          local str = prescript and prescript.MPlibmkTEXbox
          if str then
            num = num + 1
            texsprint(format("\\setbox%i\\hbox{%s}",num,str))
          end
          local texcode = prescript and prescript.MPlibVerbTeX
          if texcode and texcode ~= "" then
            TeX_code_t[f] = texcode
          end
        end
      end
    end
  end
end

local function protect_tex_text_common (data)
  local everymplib    = texgettoks('everymplibtoks')    or ''
  local everyendmplib = texgettoks('everyendmplibtoks') or ''
  data = format("\n%s\n%s\n%s",everymplib, data, everyendmplib)
  data = data:gsub("\r","\n")

  data = data:gsub("\"[^\n]-\"", function(str)
    return str:gsub("([bem])tex"..endname,"%1"..esctex)
  end)

  data = data:gsub(btex_etex, function(str)
    return format("rawtextext(\"%s\")",protecttexcontents(str))
  end)
  data = data:gsub(verbatimtex_etex, function(str)
    return format("VerbatimTeX(\"%s\")",protecttexcontents(str))
  end)

  return data
end

local function protecttextextVerbatim(data)
  data = protect_tex_text_common(data)

  data = data:gsub("\"[^\n]-\"", function(str) -- restore string btex .. etex
    return str:gsub("([bem])"..esctex, "%1tex")
  end)

  local _,result = process(data, false)
  domakeTEXboxes(result)
  return data
end

luamplib.protecttextextVerbatim = protecttextextVerbatim

luamplib.mpxcolors = {}

local function protecttextext(data)
  data = protect_tex_text_common(data)

  data = data:gsub("\"[^\n]-\"", function(str)
    str = str:gsub("([bem])"..esctex, "%1tex")
             :gsub("%%", escpcnt)
             :gsub("{",  esclbr)
             :gsub("}",  escrbr)
             :gsub("#",  eschash)
    return format("\\detokenize{%s}",str)
  end)

  data = data:gsub("%%.-\n", "")

  local grouplevel = tex.currentgrouplevel
  luamplib.mpxcolors[grouplevel] = {}
  data = data:gsub("\\mpcolor"..endname.."(.-){(.-)}", function(opt,str)
    local cnt = #luamplib.mpxcolors[grouplevel] + 1
    luamplib.mpxcolors[grouplevel][cnt] = format(
      "\\expandafter\\mplibcolor\\csname mpxcolor%i:%i\\endcsname%s{%s}",
      grouplevel,cnt,opt,str)
    return format("\\csname mpxcolor%i:%i\\endcsname",grouplevel,cnt)
  end)

  data = data:gsub("([^`\\])#","%1##")

  texsprint(data)
end

luamplib.protecttextext = protecttextext

local function makeTEXboxes (data)
  data = data:gsub("##","#")
             :gsub(escpcnt,"%%")
             :gsub(esclbr,"{")
             :gsub(escrbr,"}")
             :gsub(eschash,"#")
  local _,result = process(data, false)
  domakeTEXboxes(result)
  return data
end

luamplib.makeTEXboxes = makeTEXboxes

local factor = 65536*(7227/7200)

local function processwithTEXboxes (data)
  if not data then return end
  local num = 255 -- output box
  local prepreamble = format("TEXBOX_:=%i;\n",num)
  while true do
    num = num + 1
    local box = texgetbox(num)
    if not box then break end
    prepreamble = format(
      "%sTEXBOX_wd_[%i]:=%f;\nTEXBOX_ht_[%i]:=%f;\nTEXBOX_dp_[%i]:=%f;\n",
      prepreamble,
      num, box.width /factor,
      num, box.height/factor,
      num, box.depth /factor)
  end
  process(prepreamble .. data, true)
end
luamplib.processwithTEXboxes = processwithTEXboxes

local pdfoutput = tonumber(texget("outputmode")) or tonumber(texget("pdfoutput"))
local pdfmode = pdfoutput > 0

local function start_pdf_code()
  if pdfmode then
    pdf_literalcode("q")
  else
    texsprint("\\special{pdf:bcontent}") -- dvipdfmx
  end
end
local function stop_pdf_code()
  if pdfmode then
    pdf_literalcode("Q")
  else
    texsprint("\\special{pdf:econtent}") -- dvipdfmx
  end
end

local function putTEXboxes (object,prescript)
  local box = prescript.MPlibTEXboxID
  local n,tw,th = box[1],tonumber(box[2]),tonumber(box[3])
  if n and tw and th then
    local op = object.path
    local first, second, fourth = op[1], op[2], op[4]
    local tx, ty = first.x_coord, first.y_coord
    local sx, rx, ry, sy = 1, 0, 0, 1
    if tw ~= 0 then
      sx = (second.x_coord - tx)/tw
      rx = (second.y_coord - ty)/tw
      if sx == 0 then sx = 0.00001 end
    end
    if th ~= 0 then
      sy = (fourth.y_coord - ty)/th
      ry = (fourth.x_coord - tx)/th
      if sy == 0 then sy = 0.00001 end
    end
    start_pdf_code()
    pdf_literalcode("%f %f %f %f %f %f cm",sx,rx,ry,sy,tx,ty)
    texsprint(format("\\mplibputtextbox{%i}",n))
    stop_pdf_code()
  end
end

local pdf_objs = {}
local token, getpageres, setpageres = newtoken or token
local pgf = { bye = "pgfutil@everybye", extgs = "pgf@sys@addpdfresource@extgs@plain" }

if pdfmode then -- repect luaotfload-colors
  getpageres = pdf.getpageresources or function() return pdf.pageresources end
  setpageres = pdf.setpageresources or function(s) pdf.pageresources = s end
else
  texsprint("\\special{pdf:obj @MPlibTr<<>>}",
            "\\special{pdf:obj @MPlibSh<<>>}")
end

-- objstr <string> => obj <number>, new <boolean>
local function update_pdfobjs (os)
  local on = pdf_objs[os]
  if on then
    return on,false
  end
  if pdfmode then
    on = pdf.immediateobj(os)
  else
    on = pdf_objs.cnt or 0
    pdf_objs.cnt = on + 1
  end
  pdf_objs[os] = on
  return on,true
end

local transparancy_modes = { [0] = "Normal",
  "Normal",       "Multiply",     "Screen",       "Overlay",
  "SoftLight",    "HardLight",    "ColorDodge",   "ColorBurn",
  "Darken",       "Lighten",      "Difference",   "Exclusion",
  "Hue",          "Saturation",   "Color",        "Luminosity",
  "Compatible",
}

local function update_tr_res(res,mode,opaq)
  local os = format("<</BM /%s/ca %.3f/CA %.3f/AIS false>>",mode,opaq,opaq)
  local on, new = update_pdfobjs(os)
  if new then
    if pdfmode then
      res = format("%s/MPlibTr%i %i 0 R",res,on,on)
    else
      if pgf.loaded then
        texsprint(format("\\csname %s\\endcsname{/MPlibTr%i%s}", pgf.extgs, on, os))
      else
        texsprint(format("\\special{pdf:put @MPlibTr<</MPlibTr%i%s>>}",on,os))
      end
    end
  end
  return res,on
end

local function tr_pdf_pageresources(mode,opaq)
  if token and pgf.bye and not pgf.loaded then
    pgf.loaded = token.create(pgf.bye).cmdname == "assign_toks"
    pgf.bye    = pgf.loaded and pgf.bye
  end
  local res, on_on, off_on = "", nil, nil
  res, off_on = update_tr_res(res, "Normal", 1)
  res, on_on  = update_tr_res(res, mode, opaq)
  if pdfmode then
    if res ~= "" then
      if pgf.loaded then
        texsprint(format("\\csname %s\\endcsname{%s}", pgf.extgs, res))
      else
        local tpr, n = getpageres() or "", 0
        tpr, n = tpr:gsub("/ExtGState<<", "%1"..res)
        if n == 0 then
          tpr = format("%s/ExtGState<<%s>>", tpr, res)
        end
        setpageres(tpr)
      end
    end
  else
    if not pgf.loaded then
      texsprint(format("\\special{pdf:put @resources<</ExtGState @MPlibTr>>}"))
    end
  end
  return on_on, off_on
end

local shading_res

local function shading_initialize ()
  shading_res = {}
  if pdfmode and luatexbase.callbacktypes and luatexbase.callbacktypes.finish_pdffile then -- ltluatex
    local shading_obj = pdf.reserveobj()
    setpageres(format("%s/Shading %i 0 R",getpageres() or "",shading_obj))
    luatexbase.add_to_callback("finish_pdffile", function()
      pdf.immediateobj(shading_obj,format("<<%s>>",tableconcat(shading_res)))
      end, "luamplib.finish_pdffile")
    pdf_objs.finishpdf = true
  end
end

local function sh_pdfpageresources(shtype,domain,colorspace,colora,colorb,coordinates)
  if not shading_res then shading_initialize() end
  local os = format("<</FunctionType 2/Domain [ %s ]/C0 [ %s ]/C1 [ %s ]/N 1>>",
                    domain, colora, colorb)
  local funcobj = pdfmode and format("%i 0 R",update_pdfobjs(os)) or os
  os = format("<</ShadingType %i/ColorSpace /%s/Function %s/Coords [ %s ]/Extend [ true true ]/AntiAlias true>>",
              shtype, colorspace, funcobj, coordinates)
  local on, new = update_pdfobjs(os)
  if pdfmode then
    if new then
      local res = format("/MPlibSh%i %i 0 R", on, on)
      if pdf_objs.finishpdf then
        shading_res[#shading_res+1] = res
      else
        local pageres = getpageres() or ""
        if not stringfind(pageres,"/Shading<<.*>>") then
          pageres = pageres.."/Shading<<>>"
        end
        pageres = pageres:gsub("/Shading<<","%1"..res)
        setpageres(pageres)
      end
    end
  else
    if new then
      texsprint(format("\\special{pdf:put @MPlibSh<</MPlibSh%i%s>>}",on,os))
    end
    texsprint(format("\\special{pdf:put @resources<</Shading @MPlibSh>>}"))
  end
  return on
end

local function color_normalize(ca,cb)
  if #cb == 1 then
    if #ca == 4 then
      cb[1], cb[2], cb[3], cb[4] = 0, 0, 0, 1-cb[1]
    else -- #ca = 3
      cb[1], cb[2], cb[3] = cb[1], cb[1], cb[1]
    end
  elseif #cb == 3 then -- #ca == 4
    cb[1], cb[2], cb[3], cb[4] = 1-cb[1], 1-cb[2], 1-cb[3], 0
  end
end

local prev_override_color

local function do_preobj_color(object,prescript)
  -- transparency
  local opaq = prescript and prescript.tr_transparency
  local tron_no, troff_no
  if opaq then
    local mode = prescript.tr_alternative or 1
    mode = transparancy_modes[tonumber(mode)]
    tron_no, troff_no = tr_pdf_pageresources(mode,opaq)
    pdf_literalcode("/MPlibTr%i gs",tron_no)
  end
  -- color
  local override = prescript and prescript.MPlibOverrideColor
  if override then
    if pdfmode then
      pdf_literalcode(override)
      override = nil
    else
      texsprint(format("\\special{color push %s}",override))
      prev_override_color = override
    end
  else
    local cs = object.color
    if cs and #cs > 0 then
      pdf_literalcode(luamplib.colorconverter(cs))
      prev_override_color = nil
    elseif not pdfmode then
      override = prev_override_color
      if override then
        texsprint(format("\\special{color push %s}",override))
      end
    end
  end
  -- shading
  local sh_type = prescript and prescript.sh_type
  if sh_type then
    local domain  = prescript.sh_domain
    local centera = stringexplode(prescript.sh_center_a)
    local centerb = stringexplode(prescript.sh_center_b)
    for _,t in pairs({centera,centerb}) do
      for i,v in ipairs(t) do
        t[i] = format("%f",v)
      end
    end
    centera = tableconcat(centera," ")
    centerb = tableconcat(centerb," ")
    local colora  = prescript.sh_color_a or {0};
    local colorb  = prescript.sh_color_b or {1};
    for _,t in pairs({colora,colorb}) do
      for i,v in ipairs(t) do
        t[i] = format("%.3f",v)
      end
    end
    if #colora > #colorb then
      color_normalize(colora,colorb)
    elseif #colorb > #colora then
      color_normalize(colorb,colora)
    end
    local colorspace
    if     #colorb == 1 then colorspace = "DeviceGray"
    elseif #colorb == 3 then colorspace = "DeviceRGB"
    elseif #colorb == 4 then colorspace = "DeviceCMYK"
    else   return troff_no,override
    end
    colora = tableconcat(colora, " ")
    colorb = tableconcat(colorb, " ")
    local shade_no
    if sh_type == "linear" then
      local coordinates = tableconcat({centera,centerb}," ")
      shade_no = sh_pdfpageresources(2,domain,colorspace,colora,colorb,coordinates)
    elseif sh_type == "circular" then
      local radiusa = format("%f",prescript.sh_radius_a)
      local radiusb = format("%f",prescript.sh_radius_b)
      local coordinates = tableconcat({centera,radiusa,centerb,radiusb}," ")
      shade_no = sh_pdfpageresources(3,domain,colorspace,colora,colorb,coordinates)
    end
    pdf_literalcode("q /Pattern cs")
    return troff_no,override,shade_no
  end
  return troff_no,override
end

local function do_postobj_color(tr,over,sh)
  if sh then
    pdf_literalcode("W n /MPlibSh%s sh Q",sh)
  end
  if over then
    texsprint("\\special{color pop}")
  end
  if tr then
    pdf_literalcode("/MPlibTr%i gs",tr)
  end
end


local function flush(result,flusher)
  if result then
    local figures = result.fig
    if figures then
      for f=1, #figures do
        info("flushing figure %s",f)
        local figure = figures[f]
        local objects = getobjects(result,figure,f)
        local fignum = tonumber(stringmatch(figure:filename(),"([%d]+)$") or figure:charcode() or 0)
        local miterlimit, linecap, linejoin, dashed = -1, -1, -1, false
        local bbox = figure:boundingbox()
        local llx, lly, urx, ury = bbox[1], bbox[2], bbox[3], bbox[4] -- faster than unpack
        if urx < llx then
          -- invalid
          pdf_startfigure(fignum,0,0,0,0)
          pdf_stopfigure()
        else
          if TeX_code_t[f] then
            texsprint(TeX_code_t[f])
          end
          local TeX_code_bot = {} -- PostVerbatimTeX
          pdf_startfigure(fignum,llx,lly,urx,ury)
          start_pdf_code()
          if objects then
            for o=1,#objects do
              local object        = objects[o]
              local objecttype    = object.type
              local prescript     = object.prescript
              prescript = prescript and script2table(prescript) -- prescript is now a table
              local tr_opaq,cr_over,shade_no = do_preobj_color(object,prescript)
              if prescript and prescript.MPlibTEXboxID then
                putTEXboxes(object,prescript)
              elseif prescript and prescript.PostMPlibVerbTeX then
                TeX_code_bot[#TeX_code_bot+1] = prescript.PostMPlibVerbTeX
              elseif objecttype == "start_bounds" or objecttype == "stop_bounds" then
                -- skip
              elseif objecttype == "start_clip" then
                start_pdf_code()
                flushnormalpath(object.path,t,false)
                pdf_literalcode("W n")
              elseif objecttype == "stop_clip" then
                stop_pdf_code()
                miterlimit, linecap, linejoin, dashed = -1, -1, -1, false
              elseif objecttype == "special" then
                -- not supported
                if prescript and prescript.MPlibTEXError then
                  warn("textext() anomaly. Try disabling \\mplibtextextlabel.")
                end
              elseif objecttype == "text" then
                local ot = object.transform -- 3,4,5,6,1,2
                start_pdf_code()
                pdf_literalcode("%f %f %f %f %f %f cm",ot[3],ot[4],ot[5],ot[6],ot[1],ot[2])
                pdf_textfigure(object.font,object.dsize,object.text,object.width,object.height,object.depth)
                stop_pdf_code()
              else
                local ml = object.miterlimit
                if ml and ml ~= miterlimit then
                  miterlimit = ml
                  pdf_literalcode("%f M",ml)
                end
                local lj = object.linejoin
                if lj and lj ~= linejoin then
                  linejoin = lj
                  pdf_literalcode("%i j",lj)
                end
                local lc = object.linecap
                if lc and lc ~= linecap then
                  linecap = lc
                  pdf_literalcode("%i J",lc)
                end
                local dl = object.dash
                if dl then
                  local d = format("[%s] %i d",tableconcat(dl.dashes or {}," "),dl.offset)
                  if d ~= dashed then
                    dashed = d
                    pdf_literalcode(dashed)
                  end
                elseif dashed then
                  pdf_literalcode("[] 0 d")
                  dashed = false
                end
                local path = object.path
                local transformed, penwidth = false, 1
                local open = path and path[1].left_type and path[#path].right_type
                local pen = object.pen
                if pen then
                  if pen.type == 'elliptical' then
                    transformed, penwidth = pen_characteristics(object) -- boolean, value
                    pdf_literalcode("%f w",penwidth)
                    if objecttype == 'fill' then
                      objecttype = 'both'
                    end
                  else -- calculated by mplib itself
                    objecttype = 'fill'
                  end
                end
                if transformed then
                  start_pdf_code()
                end
                if path then
                  if transformed then
                    flushconcatpath(path,open)
                  else
                    flushnormalpath(path,open)
                  end
                  if not shade_no then ----- conflict with shading
                    if objecttype == "fill" then
                      pdf_literalcode("h f")
                    elseif objecttype == "outline" then
                      pdf_literalcode((open and "S") or "h S")
                    elseif objecttype == "both" then
                      pdf_literalcode("h B")
                    end
                  end
                end
                if transformed then
                  stop_pdf_code()
                end
                local path = object.htap
                if path then
                  if transformed then
                    start_pdf_code()
                  end
                  if transformed then
                    flushconcatpath(path,open)
                  else
                    flushnormalpath(path,open)
                  end
                  if objecttype == "fill" then
                    pdf_literalcode("h f")
                  elseif objecttype == "outline" then
                    pdf_literalcode((open and "S") or "h S")
                  elseif objecttype == "both" then
                    pdf_literalcode("h B")
                  end
                  if transformed then
                    stop_pdf_code()
                  end
                end
--                if cr then
--                  pdf_literalcode(cr)
--                end
              end
              do_postobj_color(tr_opaq,cr_over,shade_no)
            end
          end
          stop_pdf_code()
          pdf_stopfigure()
          if #TeX_code_bot > 0 then
            texsprint(TeX_code_bot)
          end
        end
      end
    end
  end
end
luamplib.flush = flush

local function colorconverter(cr)
  local n = #cr
  if n == 4 then
    local c, m, y, k = cr[1], cr[2], cr[3], cr[4]
    return format("%.3f %.3f %.3f %.3f k %.3f %.3f %.3f %.3f K",c,m,y,k,c,m,y,k), "0 g 0 G"
  elseif n == 3 then
    local r, g, b = cr[1], cr[2], cr[3]
    return format("%.3f %.3f %.3f rg %.3f %.3f %.3f RG",r,g,b,r,g,b), "0 g 0 G"
  else
    local s = cr[1]
    return format("%.3f g %.3f G",s,s), "0 g 0 G"
  end
end
luamplib.colorconverter = colorconverter
-- 
--  End of File `luamplib.lua'.
