program sort(input,output);
var j,i,o:integer;
var p :array [1..10] of integer;
var o :array [1..10] of real;
var oo :array [1..10] of real;
var b:integer;

procedure czytajtab(a: array[1..10] of integer; g: array [1..10] of real; z : integer; zz : integer);
begin
 a[1]:=10;
 a[6]:=a[5];
 a[3]:=g[2];
 g[7]:=a[4];
 a[0]:=1.2;
 g[0]:=19
end;

procedure bubblesort(a:array[1..10] of integer; g: array [1..10] of real;  z : integer; zz : integer);
begin
 czytajtab(a, o, z, j);
 a[2]:=10;
 a[6]:=a[5];
 a[3]:=g[2];
 g[7]:=a[4];
 a[0]:=1.2;
 g[0]:=19
end;

begin
  czytajtab(p, o, i, i);
  p[3]:=10;
  p[7]:=p[4];
  o[3]:=p[2];
  p[7]:=oo[4];
  p[0]:=1.2;
  oo[0]:=19;
  oo[20.2]:=19;
  o[20.2]:=19.2
end.
