local lfs = require "lfs"
local lpeg = require "lpeg"
local P, C, S = lpeg.P, lpeg.C, lpeg.S
local execute = os.execute


local patt = P"TexturePacker:: warning: " * C((1-S("\r\n"))^1)
local TMP_FILE = ".\\ui_output\\texture_pack.log"


---------------------------------------------------------
local function run_cmd(cmd)
    print(">>>run: ", cmd)
    execute(cmd)
end

local function _is_dir(dir)
    local attr = lfs.attributes(dir)
    return not not (attr and attr.mode == "directory")
end

local function _is_file(file)
    local attr = lfs.attributes(file)
    return not not (attr and attr.mode == "file")
end

local function check_have_file(dir)
    if _is_dir(dir) then
        for file in lfs.dir(dir) do
            if file ~= "." and file ~= ".." then
                local attr = lfs.attributes(dir.."\\"..file)
                if attr.mode == "file" then return true end
            end
        end
    end
    return false
end

local function del_file(file)
    if not _is_file(file) then return end
    return run_cmd("del /Q/F "..file)
end

local function del_dir(dir)
    if not _is_dir(dir) then return end
    return run_cmd("RD /Q/S "..dir)
end

local function mkdir(dir)
    if _is_dir(dir) then return end
    return run_cmd(('MKDIR "%s"'):format(dir))
end

---------------------------------------------------------
local function _join(...)
    local ret = {}
    local args = {...}
    for i=1,#args do
        ret[#ret + 1] = args[i]
    end
    return table.concat(ret, " ", args[i])
end


local function _get_log_out()
    local ret = {}

    local handle = io.open(TMP_FILE, "r")
    for line in handle:lines() do
        local file = lpeg.match(patt, line)
        if file then
            ret[#ret+1] = file
        end
    end
    handle:close()
    return ret
end

local function _gen_tmp(dir)
    if _is_dir(dir) then
        run_cmd(('RD /Q/S "%s"'):format(dir))
    end
    mkdir(dir)
end

local function pack(in_pic_dir, out_file_prefix)
    local out_png_file = out_file_prefix..".png"
    local out_json_file = out_file_prefix..".json"

    local cmd = _join(
        'TexturePacker',
         '--algorithm MaxRects',
        '--maxrects-heuristics Best',
        '--pack-mode Best',
        '--premultiply-alpha',
        '--sheet '..out_png_file,
        '--texture-format png',
        '--extrude 1',
        '--data '..out_json_file,
        ('--format json-array "%s"'):format(in_pic_dir),
        '2> '..TMP_FILE)

    del_file(TMP_FILE)
    run_cmd(cmd)

    -- 获取当前无法加载进去的文件
    local warning_files = _get_log_out()
    if #warning_files>0 then
        assert(in_pic_dir ~= 'tmp1' or in_pic_dir ~= 'tmp2')
        _gen_tmp('tmp1')
        _gen_tmp('tmp2')
        run_cmd(('COPY /Y %s\\*.png "%s\\" '):format(in_pic_dir, 'tmp1') )
        for i=1,#warning_files do
            local wf = warning_files[i]
            run_cmd(('MOVE "%s\\%s" "%s\\"'):format('tmp1', wf, 'tmp2'))
        end

        --  删除文件
        del_file(out_png_file)
        del_file(out_json_file)

        -- 再次打包
        pack('tmp1', out_file_prefix.."1")
        pack('tmp2', out_file_prefix.."2")
    end
end


return {
    --- file system
    run_cmd = run_cmd,
    del_dir = del_dir,
    del_file = del_file,
    tmp_dir = _gen_tmp,
    mkdir = mkdir,
    check_have_file = check_have_file,
    join = _join,
    -- pack
    pack = pack,
}


--- test
-- pack("..\\sg_ui\\common\\png", ".\\ui_output\\test_tp")
-- print(_is_file(TMP_FILE))
