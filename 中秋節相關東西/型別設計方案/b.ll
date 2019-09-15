; ModuleID = 'b.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.object = type { i8*, %union.anon }
%union.anon = type { %struct.closure }
%struct.closure = type { %struct.object*, {}* }
%union.basic_type = type { i64 }

@.str = private unnamed_addr constant [2 x i8] c"i\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"i2i\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%ld\00", align 1

; Function Attrs: nounwind uwtable
define void @plus(%struct.object* noalias sret %agg.result, %struct.object* %closure.coerce0, {}* %closure.coerce1, %struct.object* byval align 8 %a, %struct.object* byval align 8 %b) #0 {
  %closure = alloca %struct.closure, align 8
  %d = alloca %struct.object, align 8
  %1 = bitcast %struct.closure* %closure to { %struct.object*, {}* }*
  %2 = getelementptr inbounds { %struct.object*, {}* }, { %struct.object*, {}* }* %1, i32 0, i32 0
  store %struct.object* %closure.coerce0, %struct.object** %2, align 8
  %3 = getelementptr inbounds { %struct.object*, {}* }, { %struct.object*, {}* }* %1, i32 0, i32 1
  store {}* %closure.coerce1, {}** %3, align 8
  %4 = getelementptr inbounds %struct.object, %struct.object* %d, i32 0, i32 0
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8** %4, align 8
  %5 = getelementptr inbounds %struct.object, %struct.object* %a, i32 0, i32 1
  %6 = bitcast %union.anon* %5 to %union.basic_type*
  %7 = bitcast %union.basic_type* %6 to i64*
  %8 = load i64, i64* %7, align 8
  %9 = getelementptr inbounds %struct.object, %struct.object* %b, i32 0, i32 1
  %10 = bitcast %union.anon* %9 to %union.basic_type*
  %11 = bitcast %union.basic_type* %10 to i64*
  %12 = load i64, i64* %11, align 8
  %13 = add nsw i64 %8, %12
  %14 = getelementptr inbounds %struct.object, %struct.object* %d, i32 0, i32 1
  %15 = bitcast %union.anon* %14 to %union.basic_type*
  %16 = bitcast %union.basic_type* %15 to i64*
  store i64 %13, i64* %16, align 8
  %17 = bitcast %struct.object* %agg.result to i8*
  %18 = bitcast %struct.object* %d to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %17, i8* %18, i64 24, i32 8, i1 false)
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %a = alloca %struct.object, align 8
  %b = alloca %struct.object, align 8
  %clo = alloca %struct.object, align 8
  %d = alloca %struct.object, align 8
  store i32 0, i32* %1, align 4
  %2 = getelementptr inbounds %struct.object, %struct.object* %a, i32 0, i32 0
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8** %2, align 8
  %3 = getelementptr inbounds %struct.object, %struct.object* %a, i32 0, i32 1
  %4 = bitcast %union.anon* %3 to %union.basic_type*
  %5 = bitcast %union.basic_type* %4 to i64*
  store i64 6789, i64* %5, align 8
  %6 = getelementptr inbounds %struct.object, %struct.object* %b, i32 0, i32 0
  store i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8** %6, align 8
  %7 = getelementptr inbounds %struct.object, %struct.object* %b, i32 0, i32 1
  %8 = bitcast %union.anon* %7 to %union.basic_type*
  %9 = bitcast %union.basic_type* %8 to i64*
  store i64 6, i64* %9, align 8
  %10 = getelementptr inbounds %struct.object, %struct.object* %clo, i32 0, i32 0
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i8** %10, align 8
  %11 = getelementptr inbounds %struct.object, %struct.object* %clo, i32 0, i32 1
  %12 = bitcast %union.anon* %11 to %struct.closure*
  %13 = getelementptr inbounds %struct.closure, %struct.closure* %12, i32 0, i32 1
  %14 = bitcast {}** %13 to void (%struct.object*, %struct.object*, {}*, %struct.object*, %struct.object*)**
  store void (%struct.object*, %struct.object*, {}*, %struct.object*, %struct.object*)* @plus, void (%struct.object*, %struct.object*, {}*, %struct.object*, %struct.object*)** %14, align 8
  %15 = getelementptr inbounds %struct.object, %struct.object* %clo, i32 0, i32 1
  %16 = bitcast %union.anon* %15 to %struct.closure*
  %17 = getelementptr inbounds %struct.closure, %struct.closure* %16, i32 0, i32 1
  %18 = bitcast {}** %17 to void (%struct.object*, %struct.object*, {}*, %struct.object*, %struct.object*)**
  %19 = load void (%struct.object*, %struct.object*, {}*, %struct.object*, %struct.object*)*, void (%struct.object*, %struct.object*, {}*, %struct.object*, %struct.object*)** %18, align 8
  %20 = getelementptr inbounds %struct.object, %struct.object* %clo, i32 0, i32 1
  %21 = bitcast %union.anon* %20 to %struct.closure*
  %22 = bitcast %struct.closure* %21 to { %struct.object*, {}* }*
  %23 = getelementptr inbounds { %struct.object*, {}* }, { %struct.object*, {}* }* %22, i32 0, i32 0
  %24 = load %struct.object*, %struct.object** %23, align 8
  %25 = getelementptr inbounds { %struct.object*, {}* }, { %struct.object*, {}* }* %22, i32 0, i32 1
  %26 = load {}*, {}** %25, align 8
  call void %19(%struct.object* sret %d, %struct.object* %24, {}* %26, %struct.object* byval align 8 %a, %struct.object* byval align 8 %b)
  %27 = getelementptr inbounds %struct.object, %struct.object* %d, i32 0, i32 1
  %28 = bitcast %union.anon* %27 to %union.basic_type*
  %29 = bitcast %union.basic_type* %28 to i64*
  %30 = load i64, i64* %29, align 8
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), i64 %30)
  ret i32 0
}

declare i32 @printf(i8*, ...) #2

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)"}
