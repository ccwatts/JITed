; ModuleID = 'ttt.c'
source_filename = "ttt.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.union_t = type { i32*, i8** }

@.str = private unnamed_addr constant [12 x i8] c"Hello world\00", align 1
@s = dso_local global i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0), align 8
@__const.ut.rt = private unnamed_addr constant %struct.union_t { i32* null, i8** @s }, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local { i32*, i8** } @ut(i32* %0, i8** %1) #0 {
  %3 = alloca %struct.union_t, align 8
  %4 = alloca %struct.union_t, align 8
  %5 = bitcast %struct.union_t* %4 to { i32*, i8** }*
  %6 = getelementptr inbounds { i32*, i8** }, { i32*, i8** }* %5, i32 0, i32 0
  store i32* %0, i32** %6, align 8
  %7 = getelementptr inbounds { i32*, i8** }, { i32*, i8** }* %5, i32 0, i32 1
  store i8** %1, i8*** %7, align 8
  %8 = bitcast %struct.union_t* %3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %8, i8* align 8 bitcast (%struct.union_t* @__const.ut.rt to i8*), i64 16, i1 false)
  %9 = bitcast %struct.union_t* %3 to { i32*, i8** }*
  %10 = load { i32*, i8** }, { i32*, i8** }* %9, align 8
  ret { i32*, i8** } %10
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1~18.04.2 "}
