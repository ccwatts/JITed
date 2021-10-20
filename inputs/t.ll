%struct.union_t = type { i32*, i8** }

@.str = private unnamed_addr constant [12 x i8] c"Hello world\00", align 1
@s = dso_local global i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0), align 8
@__const.ut.rt = private unnamed_addr constant %struct.union_t { i32* null, i8** @s }, align 8

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

declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1
