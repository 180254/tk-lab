        jump.i  #lab0                   ;jump.i  lab0
czytajtab:
        enter.i #20                     ;enter.i 20
        mov.i   #1,BP-4                 ;mov.i   1,i
lab2:
        jl.i    BP-4,#11,#lab3          ;jl.i    i,11,lab3
        mov.i   #0,BP-8                 ;mov.i   0,$t0
        jump.i  #lab4                   ;jump.i  lab4
lab3:
        mov.i   #1,BP-8                 ;mov.i   1,$t0
lab4:
        je.i    BP-8,#0,#lab1           ;je.i    $t0,0,lab1
        sub.i   BP-4,#1,BP-12           ;sub.i   i,1,$t1
        mul.i   BP-12,#4,BP-12          ;mul.i   $t1,4,$t1
        add.i   BP+8,BP-12,BP-16        ;add.i   &a,$t1,$t2
        read.i  *BP-16                  ;read.i  $t2
        add.i   BP-4,#1,BP-20           ;add.i   i,1,$t3
        mov.i   BP-20,BP-4              ;mov.i   $t3,i
        jump.i  #lab2                   ;jump.i  lab2
lab1:
        leave                           ;leave   
        return                          ;return  
bubblesort:
        enter.i #100                    ;enter.i 100
        mov.i   #1,BP-4                 ;mov.i   1,i
lab6:
        jl.i    BP-4,#11,#lab7          ;jl.i    i,11,lab7
        mov.i   #0,BP-20                ;mov.i   0,$t4
        jump.i  #lab8                   ;jump.i  lab8
lab7:
        mov.i   #1,BP-20                ;mov.i   1,$t4
lab8:
        je.i    BP-20,#0,#lab5          ;je.i    $t4,0,lab5
        add.i   BP-4,#1,BP-24           ;add.i   i,1,$t5
        mov.i   BP-24,BP-8              ;mov.i   $t5,j
lab10:
        jl.i    BP-8,#11,#lab11         ;jl.i    j,11,lab11
        mov.i   #0,BP-28                ;mov.i   0,$t6
        jump.i  #lab12                  ;jump.i  lab12
lab11:
        mov.i   #1,BP-28                ;mov.i   1,$t6
lab12:
        je.i    BP-28,#0,#lab9          ;je.i    $t6,0,lab9
        sub.i   BP-4,#1,BP-32           ;sub.i   i,1,$t7
        mul.i   BP-32,#4,BP-32          ;mul.i   $t7,4,$t7
        add.i   BP+8,BP-32,BP-36        ;add.i   &a,$t7,$t8
        sub.i   BP-8,#1,BP-40           ;sub.i   j,1,$t9
        mul.i   BP-40,#4,BP-40          ;mul.i   $t9,4,$t9
        add.i   BP+8,BP-40,BP-44        ;add.i   &a,$t9,$t10
        jg.i    *BP-36,*BP-44,#lab13    ;jg.i    $t8,$t10,lab13
        mov.i   #0,BP-48                ;mov.i   0,$t11
        jump.i  #lab14                  ;jump.i  lab14
lab13:
        mov.i   #1,BP-48                ;mov.i   1,$t11
lab14:
        je.i    BP-48,#0,#lab15         ;je.i    $t11,0,lab15
        sub.i   BP-4,#1,BP-52           ;sub.i   i,1,$t12
        mul.i   BP-52,#4,BP-52          ;mul.i   $t12,4,$t12
        add.i   BP+8,BP-52,BP-56        ;add.i   &a,$t12,$t13
        inttoreal.i *BP-56,BP-64        ;inttoreal.i $t13,$t14
        mov.r   BP-64,BP-16             ;mov.r   $t14,tmp
        sub.i   BP-4,#1,BP-68           ;sub.i   i,1,$t15
        mul.i   BP-68,#4,BP-68          ;mul.i   $t15,4,$t15
        add.i   BP+8,BP-68,BP-72        ;add.i   &a,$t15,$t16
        sub.i   BP-8,#1,BP-76           ;sub.i   j,1,$t17
        mul.i   BP-76,#4,BP-76          ;mul.i   $t17,4,$t17
        add.i   BP+8,BP-76,BP-80        ;add.i   &a,$t17,$t18
        mov.i   *BP-80,*BP-72           ;mov.i   $t18,$t16
        sub.i   BP-8,#1,BP-84           ;sub.i   j,1,$t19
        mul.i   BP-84,#4,BP-84          ;mul.i   $t19,4,$t19
        add.i   BP+8,BP-84,BP-88        ;add.i   &a,$t19,$t20
        realtoint.r BP-16,BP-92         ;realtoint.r tmp,$t21
        mov.i   BP-92,*BP-88            ;mov.i   $t21,$t20
        jump.i  #lab16                  ;jump.i  lab16
lab15:
lab16:
        add.i   BP-8,#1,BP-96           ;add.i   j,1,$t22
        mov.i   BP-96,BP-8              ;mov.i   $t22,j
        jump.i  #lab10                  ;jump.i  lab10
lab9:
        add.i   BP-4,#1,BP-100          ;add.i   i,1,$t23
        mov.i   BP-100,BP-4             ;mov.i   $t23,i
        jump.i  #lab6                   ;jump.i  lab6
lab5:
        leave                           ;leave   
        return                          ;return  
wypisztab:
        enter.i #20                     ;enter.i 20
        mov.i   #1,BP-4                 ;mov.i   1,i
lab18:
        jl.i    BP-4,#11,#lab19         ;jl.i    i,11,lab19
        mov.i   #0,BP-8                 ;mov.i   0,$t24
        jump.i  #lab20                  ;jump.i  lab20
lab19:
        mov.i   #1,BP-8                 ;mov.i   1,$t24
lab20:
        je.i    BP-8,#0,#lab17          ;je.i    $t24,0,lab17
        sub.i   BP-4,#1,BP-12           ;sub.i   i,1,$t25
        mul.i   BP-12,#4,BP-12          ;mul.i   $t25,4,$t25
        add.i   BP+8,BP-12,BP-16        ;add.i   &a,$t25,$t26
        write.i *BP-16                  ;write.i $t26
        add.i   BP-4,#1,BP-20           ;add.i   i,1,$t27
        mov.i   BP-20,BP-4              ;mov.i   $t27,i
        jump.i  #lab18                  ;jump.i  lab18
lab17:
        leave                           ;leave   
        return                          ;return  
lab0:
        push.i  #12                     ;push.i  &p
        call.i  #czytajtab              ;call.i  &czytajtab
        incsp.i #4                      ;incsp.i 4
        push.i  #12                     ;push.i  &p
        call.i  #bubblesort             ;call.i  &bubblesort
        incsp.i #4                      ;incsp.i 4
        push.i  #12                     ;push.i  &p
        call.i  #wypisztab              ;call.i  &wypisztab
        incsp.i #4                      ;incsp.i 4
        exit                            ;exit    
