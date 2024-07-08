macro MODULE_INFO_BEGIN { 
	section '.modinfo' data
}

macro MODULE_INFO tag,value {
	public module_#tag
module_#tag db `value#, 0

}

macro MODULE_AUTHOR value {
	MODULE_INFO author, value
}

macro MODULE_VERSION value {
	MODULE_INFO version, value
}

macro MODULE_LICENSE value {
	MODULE_INFO license, value
}

macro MODULE_DESCRIPTION value {
	MODULE_INFO description, value
}
