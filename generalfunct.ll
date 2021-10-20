target triple="i686"
%struct.IntHolder = type {i32}
@interval = common global i32 0, align 4
@end = common global i32 0, align 4

define i32 @multBy4xTimes(%struct.IntHolder* %num, i32 %timesLeft)
{
LU1:
	%u3 = icmp sle i32 %timesLeft, 0
	br i1 %u3, label %LU2, label %LU3
LU2:
	%u4 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	%u5 = load i32* %u4
	br label %LU0
LU3:
	br label %LU4
LU4:
	%u6 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	%u7 = load i32* %u6
	%u8 = mul i32 4, %u7
	%u9 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	store i32 %u8, i32* %u9
	%u10 = sub i32 %timesLeft, 1
	%u11 = call i32 @multBy4xTimes(%struct.IntHolder* %num, i32 %u10)
	%u12 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	%u13 = load i32* %u12
	br label %LU0
LU0:
	%phi0 = phi i32 [%u5, %LU2], [%u13, %LU4]
	ret i32 %phi0
}
define void @divideBy8(%struct.IntHolder* %num)
{
LU6:
	%u16 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	%u17 = load i32* %u16
	%u18 = sdiv i32 %u17, 2
	%u19 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	store i32 %u18, i32* %u19
	%u20 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	%u21 = load i32* %u20
	%u22 = sdiv i32 %u21, 2
	%u23 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	store i32 %u22, i32* %u23
	%u24 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	%u25 = load i32* %u24
	%u26 = sdiv i32 %u25, 2
	%u27 = getelementptr %struct.IntHolder* %num, i1 0, i32 0
	store i32 %u26, i32* %u27
	br label %LU5
LU5:
	ret void
}
define i32 @main()
{
LU8:
	%u38 = call i8* @malloc(i32 4)
	%u39 = bitcast i8* %u38 to %struct.IntHolder*
	store i32 1000000, i32* @end
	%u42 = call i32 @readInt()
	%u44 = call i32 @readInt()
	store i32 %u44, i32* @interval
	call void @printLineInt(i32 %u42)
	%u46 = load i32* @interval
	call void @printLineInt(i32 %u46)
	br label %LU9
LU9:
	%phi16 = phi i32 [0, %LU8], [%phi15, %LU12]
	%phi8 = phi i32 [0, %LU8], [%u94, %LU12]
	%phi5 = phi %struct.IntHolder* [%u39, %LU8], [%phi11, %LU12]
	%u52 = load i32* @end
	%u53 = icmp sle i32 0, %u52
	br i1 %u53, label %LU11, label %LU12
LU11:
	%phi10 = phi i32 [%phi8, %LU9], [%phi10, %LU15]
	%phi2 = phi %struct.IntHolder* [%phi5, %LU9], [%phi2, %LU15]
	%phi1 = phi i32 [0, %LU9], [%u84, %LU15]
	%u66 = add i32 %phi1, 1
	%u69 = getelementptr %struct.IntHolder* %phi2, i1 0, i32 0
	store i32 %u66, i32* %u69
	%u70 = getelementptr %struct.IntHolder* %phi2, i1 0, i32 0
	%u73 = call i32 @multBy4xTimes(%struct.IntHolder* %phi2, i32 2)
	call void @divideBy8(%struct.IntHolder* %phi2)
	%u74 = load i32* @interval
	%u75 = sub i32 %u74, 1
	%u77 = icmp sle i32 %u75, 0
	br i1 %u77, label %LU13, label %LU14
LU13:
	br label %LU15
LU14:
	br label %LU15
LU15:
	%phi4 = phi i32 [1, %LU13], [%u75, %LU14]
	%u84 = add i32 %u66, %phi4
	%u86 = load i32* @end
	%u87 = icmp sle i32 %u84, %u86
	br i1 %u87, label %LU11, label %LU12
LU12:
	%phi15 = phi i32 [%phi16, %LU9], [39916800, %LU15]
	%phi13 = phi i32 [0, %LU9], [%u84, %LU15]
	%phi11 = phi %struct.IntHolder* [%phi5, %LU9], [%phi2, %LU15]
	%phi7 = phi i32 [%phi8, %LU9], [%phi10, %LU15]
	%u94 = add i32 %phi7, 1
	%u96 = icmp slt i32 %u94, 50
	br i1 %u96, label %LU9, label %LU10
LU10:
	%phi14 = phi i32 [0, %LU8], [%phi15, %LU12]
	%phi12 = phi i32 [0, %LU8], [%phi13, %LU12]
	call void @printLineInt(i32 %phi12)
	call void @printLineInt(i32 %phi14)
	br label %LU7
LU7:
	ret i32 0
}
declare i8* @malloc(i32)
declare void @free(i8*)
declare void @printInt(i32)
declare void @printLineInt(i32)
declare i32 @readInt()