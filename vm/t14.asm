        jump.i  #lab0                   ;jump.i  lab0
lab0:
lab2:
        jl.i    0,4,#lab3               ;jl.i    i,j,lab3
        mov.i   #0,36                   ;mov.i   0,$t0
        jump.i  #lab4                   ;jump.i  lab4
lab3:
        mov.i   #1,36                   ;mov.i   1,$t0
lab4:
        je.i    36,#0,#lab5             ;je.i    $t0,0,lab5
        mov.i   #0,40                   ;mov.i   0,$t1
        jump.i  #lab6                   ;jump.i  lab6
lab5:
        mov.i   #1,40                   ;mov.i   1,$t1
lab6:
        jg.i    8,4,#lab7               ;jg.i    k,j,lab7
        mov.i   #0,44                   ;mov.i   0,$t2
        jump.i  #lab8                   ;jump.i  lab8
lab7:
        mov.i   #1,44                   ;mov.i   1,$t2
lab8:
        or.i    40,44,48                ;or.i    $t1,$t2,$t3
        je.i    48,#0,#lab1             ;je.i    $t3,0,lab1
        add.r   12,#1.35,52             ;add.r   x,1.35,$t4
        mov.r   52,12                   ;mov.r   $t4,x
        jump.i  #lab2                   ;jump.i  lab2
lab1:
        exit                            ;exit    
