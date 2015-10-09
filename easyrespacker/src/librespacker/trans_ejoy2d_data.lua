static const char* trans_ejoy2d_data = STRINGIFY(

local all = {}

function trans2table(filename)
	f = assert(loadfile(filename , "t"))
	return f()  
end

);