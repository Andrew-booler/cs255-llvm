; ModuleID = 'test.bc.opt'
source_filename = "<stdin>"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
BB_:
  %0 = alloca i32, align 4
  %c = alloca [100 x i8], align 16
  %d = alloca [50 x i8], align 16
  %e = alloca [50 x i8], align 16
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %0, align 4
  store i32 0, i32* %k, align 4
  store i32 25, i32* %j, align 4
  store i32 0, i32* %i, align 4
  br label %BB_1

BB_1:                                             ; preds = %BB_3, %BB_
  %1 = load i32, i32* %i, align 4
  %2 = icmp slt i32 %1, 10
  br i1 %2, label %BB_2, label %BB_4

BB_2:                                             ; preds = %BB_1
  %3 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i64 0, i64 4
  %c0 = load i8, i8* %3, align 4
  %4 = load i32, i32* %i, align 4
  %5 = sext i32 %4 to i64
  %c1 = getelementptr inbounds [100 x i8], [100 x i8]* %c, i64 0, i64 %5
  store i8 %c0, i8* %c1, align 1
  %6 = load i32, i32* %i, align 4
  %7 = sub nsw i32 %6, 3
  %8 = sext i32 %7 to i64
  %9 = getelementptr inbounds [50 x i8], [50 x i8]* %d, i64 0, i64 %8
  %d2 = load i8, i8* %9, align 1
  %10 = sext i8 %d2 to i32
  %11 = add nsw i32 %10, 2
  %12 = trunc i32 %11 to i8
  %13 = load i32, i32* %i, align 4
  %14 = sext i32 %13 to i64
  %d3 = getelementptr inbounds [50 x i8], [50 x i8]* %d, i64 0, i64 %14
  store i8 %12, i8* %d3, align 1
  br label %BB_3

BB_3:                                             ; preds = %BB_2
  %15 = load i32, i32* %i, align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, i32* %i, align 4
  br label %BB_1

BB_4:                                             ; preds = %BB_1
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (branches/release_38 259745) (llvm/branches/release_38 259685)"}
