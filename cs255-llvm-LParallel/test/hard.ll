; ModuleID = 'hard.bc'
source_filename = "hard.bc"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @hardbench(i32* %a, i32* %b, i32* %c) #0 {
  %1 = alloca i32*, align 8
  %2 = alloca i32*, align 8
  %3 = alloca i32*, align 8
  %temp = alloca i32, align 4
  %val = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %t = alloca i32, align 4
  store i32* %a, i32** %1, align 8
  store i32* %b, i32** %2, align 8
  store i32* %c, i32** %3, align 8
  store i32 0, i32* %t, align 4
  store i32 0, i32* %t, align 4
  br label %4

; <label>:4:                                      ; preds = %16, %0
  %5 = load i32, i32* %t, align 4
  %6 = icmp slt i32 %5, 100
  br i1 %6, label %7, label %19

; <label>:7:                                      ; preds = %4
  %8 = load i32, i32* %t, align 4
  %9 = mul nsw i32 %8, 2
  %10 = add nsw i32 %9, 3
  store i32 %10, i32* %val, align 4
  %11 = load i32, i32* %val, align 4
  %12 = load i32, i32* %t, align 4
  %13 = sext i32 %12 to i64
  %14 = load i32*, i32** %1, align 8
  %15 = getelementptr inbounds i32, i32* %14, i64 %13
  store i32 %11, i32* %15, align 4
  br label %16

; <label>:16:                                     ; preds = %7
  %17 = load i32, i32* %t, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, i32* %t, align 4
  br label %4

; <label>:19:                                     ; preds = %4
  store i32 1, i32* %i, align 4
  br label %20

; <label>:20:                                     ; preds = %35, %19
  %21 = load i32, i32* %i, align 4
  %22 = icmp slt i32 %21, 100
  br i1 %22, label %23, label %38

; <label>:23:                                     ; preds = %20
  %24 = load i32, i32* %i, align 4
  %25 = sub nsw i32 %24, 1
  %26 = sext i32 %25 to i64
  %27 = load i32*, i32** %1, align 8
  %28 = getelementptr inbounds i32, i32* %27, i64 %26
  %29 = load i32, i32* %28, align 4
  %30 = mul nsw i32 %29, 2
  %31 = load i32, i32* %i, align 4
  %32 = sext i32 %31 to i64
  %33 = load i32*, i32** %1, align 8
  %34 = getelementptr inbounds i32, i32* %33, i64 %32
  store i32 %30, i32* %34, align 4
  br label %35

; <label>:35:                                     ; preds = %23
  %36 = load i32, i32* %i, align 4
  %37 = add nsw i32 %36, 1
  store i32 %37, i32* %i, align 4
  br label %20

; <label>:38:                                     ; preds = %20
  store i32 0, i32* %j, align 4
  br label %39

; <label>:39:                                     ; preds = %59, %38
  %40 = load i32, i32* %j, align 4
  %41 = icmp slt i32 %40, 100
  br i1 %41, label %42, label %62

; <label>:42:                                     ; preds = %39
  %43 = load i32, i32* %j, align 4
  %44 = sub nsw i32 99, %43
  %45 = sext i32 %44 to i64
  %46 = load i32*, i32** %1, align 8
  %47 = getelementptr inbounds i32, i32* %46, i64 %45
  %48 = load i32, i32* %47, align 4
  %49 = load i32, i32* %j, align 4
  %50 = sext i32 %49 to i64
  %51 = load i32*, i32** %1, align 8
  %52 = getelementptr inbounds i32, i32* %51, i64 %50
  %53 = load i32, i32* %52, align 4
  %54 = sub nsw i32 %48, %53
  %55 = load i32, i32* %j, align 4
  %56 = sext i32 %55 to i64
  %57 = load i32*, i32** %2, align 8
  %58 = getelementptr inbounds i32, i32* %57, i64 %56
  store i32 %54, i32* %58, align 4
  br label %59

; <label>:59:                                     ; preds = %42
  %60 = load i32, i32* %j, align 4
  %61 = add nsw i32 %60, 1
  store i32 %61, i32* %j, align 4
  br label %39

; <label>:62:                                     ; preds = %39
  store i32 2, i32* %k, align 4
  br label %63

; <label>:63:                                     ; preds = %85, %62
  %64 = load i32, i32* %k, align 4
  %65 = icmp slt i32 %64, 48
  br i1 %65, label %66, label %88

; <label>:66:                                     ; preds = %63
  %67 = load i32, i32* %k, align 4
  %68 = mul nsw i32 %67, 2
  %69 = sub nsw i32 %68, 3
  %70 = sext i32 %69 to i64
  %71 = load i32*, i32** %3, align 8
  %72 = getelementptr inbounds i32, i32* %71, i64 %70
  %73 = load i32, i32* %72, align 4
  %74 = load i32, i32* %k, align 4
  %75 = sub nsw i32 %74, 1
  %76 = sext i32 %75 to i64
  %77 = load i32*, i32** %2, align 8
  %78 = getelementptr inbounds i32, i32* %77, i64 %76
  %79 = load i32, i32* %78, align 4
  %80 = add nsw i32 %73, %79
  %81 = load i32, i32* %k, align 4
  %82 = sext i32 %81 to i64
  %83 = load i32*, i32** %3, align 8
  %84 = getelementptr inbounds i32, i32* %83, i64 %82
  store i32 %80, i32* %84, align 4
  br label %85

; <label>:85:                                     ; preds = %66
  %86 = load i32, i32* %k, align 4
  %87 = add nsw i32 %86, 1
  store i32 %87, i32* %k, align 4
  br label %63

; <label>:88:                                     ; preds = %63
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (branches/release_38 259745) (llvm/branches/release_38 259685)"}
