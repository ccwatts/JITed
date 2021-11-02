%struct.A = type {i32, i32}
@x = common global i32 0, align 4
define i32 @changeX(i32 %x)
{
LU0:
br label %LU1
LU1:
%phi0 = phi i32[%x, %LU0]
ret i32 %phi0
}
