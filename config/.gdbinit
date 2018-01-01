set pagination off
set print repeats 0
set print elements 4096
define comm_find
find /w1 0x20000000, +12000, 0x87654321
set $comm_ptr=$_
p/a $comm_ptr
set $buf_len=( $comm_ptr + 4 )
set $buf_ptr=( $comm_ptr + 8 )
p/a $buf_len
p/a $buf_ptr
end

define comm_get_data
set $a = {int*}$buf_len
if $a != 0
    p/u $a
    p (char*)$buf_ptr

    if 1
    if $a <= 4
       p/d *(int*)$buf_ptr@1
    else
    if $a <= 8
       p/d *(int*)$buf_ptr@2
    else
    if $a <= 16
       p/d *(int*)$buf_ptr@4
    else
    if $a <= 32
       p/d *(int*)$buf_ptr@8
    else
    if $a <= 64
       p/d *(int*)$buf_ptr@16
    else
    if $a <= 128
       p/d *(int*)$buf_ptr@32
    else
    if $a <= 256
       p/d *(int*)$buf_ptr@64
    else
    if $a <= 512
       p/d *(int*)$buf_ptr@128
    else
    if $a <= 1024
       p/d *(int*)$buf_ptr@256
    else
    if $a <= 2048
       p/d *(int*)$buf_ptr@512
    else
    if $a <= 4096
       p/d *(int*)$buf_ptr@1024
    else
       #p/d *(int*)$buf_ptr@2048
    end
    end
    end
    end
    end
    end
    end
    end
    end
    end
    end
    end

    set {int*}$buf_len = 0
end
end

define comm_loop_data
while 1
   comm_get_data
   shell sleep 0.01
end
end

set confirm off
target remote localhost:3333
comm_find
comm_loop_data
quit

