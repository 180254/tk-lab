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
        jl.i    0,8,#lab5               ;jl.i    i,k,lab5
        mov.i   #0,44                   ;mov.i   0,$t2
        jump.i  #lab6                   ;jump.i  lab6
lab5:
        mov.i   #1,44                   ;mov.i   1,$t2
lab6:
        or.i    40,44,48                ;or.i    $t1,$t2,$t3
        and.i   36,48,52                ;and.i   $t0,$t3,$t4
        je.i    52,#0,#lab7             ;je.i    $t4,0,lab7
        mov.r   #0,12                   ;mov.r   0,x
        jump.i  #lab8                   ;jump.i  lab8
lab7:
        mov.r   #1,12                   ;mov.r   1,x
lab8:
        exit                            ;exit    
