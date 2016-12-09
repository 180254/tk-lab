        jump.i  #lab0                   ;jump.i  lab0
lab0:
        jl.i    0,4,#lab1               ;jl.i    i,j,lab1
        mov.i   #0,36                   ;mov.i   0,$t0
        jump.i  #lab2                   ;jump.i  lab2
lab1:
        mov.i   #1,36                   ;mov.i   1,$t0
lab2:
        jl.i    4,8,#lab3               ;jl.i    j,k,lab3
        mov.i   #0,40                   ;mov.i   0,$t1
        jump.i  #lab4                   ;jump.i  lab4
lab3:
        mov.i   #1,40                   ;mov.i   1,$t1
lab4:
        and.i   36,40,44                ;and.i   $t0,$t1,$t2
        je.i    44,#0,#lab5             ;je.i    $t2,0,lab5
        mov.r   #0,12                   ;mov.r   0,x
        jump.i  #lab6                   ;jump.i  lab6
lab5:
        mov.r   #1,12                   ;mov.r   1,x
lab6:
        exit                            ;exit    
