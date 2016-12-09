        jump.i  #lab0                   ;jump.i  lab0
lab0:
        inttoreal.i 4,24                ;inttoreal.i y,$t0
        mul.r   24,16,32                ;mul.r   $t0,h,$t1
        inttoreal.i 0,40                ;inttoreal.i x,$t2
        add.r   40,32,48                ;add.r   $t2,$t1,$t3
        mov.r   48,8                    ;mov.r   $t3,g
        write.r 8                       ;write.r g
        exit                            ;exit    
