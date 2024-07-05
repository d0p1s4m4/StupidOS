klog_print_date:
	clc

	ret

macro KLOG_INIT {
}

macro KLOG msg {
	out 0xe9, al
}
