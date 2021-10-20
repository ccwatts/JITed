%struct.union_t = type { i32*, i8** }

@.str = private unnamed_addr constant [4 x i8] c"Hi!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @func1() #0 {
  ret i32 3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @func2() #0 {
  ret i32 4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @add1(i32 %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = add nsw i32 %3, 1
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @add2(i32 %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = add nsw i32 %3, 2
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @mult3(i32 %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = mul nsw i32 %3, 3
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @multiple_args(i32* %0) #0 {
  %2 = alloca i32*, align 8
  store i32* %0, i32** %2, align 8
  %3 = load i32*, i32** %2, align 8
  %4 = getelementptr inbounds i32, i32* %3, i64 0
  %5 = load i32, i32* %4, align 4
  %6 = load i32*, i32** %2, align 8
  %7 = getelementptr inbounds i32, i32* %6, i64 1
  %8 = load i32, i32* %7, align 4
  %9 = add nsw i32 %5, %8
  ret i32 %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @ut(i32* %0, i8** %1) #0 {
  %3 = alloca %struct.union_t, align 8
  %4 = bitcast %struct.union_t* %3 to { i32*, i8** }*
  %5 = getelementptr inbounds { i32*, i8** }, { i32*, i8** }* %4, i32 0, i32 0
  store i32* %0, i32** %5, align 8
  %6 = getelementptr inbounds { i32*, i8** }, { i32*, i8** }* %4, i32 0, i32 1
  store i8** %1, i8*** %6, align 8
  %7 = call i32 @func1()
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @indirect2() #0 {
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0))
  ret i32 42
}

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @indirect1() #0 {
  %1 = call i32 @indirect2()
  ret i32 %1
}
